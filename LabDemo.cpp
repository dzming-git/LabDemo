#include "LabDemo.h"
#include <iostream>
#include <vector>
#include <QRegExpValidator>
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
//using namespace cv;

LabDemo::LabDemo(QWidget *parent): QMainWindow(parent) {
    ui.setupUi(this);
    ui.yoloConfLe->setValidator(new QRegExpValidator(QRegExp("^(([0-9]+\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\.[0-9]+)|([0-9]*[1-9][0-9]*))$")));
    ui.yoloIouLe->setValidator(new QRegExpValidator(QRegExp("^(([0-9]+\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\.[0-9]+)|([0-9]*[1-9][0-9]*))$")));
    setTimer();
}

LabDemo::~LabDemo(){
}

void LabDemo::setTimer() {
    getResultTimer = new QTimer();
    getResultTimer->start(1000);
    connect(getResultTimer, SIGNAL(timeout()), this, SLOT(getResult()));
}

void LabDemo::getYoloResult() {
    // 文本
    if (nullptr == client.yoloClient) return;
    vector<YoloResult> results;
    if (client.yoloClient->getYoloResults(results)) {
        QString content = "YOLO\n";
        for (auto it = results.begin(); it < results.end(); it++) {
            QString temp = QString("label:%1\nbbox:\n x0:%2\n y0:%3\n x1:%4\n y1:%5\nconf:%5\n\n")
                .arg(QString::fromStdString(it->label))
                .arg(it->bbox[0])
                .arg(it->bbox[1])
                .arg(it->bbox[2])
                .arg(it->bbox[3])
                .arg(it->conf);
            content.append(temp);
        }
        ui.resultShowTe->textCursor().insertText(content);
    }
    // 图像
    unsigned char* pBuffer = nullptr;
    int w, h;
    QSize labelSize = ui.yoloImgLb->size();
    if (client.yoloClient->getYoloImg(pBuffer, w, h, labelSize.width(), labelSize.height())) {
        //Mat img = Mat(h, w, CV_8UC3, pBuffer);
        //cvtColor(img, img, cv::COLOR_BGR2RGB);
        //QImage qImg = QImage((uchar*)img.data, w, h, img.step, QImage::Format_RGB888);
        //ui.yoloImgLb->setPixmap(QPixmap::fromImage(qImg));

        // 避开opencv的写法
        QImage qImg = QImage(pBuffer, w, h, 3 * w, QImage::Format_RGB888).rgbSwapped();
        ui.yoloImgLb->setPixmap(QPixmap::fromImage(qImg));

        delete[] pBuffer;
    }
}

void LabDemo::on_yoloServerConnectBtn_clicked() {
    string yoloServerIp = ui.yoloIpLe->text().toStdString();
    string yoloServerPort = ui.yoloPortLe->text().toStdString();
    if (nullptr == client.yoloClient) {
        client.yoloClient = new YoloCommunicateClient;
    }
    if (false == client.yoloClient->connect(yoloServerIp, yoloServerPort)) {
        delete client.yoloClient;
        client.yoloClient = nullptr;
    }
    else {
        ui.yoloConnectStatusLb->setText(QStringLiteral("已连接"));
    }
}

void LabDemo::on_handServerConnectBtn_clicked() {

}

void LabDemo::on_emoServerConnectBtn_clicked() {

}

void LabDemo::on_webCamConfigBtn_clicked() {
    QString webCamIp = ui.webCamIpLe->text();
    QString webCamUser = ui.webCamUserLe->text();
    QString webCamPwd = ui.webCamPwdLe->text();
    QString source = QString("rtsp://%1:%2@%3:554").arg(webCamUser, webCamPwd, webCamIp);
    ui.yoloSourceLe->setText(source);
    ui.tabWidget->setCurrentIndex(2);
}

void LabDemo::on_yoloStartBtn_clicked() {
    if (nullptr == client.yoloClient) return;
    string source = ui.yoloSourceLe->text().toStdString();
    float confThres = atof(ui.yoloConfLe->text().toStdString().data());
    float iouThres = atof(ui.yoloIouLe->text().toStdString().data());
    string device = ui.yoloDeviceLe->text().toStdString();
    YoloConfig yoloConfig(source, confThres, iouThres, device);
    client.yoloClient->setYoloConfig(yoloConfig);
}

void LabDemo::on_yoloStopBtn_clicked() {
    client.yoloClient->stopYolo();
}

void LabDemo::getResult() {
    ui.resultShowTe->clear();
    getYoloResult();
}
