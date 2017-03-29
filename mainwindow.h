#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imageMatrix.h"
#include <QPixmap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    ImageMatrix * imageMatrix;
    QImage resultImage;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QPixmap loadImage();
    QPixmap getSobel();
    QPixmap getGaus();
    void showImage(QPixmap image);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_Sobel_clicked();

    void on_pushButton_Save_clicked();

    void on_pushButton_SobelX_clicked();

    void on_pushButton_GrayImg_clicked();

    void on_pushButton_Gaus_clicked();

private:
    Ui::MainWindow *ui;
    ImageMatrix imageMatrix22;
};

#endif // MAINWINDOW_H
