#define _USE_MATH_DEFINES
#include "editmatrix.h"

#include <math.h>
#include <iostream>

editMatrix::editMatrix()
{
    dimensionMatrixConv = 3;
    gausMatrixX = make_unique<double []>(dimensionMatrixConv);
    gausMatrixX[0] = 1;
    gausMatrixX[1] = 2;
    gausMatrixX[2] = 1;
    gausMatrixY = make_unique<double []>(dimensionMatrixConv);
    gausMatrixY[0] = 1;
    gausMatrixY[1] = 0;
    gausMatrixY[2] = -1;
}

ImageMatrix * editMatrix::sobel(ImageMatrix * imageMatrix){
    int width = imageMatrix->getWidth();
    int height = imageMatrix->getHeight();
    auto resultMatrix = make_unique<double []>(width * height);
    resultMatrix = sobelOperator((imageMatrix->convolutionAxist(gausMatrixY.get(), gausMatrixX.get(), dimensionMatrixConv)).get(),
                                 (imageMatrix->convolutionAxist(gausMatrixX.get(), gausMatrixY.get(), dimensionMatrixConv)).get(), width, height); // свертка по Х, Y
    return new ImageMatrix(resultMatrix.get(), width, height);
}

unique_ptr<double []> editMatrix::sobelOperator(const double * matrixX, const double * matrixY, int width, int height){
    auto resultMatrix = make_unique<double []>(width * height);
    for(int j = 0; j< height; j++)
    {
        for(int i = 0; i< width; i++)
        {
            resultMatrix[i + j * width]= sqrt(pow(matrixX[i + j * width],2) + pow(matrixY[i + j * width],2));
        }
    }
    return resultMatrix;
}

unique_ptr<double []> editMatrix::getBlurRadius(double sigma, int dimention){
    int coord;
    double  multiplier = 1.0/(sqrt(2 * M_PI) * sigma);
    double expon;
    double sum = 0;
    auto array = make_unique<double []>(dimention);
    for(int i = 0; i < dimention; i++){
        coord = i - dimention/2;
        expon = pow(M_E, ((-1.0) * pow(coord, 2))/(2 * pow(sigma, 2)));
        array[i] = multiplier * expon;
        sum += array[i];
    }
    return array;
}

int editMatrix::getDimentionRadius(double sigma){
    int first = (int)(sigma * 3);
    return first * 2;
}

ImageMatrix * editMatrix::gaus(ImageMatrix * imageMatrix){
    double sigma = 4;
    int dimentionRadius = getDimentionRadius(sigma);
    auto blurMatrix = getBlurRadius(sigma, dimentionRadius);
    return new ImageMatrix((imageMatrix->convolutionAxist(blurMatrix.get(), blurMatrix.get(), dimentionRadius)).get(), imageMatrix->getWidth(), imageMatrix->getHeight());
}

