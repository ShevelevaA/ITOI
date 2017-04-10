#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <math.h>
#include "imageMatrix.cpp"
#include "convmatrix.h"
//#include "pyramid.h"
#include "pyramid.cpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QPixmap MainWindow::loadImage()
{
    QString imgName = QFileDialog::getOpenFileName(0, "Open dialog", "", "*.jpg *.png");
    if (!imgName.isEmpty())
    {
        QPixmap qPixmap;
        if(!qPixmap.load(imgName)){
            return NULL;
        }
        imageMatrix = new ImageMatrix(qPixmap);
        return qPixmap;
    }
    return NULL;
}

void MainWindow::showImage(QPixmap image){
    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(image);
    ui->graphicsView_2->setScene(scene);
    ui->graphicsView_2->show();
}

QPixmap MainWindow::getSobel(){
   Convolution * edit = new Convolution();
   ImageMatrix * matrixSobel = edit->sobel(imageMatrix);
   matrixSobel->ratingMatrix();
   return matrixSobel->createImgFromMatrix();
}

QPixmap MainWindow::getGaus(){
   Convolution * edit = new Convolution();
   ImageMatrix * matrixGaus = edit->gaus(imageMatrix,4);
   matrixGaus->ratingMatrix();
   return matrixGaus->createImgFromMatrix();
}

void MainWindow::on_pushButton_clicked()
{
    QPixmap qPixmap = loadImage();
    if( !qPixmap.isNull() ){
        QGraphicsScene *scene = new QGraphicsScene;
        scene->addPixmap(qPixmap);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->show();
    }
    else
        ui->label_2->setText("Loading error!");
}

void MainWindow::on_pushButton_Sobel_clicked()
{
    showImage(getSobel());
}

void MainWindow::on_pushButton_Save_clicked()
{
    QPixmap pixmapImg = QPixmap::fromImage(resultImage);
    QFile file("img111.jpg");
    file.open(QIODevice::WriteOnly);
    pixmapImg.save(&file, "JPG");

}

void MainWindow::on_pushButton_SobelX_clicked()
{
}

void MainWindow::on_pushButton_GrayImg_clicked()
{
    showImage(imageMatrix->createImgFromMatrix());
}

void MainWindow::on_pushButton_Gaus_clicked()
{
    showImage(getGaus());
}

void MainWindow::on_pushButton_Pyramid_clicked()
{
    Pyramid * pyramid = new Pyramid();
    ImageMatrix * pyrMatrix = pyramid->getPyramidImage(imageMatrix);
    //pyrMatrix->ratingMatrix();
    showImage(pyrMatrix->createImgFromMatrix());
}
