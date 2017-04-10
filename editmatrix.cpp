#define _USE_MATH_DEFINES
#include "convmatrix.h"

#include <math.h>
#include <iostream>

Convolution::Convolution()
{
}

ImageMatrix * Convolution::sobel(ImageMatrix * imageMatrix){
    int dimensionMatrixConv = 3;
    double matrixX[3] = {1,2,1};
    double matrixY[3] = {1,0,-1};
    return imageMatrix->convolutionAxistXYImage(matrixY, matrixX, dimensionMatrixConv);
}

unique_ptr<double []> Convolution::getBlurRadius(double sigma, int dimention){
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

int Convolution::getDimentionRadius(double sigma){
    int first = (int)(sigma * 3);
    return first * 2;
}

ImageMatrix * Convolution::gaus(ImageMatrix * imageMatrix, double sigma){
   // double sigma = 4;
    int dimentionRadius = getDimentionRadius(sigma);
    auto blurMatrix = getBlurRadius(sigma, dimentionRadius);
    return imageMatrix->convolutionAxistImage(blurMatrix.get(), blurMatrix.get(), dimentionRadius);
}

