#ifndef EDITMATRIX_H
#define EDITMATRIX_H

#include "imageMatrix.h"
#include <memory>

class editmatrix
{
public:
    editmatrix();
    ImageMatrix * sobel(ImageMatrix * imageMatrix);
    ImageMatrix * gaus(ImageMatrix * imageMatrix);
private:
    unique_ptr<double []> sobelOperator(const double * matrixX, const double * matrixY, int width, int height);
    unique_ptr<double []> getBlurRadius(double sigma, int dimention);
    int getDimentionRadius(double sigma);
};

#endif // EDITMATRIX_H
