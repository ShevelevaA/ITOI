#include "imageMatrix.h"

ImageMatrix::ImageMatrix()
{
}

ImageMatrix::ImageMatrix(const QPixmap &picture)
{
    this->width = picture.width();
    this->height = picture.height();

    this->matrix = make_unique<double []>(this->width * this->height);

    for(int j = 0; j < this->height; j++)
    {
        for(int i = 0; i < this->width; i++)
        {
            QColor pixel = (QColor)picture.toImage().pixel(i,j);
            this->matrix[i + j * this->width] = (0.213*(pixel.red()) + 0.715*((pixel.green())) + 0.072*((pixel.blue())));
        }
    }
}

ImageMatrix::ImageMatrix(const double * matrix, int width, int height)
{
    this->width = width;
    this->height = height;

    this->matrix = make_unique<double []>(this->width * this->height);
    for(int i = 0; i < this->width * this->height; i++){
        this->matrix[i] = matrix[i];
    }
}

QPixmap ImageMatrix::createImgFromMatrix(){
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

int ImageMatrix::getWidth(){
    return this->width;
}

int ImageMatrix::getHeight(){
    return this->height;
}

unique_ptr<double []> ImageMatrix::convolutionAxist(const double * matrFirst, const double * matrSecond, int dimensionMatrixConv){
    auto matrix1 = convolution(this->matrix.get(), this->width, this->height, matrFirst, 1, dimensionMatrixConv);
    auto matrix2 = convolution(matrix1.get(), this->width, this->height, matrSecond, dimensionMatrixConv, 1);
    return matrix2;
}

double ImageMatrix::getMatrixElem(const double * matrix, int width, int height, int coordX, int coordY){
    if(coordX > -1 && coordX < width && coordY > -1 && coordY < height){
        return matrix[coordX + coordY * width];
    }
    return 0;
}

unique_ptr<double []> ImageMatrix::convolution(const double * matrixOrig, int width, int height, const double * matrix, int dimensionX, int dimensionY)
{
    double sum=0;

    auto resultMatrix = make_unique<double []>(width * height);

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


void  ImageMatrix::ratingMatrix(){
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
}
