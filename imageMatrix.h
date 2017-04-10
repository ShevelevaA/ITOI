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
    int edgeProcessing;
public:
    ImageMatrix();
    ImageMatrix(const QPixmap &picture);
    ImageMatrix(const double *matrix, int width, int height);
    ImageMatrix& operator=(ImageMatrix& imgMatrix) = default;
    QPixmap createImgFromMatrix();
    void ratingMatrix();
    ImageMatrix* ImageMatrix::convolutionAxistImage(const double * matrFirst, const double * matrSecond, int dimensionMatrixConv);
    ImageMatrix* convolutionAxistXYImage(const double * matrFirst, const double * matrSecond, int dimensionMatrixConv);
    ImageMatrix* reduceImage();
    int getWidth();
    int getHeight();
private:
   void createMatrixFromPicture();
   unique_ptr<double []> convolution(const double * matrixOrig, int width, int height, const double * matrix, int dimensionX, int dimensionY);
   unique_ptr<double []> convolutionAxist(const double * matrFirst, const double * matrSecond, int dimensionMatrixConv);
   double getMatrixElem(const double * matrix, int width, int height, int coordX, int coordY);
   double getThisMatrixElem(int coordX, int coordY);
   double edgeValue(const double * matrix, int width, int height, int coordX, int coordY);
   double reflectionEdge(const double * matrix, int width, int height, int coordX, int coordY);
   double turnImage(const double * matrix, int width, int height, int coordX, int coordY);
   double getAverage(int x, int y);

};

#endif // IMAGE_H
