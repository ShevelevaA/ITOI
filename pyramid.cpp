#include "pyramid.h"

Pyramid::Pyramid()
{
    startSigma = 0.5;
    zeroSigma = 1.6;
    layerCount = 4;
    octavesCount = 0;
}

ImageMatrix * Pyramid::getPyramidImage(ImageMatrix * imageMatrix, int octaveNumber){
    changeImageSize(imageMatrix);
    if(octaveNumber >= octavesCount)
        octaveNumber = octavesCount - 1;
    if(octaveNumber < 0)
        octaveNumber = 0;
    vector<Layer> octave = octavesVector[octaveNumber];
    return octave[0].imageMatrix;
}

vector<vector<Layer>> Pyramid::changeImageSize(ImageMatrix * imageMatrix){
    octavesCount = accountOctaves(imageMatrix->getWidth(), imageMatrix->getHeight());
    double k = pow(2, 1.0/layerCount);
    Convolution * convolution = new Convolution();

    for(int i = 0; i < octavesCount; i++){
        vector<Layer> layerVector;
        if(i == 0){
            Layer layer;
            layer.sigma = zeroSigma;
            layer.imageMatrix = convolution->gaus(imageMatrix, accountDeltaSigma(startSigma, zeroSigma));
            layerVector.push_back(layer);
        }
        else{
            vector<Layer> octave = octavesVector[i - 1];
            Layer layer;
            layer.sigma = (octave[layerCount - 1].sigma)/2;
            layer.imageMatrix = (octave[layerCount - 1].imageMatrix)->reduceImage();
            layerVector.push_back(layer);
        }
        for(int j = 1; j < layerCount; j++){
            Layer layer;
            ImageMatrix * newImageMatrix = layerVector[j - 1].imageMatrix;
            double sigmaPrev = layerVector[j - 1].sigma;
            double sigmaForw = sigmaPrev * k;
            layer.imageMatrix = convolution->gaus(newImageMatrix, accountDeltaSigma(sigmaPrev, sigmaForw));
            layer.sigma = sigmaForw;
            layerVector.push_back(layer);
        }
        octavesVector.push_back(layerVector);
    }
    return octavesVector;
}

int Pyramid::accountOctaves(int width, int height){
    int minSide;
    if(width < height)
        minSide = width;
    else
        minSide = height;

    return minSide/150;
}

double Pyramid::accountDeltaSigma(double sigmaPrevious, double sigmaFollowing){
    return sqrt(pow(sigmaFollowing, 2) - pow(sigmaPrevious, 2));
}
