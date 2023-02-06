#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "grpc++/grpc++.h"
#include "grpcpp/impl/codegen/channel_interface.h"
#include "gRPC/yolo/yolo_gRPC.grpc.pb.h"
#include "gRPC/yolo/yolo_gRPC.pb.h"

class YoloResult {
public:
	YoloResult(double x0, double y0, double x1, double y1, double conf, std::string laebl);
	double bbox[4];
	double conf;
	std::string label;
};

class YoloConfig {
public:
	YoloConfig(std::string source, double confThres, double iouThres, std::string device);
	std::string source;
	double confThres;
	double iouThres;
	std::string device;
};

class YoloCommunicateClient {
public:
	bool connect(std::string ip, std::string port);
	bool setYoloConfig(YoloConfig config);
	bool stopYolo();
	bool getYoloResults(std::vector<YoloResult>& results);
	bool getYoloImg(unsigned char*& pBuffer, int& imgW, int& imgH, int labelW, int labelH);
private:
	std::unique_ptr<yolo_gRPC::Communicate::Stub> stub;
};