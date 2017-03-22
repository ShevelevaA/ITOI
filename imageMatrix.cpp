#define _USE_MATH_DEFINES
#include <math.h>
#include "imageMatrix.h"
#include <iostream>
ImageMatrix::ImageMatrix()
{
    grayMatrix = nullptr;
}

ImageMatrix::~ImageMatrix()
{
}

bool ImageMatrix::loadImage()
{
    QString imgName = QFileDialog::getOpenFileName(0, "Open dialog", "", "*.jpg *.png");
    if (!imgName.isEmpty())
    {
        QPixmap qPixmap;
        if(!qPixmap.load(imgName)){
            return false;
        }
        originalImage = qPixmap;
        return true;
    }
    return false;
}

QPixmap ImageMatrix::createImgFromMatrix(double* matrix, int width, int height){
    QColor it;
    QImage result(width, height, QImage::Format_RGB32);
    for(int j = 0; j < height; j++)
    {
        for(int i = 0; i < width; i++)
        {
            it.setRgb(matrix[i+j*width], matrix[i+j*width], matrix[i+j*width]);
            result.setPixel(i,j, it.rgb());
        }
    }

    return  QPixmap::fromImage(result);
}

void ImageMatrix::createGrayMatrix(){
    if(grayMatrix == nullptr){
        int width = originalImage.width();
        int height = originalImage.height();

        grayMatrix = new double[width * height];

        for(int j = 0; j < height; j++)
        {
            for(int i = 0; i < width; i++)
            {
                QColor pixel = (QColor)originalImage.toImage().pixel(i,j);
                grayMatrix[i+j*width] = (0.213*(pixel.red()) + 0.715*((pixel.green())) + 0.072*((pixel.blue())));
            }
        }
    }
}

QPixmap ImageMatrix::getGrayImg(){
    createGrayMatrix();
    return createImgFromMatrix(grayMatrix, originalImage.width(), originalImage.height());
}

QPixmap ImageMatrix::getSobelImg(){
    sobel();
    return createImgFromMatrix(gradientMatrix, originalImage.width(), originalImage.height());
}

QPixmap ImageMatrix::getGausImg(){
    gaus();
    return createImgFromMatrix(gausMatrix, originalImage.width(), originalImage.height());
}

void ImageMatrix::sobel(){
    double matrixX[3] = {1,2,1};
    double matrixY[3] = {1,0,-1};
    int dimensionMatrixConv = 3;
    int width = originalImage.width();
    int height = originalImage.height();
    double * resultMatrix = new double[width * height];
    createGrayMatrix();
    resultMatrix = sobelOperator(convolutionAxist(matrixY, matrixX, dimensionMatrixConv), convolutionAxist(matrixX, matrixY, dimensionMatrixConv), width, height); // свертка по Х, Y
    gradientMatrix = ratingMatrix(resultMatrix, width, height);
}

double * ImageMatrix::sobelOperator(double * matrixX, double * matrixY, int width, int height){
    double * resultMatrix;
    resultMatrix = new double[width * height];
    for(int j = 0; j< height; j++)
    {
        for(int i = 0; i< width; i++)
        {
            double x = matrixX[i + j * width];
            resultMatrix[i + j * width]= sqrt(pow(x,2) + pow(matrixY[i + j * width],2));
        }
    }
    return resultMatrix;
}

double * ImageMatrix::convolutionAxist(double * matrFirst, double * matrSecond, int dimensionMatrixConv){
    int width = originalImage.width();
    int height = originalImage.height();
    createGrayMatrix();
    double *matrix, *matrix1;
    matrix1 = convolution(grayMatrix, width, height, matrFirst, 1, dimensionMatrixConv);
    matrix = convolution(matrix1, width, height, matrSecond, dimensionMatrixConv, 1);
    return matrix;
}

double getMatrixElem(double * matrix, int width, int height, int coordX, int coordY){
    if(coordX > -1 && coordX < width && coordY > -1 && coordY < height){
        return matrix[coordX + coordY * width];
    }
    return 0;
}

double * ImageMatrix::convolution(double * matrixOrig, int width, int height, double * matrix, int dimensionX, int dimensionY){

    double *resultMatrix;
    resultMatrix = new double[width * height];
    double sum=0;

    for(int j = 0; j< height; j++)
    {
        for(int i = 0; i< width; i++)
        {
            for(int k = 0; k < dimensionY; k++){
                for(int l = 0; l < dimensionX; l++){
                    sum += matrix[l + k * dimensionX] * getMatrixElem(matrixOrig, width, height, dimensionX/2 + i - l, dimensionY/2 + j - k);
                }
            }
            resultMatrix[i + j * width] = sum;
            sum = 0;
        }
    }
    return resultMatrix;
}

double * ImageMatrix::ratingMatrix(double * matrix, int width, int height){
    double max = *std::max_element(&matrix[0], &matrix[width*height - 1]);
    double min = *std::min_element(&matrix[0], &matrix[width*height - 1]);
    int difference = max - min;
    if(difference > 0){
        for(int j = 0; j< height; j++)
        {
            for(int i = 0; i< width; i++)
            {
                matrix[i + j * width] = (( matrix[i + j * width] - min) / difference) * 255;
            }
        }

    }
    return matrix;
}

double * ImageMatrix::getBlurRadius(double sigma, int dimention){
    int coord;
    double  multiplier = 1.0/(sqrt(2 * M_PI) * sigma);
    double expon;
    double sum = 0;
    double * array;
    array = new double[dimention];
    for(int i = 0; i < dimention; i++){
        coord = i - dimention/2;
        expon = pow(M_E, ((-1.0) * pow(coord, 2))/(2 * pow(sigma, 2)));
        array[i] = multiplier * expon;
        sum += array[i];
    }

    return array;
}

int ImageMatrix::getDimentionRadius(double sigma){
    int first = (int)(sigma * 3);
    return first * 2;
}

void ImageMatrix::gaus(){
    double sigma = 4;
    int dimentionRadius = getDimentionRadius(sigma);
    double * blurMatrix;
    blurMatrix = getBlurRadius(sigma, dimentionRadius);
    gausMatrix = ratingMatrix(convolutionAxist(blurMatrix, blurMatrix, dimentionRadius), originalImage.width(), originalImage.height());
}


