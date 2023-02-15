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
    ui.yoloResultTe->clear();
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
        ui.yoloResultTe->textCursor().insertText(content);
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
        ui.emoImgLb->setAlignment(Qt::AlignCenter);
        delete[] pBuffer;
    }
}

void LabDemo::getEmoResult() {
    // 文本
    if (nullptr == client.emoClient) return;
    ui.emoResultTe->clear();
    vector<EmoResult> results;
    if (client.emoClient->getEmoResults(results)) {
        QString content = "Emotion\n";
        for (auto it = results.begin(); it < results.end(); it++) {
            QString temp = QString("label:%1\nbbox:\n x0:%2\n y0:%3\n x1:%4\n y1:%5\n\n")
                .arg(QString::fromStdString(it->label))
                .arg(it->bbox[0])
                .arg(it->bbox[1])
                .arg(it->bbox[2])
                .arg(it->bbox[3]);
            content.append(temp);
        }
        ui.emoResultTe->textCursor().insertText(content);
    }
    // 图像
    unsigned char* pBuffer = nullptr;
    int w, h;
    QSize labelSize = ui.emoImgLb->size();
    if (client.emoClient->getEmoImg(pBuffer, w, h, labelSize.width(), labelSize.height())) {
        QImage qImg = QImage(pBuffer, w, h, 3 * w, QImage::Format_RGB888).rgbSwapped();
        ui.emoImgLb->setPixmap(QPixmap::fromImage(qImg));
        ui.emoImgLb->setAlignment(Qt::AlignCenter);
        delete[] pBuffer;
    }
}

void LabDemo::getHandResult() {
    // 文本
    if (nullptr == client.handClient) return;
    ui.handResultTe->clear();
    vector<HandResult> results;
    if (client.handClient->getHandResults(results)) {
        QString content = "Hand\n";
        for (auto it = results.begin(); it < results.end(); it++) {
            QString temp = QString("label:%1\n\n")
                .arg(QString::fromStdString(it->label));
            content.append(temp);
        }
        ui.handResultTe->textCursor().insertText(content);
    }
    // 图像
    unsigned char* pBuffer = nullptr;
    int w, h;
    QSize labelSize = ui.handImgLb->size();
    if (client.handClient->getHandImg(pBuffer, w, h, labelSize.width(), labelSize.height())) {
        QImage qImg = QImage(pBuffer, w, h, 3 * w, QImage::Format_RGB888).rgbSwapped();
        ui.handImgLb->setPixmap(QPixmap::fromImage(qImg));
        ui.handImgLb->setAlignment(Qt::AlignCenter);
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
    string handServerIp = ui.handIpLe->text().toStdString();
    string handServerPort = ui.handPortLe->text().toStdString();
    if (nullptr == client.handClient) {
        client.handClient = new HandCommunicateClient;
    }
    if (false == client.handClient->connect(handServerIp, handServerPort)) {
        delete client.handClient;
        client.handClient = nullptr;
    }
    else {
        ui.handConnectStatusLb->setText(QStringLiteral("已连接"));
    }
}

void LabDemo::on_emoServerConnectBtn_clicked() {
    string emoServerIp = ui.emoIpLe->text().toStdString();
    string emoServerPort = ui.emoPortLe->text().toStdString();
    if (nullptr == client.emoClient) {
        client.emoClient = new EmoCommunicateClient;
    }
    if (false == client.emoClient->connect(emoServerIp, emoServerPort)) {
        delete client.emoClient;
        client.emoClient = nullptr;
    }
    else {
        ui.emoConnectStatusLb->setText(QStringLiteral("已连接"));
    }
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

void LabDemo::on_emoStartBtn_clicked() {
    EmoConfig emoConfig;
    client.emoClient->setEmoConfig(emoConfig);
}

void LabDemo::on_emoStopBtn_clicked() {
    client.emoClient->stopEmo();
}

void LabDemo::on_handStartBtn_clicked() {
    HandConfig handConfig;
    client.handClient->setHandConfig(handConfig);
}

void LabDemo::on_handStopBtn_clicked() {
    client.handClient->stopHand();
}

void LabDemo::getResult() {
    getYoloResult();
    getEmoResult();
    getHandResult();
}
