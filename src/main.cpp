#include "ui_cvpro.h"
#include <iostream>
#include "opencv2/opencv.hpp"
#include "QApplication"
#include "QFileDialog"
#include "QPixmap"
#include "QImage"
#include "QMessageBox"

class MainWin : public Ui_Form, public QWidget{
    public:
    MainWin(QWidget *parent=0) : QWidget(parent), func(nullptr){
        this->setupUi(this);

        connect(this->loadBT, &QPushButton::clicked, this, &MainWin::on_loadBT_clicked);
        connect(this->grayBT, &QPushButton::clicked, this, &MainWin::on_grayBT_clicked);
        connect(this->binarizeBT, &QPushButton::clicked, this, &MainWin::on_binarizeBT_clicked);
        connect(this->erodeBT, &QPushButton::clicked, this, &MainWin::on_erodeBT_clicked);
        connect(this->dilateBT, &QPushButton::clicked, this, &MainWin::on_dilateBT_clicked);
        connect(this->Slider1, &QSlider::valueChanged, this, &MainWin::on_slider1_changed);
        connect(this->Slider2, &QSlider::valueChanged, this, &MainWin::on_slider2_changed);
        connect(this->openBT, &QPushButton::clicked, this, &MainWin::on_openBT_clicked);
        connect(this->closeBT, &QPushButton::clicked, this, &MainWin::on_closeBT_clicked);
        connect(this->saveBT, &QPushButton::clicked, this, &MainWin::on_saveBT_clicked);
    }

    void on_loadBT_clicked(){
        QString src_name = QFileDialog::getOpenFileName(this, "选择要操作的图片文件", "D:/Projects/inky_ws/src/cvpro/ui");
        src_ = cv::imread(src_name.toStdString());
        QPixmap pm;
        pm.load(src_name);
        this->img_src->setPixmap(pm);
    }

    void on_grayBT_clicked(){
        if(src_.empty()) return;
        cv::cvtColor(src_, des_, cv::COLOR_RGB2GRAY);
        QImage temp = grayMat2Qimage(des_);
        this->img_des->setPixmap(QPixmap::fromImage(temp));
    }

    void on_binarizeBT_clicked(){
        int thresh = this->Slider1->value();
        cv::threshold(des_, des_, thresh, 255, cv::THRESH_BINARY);
        this->img_des->setPixmap(QPixmap::fromImage(grayMat2Qimage(des_)));
    }

    void on_erodeBT_clicked(){
        func = &MainWin::erode_des;
        int k = this->Slider2->value();
        (this->*func)(k);
    }

    void on_dilateBT_clicked(){
        func = &MainWin::dilate_des;
        int k = this->Slider2->value();
        (this->*func)(k);
    }

    void on_openBT_clicked(){
        func = &MainWin::open_des;
        int k = this->Slider2->value();
        (this->*func)(k);
    }

    void on_closeBT_clicked(){
        func = &MainWin::close_des;
        int k = this->Slider2->value();
        (this->*func)(k);
    }

    void on_slider1_changed(int value){
        cv::cvtColor(src_, des_, cv::COLOR_RGB2GRAY);
        cv::threshold(des_, des_, value, 255, cv::THRESH_BINARY);
        this->img_des->setPixmap(QPixmap::fromImage(grayMat2Qimage(des_)));
    }

    void on_slider2_changed(int value){
        if(nullptr != func)
            (this->*func)(value);
    }

    void on_saveBT_clicked(){
        des_.copyTo(bin_);
        this->Slider1->setValue(1);
        this->Slider2->setValue(3);
        this->img_des->setPixmap(QPixmap::fromImage(grayMat2Qimage(bin_)));
    }

    void erode_des(int k){
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(k, k));
        // cv::Mat tmp;
        cv::erode(bin_, des_, kernel);
        this->img_des->setPixmap(QPixmap::fromImage(grayMat2Qimage(des_)));
    }

    void dilate_des(int k){
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(k, k));
        // cv::Mat tmp;
        cv::dilate(bin_, des_, kernel);
        this->img_des->setPixmap(QPixmap::fromImage(grayMat2Qimage(des_)));
    }

    void open_des(int k){
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(k, k));
        // cv::Mat des_;
        cv::morphologyEx(bin_, des_, cv::MORPH_OPEN, kernel);
        this->img_des->setPixmap(QPixmap::fromImage(grayMat2Qimage(des_)));
    }

    void close_des(int k){
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(k, k));
        // cv::Mat tmp;
        cv::morphologyEx(bin_, des_, cv::MORPH_CLOSE, kernel);
        this->img_des->setPixmap(QPixmap::fromImage(grayMat2Qimage(des_)));
    }

    QImage grayMat2Qimage(const cv::Mat& mat){
        QImage temp = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.cols*mat.channels(), QImage::Format_Indexed8);
        QVector<QRgb> colorTable;
		for (int k = 0; k<256; ++k)
		{
			colorTable.push_back(qRgb(k, k, k));
		}
		temp.setColorTable(colorTable);
        return temp;
    }
    
    private:
        cv::Mat src_;
        cv::Mat des_;
        cv::Mat bin_;
        void (MainWin::*func)(int);

};

int main(int argc, char** argv){
    std::cout << "hello world" << std::endl;
    QApplication app(argc, argv);
    MainWin win;
    win.show();

    return app.exec();
}