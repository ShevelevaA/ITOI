#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <math.h>
#include "imageMatrix.cpp"

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

void MainWindow::showImg(QPixmap image){
    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(image);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
}

void MainWindow::on_pushButton_clicked()
{
    imageMatrix22.loadImage();
    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(imageMatrix22.originalImage);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();

}

void MainWindow::on_pushButton_Sobel_clicked()
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(imageMatrix22.getSobelImg());
    ui->graphicsView_2->setScene(scene);
    ui->graphicsView_2->show();
}

void MainWindow::on_pushButton_Save_clicked()
{
    QPixmap pixmapImg = QPixmap::fromImage(resultImage);
    QFile file("img.jpg");
    file.open(QIODevice::WriteOnly);
    pixmapImg.save(&file, "JPG");

}

void MainWindow::on_pushButton_SobelX_clicked()
{

}

void MainWindow::on_pushButton_GrayImg_clicked()
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(imageMatrix22.getGrayImg());
    ui->graphicsView_2->setScene(scene);
    ui->graphicsView_2->show();
}

void MainWindow::on_pushButton_Gaus_clicked()
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(imageMatrix22.getGausImg());
    ui->graphicsView_2->setScene(scene);
    ui->graphicsView_2->show();
}
