#include "imageMatrix.h"

ImageMatrix::ImageMatrix()
{
}

ImageMatrix::ImageMatrix(const QPixmap &picture)
{
    this->width = picture.width();
    this->height = picture.height();
    this->edgeProcessing = 1;

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
    this->edgeProcessing = 0;
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

ImageMatrix* ImageMatrix::convolutionAxistImage(const double * matrFirst, const double * matrSecond, int dimensionMatrixConv){
    return new ImageMatrix((convolutionAxist(matrFirst, matrSecond, dimensionMatrixConv)).get(), this->width, this->height);
}

unique_ptr<double []> ImageMatrix::convolutionAxist(const double * matrFirst, const double * matrSecond, int dimensionMatrixConv){
    auto matrix1 = convolution(this->matrix.get(), this->width, this->height, matrFirst, 1, dimensionMatrixConv);
    auto matrix2 = convolution(matrix1.get(), this->width, this->height, matrSecond, dimensionMatrixConv, 1);
    return matrix2;
}

ImageMatrix* ImageMatrix::convolutionAxistXYImage(const double * matrFirst, const double * matrSecond, int dimensionMatrixConv){
    auto resultMatrix = make_unique<double []>(width * height);
    auto matrixX = make_unique<double []>(width * height);
    auto matrixY = make_unique<double []>(width * height);
    matrixX = convolutionAxist(matrFirst, matrSecond, dimensionMatrixConv);
    matrixY = convolutionAxist(matrSecond, matrFirst, dimensionMatrixConv);
    for(int j = 0; j< height; j++)
        {
            for(int i = 0; i< width; i++)
            {
                resultMatrix[i + j * width]= sqrt(pow(matrixX[i + j * width],2) + pow(matrixY[i + j * width],2));
            }
        }
    return new ImageMatrix(resultMatrix.get(), width, height);
}

double ImageMatrix::getMatrixElem(const double * matrix, int width, int height, int coordX, int coordY){
    if(coordX > -1 && coordX < width && coordY > -1 && coordY < height){
        return matrix[coordX + coordY * width];
    }
    switch(edgeProcessing){
    case 0: return 0;
    case 1: return edgeValue(matrix, width, height, coordX, coordY);
    case 2: return reflectionEdge(matrix, width, height, coordX, coordY);
    default: return turnImage(matrix, width, height, coordX, coordY);
    }
}

double ImageMatrix::edgeValue(const double * matrix, int width, int height, int coordX, int coordY){
    if(coordX < 0)
        coordX = 0;
    if(coordX >= width)
        coordX = width - 1;
    if(coordY < 0)
        coordY = 0;
    if(coordY >= height)
        coordY = height - 1;
    return matrix[coordX + coordY * width];
}

double ImageMatrix::reflectionEdge(const double * matrix, int width, int height, int coordX, int coordY){
    if(coordX < 0)
        coordX = abs(coordX);
    if(coordX >= width)
        coordX = width - (coordX - width) - 1;
    if(coordY < 0)
        coordY = abs(coordY);
    if(coordY >= height)
        coordY = height - (coordY - height) - 1;
    return matrix[coordX + coordY * width];
}

double ImageMatrix::turnImage(const double * matrix, int width, int height, int coordX, int coordY){
    if(coordX < 0)
        coordX = width + coordX;
    if(coordX >= width)
        coordX = 1 + (coordX - width);
    if(coordY < 0)
        coordY = height + coordY;
    if(coordY >= height)
        coordY = 1 + (coordY - height);
    return matrix[coordX + coordY * width];
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
