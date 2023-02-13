#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include "ui_LabDemo.h"
#include "yolo_gRPC_client.h"
#include "emo_gRPC_client.h"
#include "hand_gRPC_client.h"

struct Client {
    YoloCommunicateClient* yoloClient = nullptr;
    EmoCommunicateClient* emoClient = nullptr;
    HandCommunicateClient* handClient = nullptr;

};

class LabDemo : public QMainWindow {
    Q_OBJECT

public:
    LabDemo(QWidget *parent = nullptr);
    ~LabDemo();

private:
    void setTimer();
    void getYoloResult();
    void getEmoResult();
    void getHandResult();

private slots:
    void on_yoloServerConnectBtn_clicked();
    void on_handServerConnectBtn_clicked();
    void on_emoServerConnectBtn_clicked();
    void on_webCamConfigBtn_clicked();
    void on_yoloStartBtn_clicked();
    void on_yoloStopBtn_clicked();
    void on_emoStartBtn_clicked();
    void on_emoStopBtn_clicked();
    void on_handStartBtn_clicked();
    void on_handStopBtn_clicked();
    void getResult();
    

private:
    Ui::LabDemoClass ui;
    Client client;
    QTimer* getResultTimer;
};
