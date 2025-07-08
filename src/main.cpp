#include <QApplication>
#include <QMetaType>
#include <opencv2/opencv.hpp>
#include "UI/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 注册cv::Mat类型以便在信号槽中使用
    qRegisterMetaType<cv::Mat>("cv::Mat");

    MainWindow w;
    w.show();

    return app.exec();
}
