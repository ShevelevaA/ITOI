#ifndef PYRAMID_H
#define PYRAMID_H

#include <vector>
#include "imageMatrix.h"
#include "convmatrix.h"

struct Layer{
ImageMatrix * imageMatrix;
double sigma;
};

class Pyramid
{
private:
    double startSigma;
    double zeroSigma;
    int layerCount;
    int octavesCount;
    vector<vector<Layer>> octavesVector;
public:
    Pyramid();
    ImageMatrix *getPyramidImage(ImageMatrix * imageMatrix);
    vector<vector<Layer>> changeImageSize(ImageMatrix * imageMatrix);
private:
    int accountOctaves(int width, int height);
    double accountDeltaSigma(double sigmaPrevious, double sigmaFollowing);
};

#endif // PYRAMID_H
