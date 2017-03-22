#ifndef IMAGE_H
#define IMAGE_H

#include <QPixmap>

class ImageMatrix
{
//private:
public:
    QPixmap originalImage;
    double * grayMatrix;
    double * gradientMatrix;
    double * gausMatrix;
public:
    ImageMatrix();
    ~ImageMatrix();
    bool loadImage();
    double * ratingMatrix(double * matrix, int width, int height);
    QPixmap getGrayImg();
    QPixmap getSobelImg();
    QPixmap getGausImg();
private:
    QPixmap createImgFromMatrix(double* matrix, int width, int height);
    void setOriginalImg();
    void createGrayMatrix();
    void sobel();
    void gaus();
    double * sobelOperator(double * matrixX, double * matrixY, int width, int height);
    double * convolution(double * matrixOrig, int width, int height, double * matrix, int dimensionX, int dimensionY);
    double * convolutionAxist(double * matrFirst, double * matrSecond, int dimensionMatrixConv);
    double getBlurRadius(double sigma, int i, int j, int dimention);
    double * getBlurRadius(double sigma, int dimention);
    int getDimentionRadius(double sigma);

    QVector<double> createGradientMatrix();
    QVector<double> gradientByX(QVector<double> matrix);
    QVector<double> gradientByY(QVector<double> matrix);
    QVector<double> ratingMatrix(QVector<double> matrix);
   // doub convolution(double *matrix, int width, int height, double *derivativeMatrix, int n, int m);
};

#endif // IMAGE_H
