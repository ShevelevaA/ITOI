#ifndef IMAGE_H
#define IMAGE_H

#include <QPixmap>
#include <memory>

using namespace std;

class ImageMatrix
{
private:
    unique_ptr<double []> matrix;
    int width;
    int height;
public:
    ImageMatrix();
    ImageMatrix(const QPixmap &picture);
    ImageMatrix(const double *matrix, int width, int height);
    ImageMatrix& operator=(ImageMatrix& imgMatrix) = default;
    QPixmap createImgFromMatrix();
    void ratingMatrix();
    unique_ptr<double []> convolutionAxist(const double * matrFirst, const double * matrSecond, int dimensionMatrixConv);
    int getWidth();
    int getHeight();
private:
   void createMatrixFromPicture();
   unique_ptr<double []> convolution(const double * matrixOrig, int width, int height, const double * matrix, int dimensionX, int dimensionY);
   double getMatrixElem(const double * matrix, int width, int height, int coordX, int coordY);
};

#endif // IMAGE_H
