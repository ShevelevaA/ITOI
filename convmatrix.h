#ifndef EDITMATRIX_H
#define EDITMATRIX_H

#include "imageMatrix.h"
#include <memory>

class Convolution
{
public:
    Convolution();
    ImageMatrix * sobel(ImageMatrix * imageMatrix);
    ImageMatrix * gaus(ImageMatrix * imageMatrix, double sigma);
private:
    unique_ptr<double []> sobelOperator(const double * matrixX, const double * matrixY, int width, int height);
    unique_ptr<double []> getBlurRadius(double sigma, int dimention);
    int getDimentionRadius(double sigma);
};

#endif // EDITMATRIX_H
