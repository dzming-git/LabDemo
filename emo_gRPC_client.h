#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "grpc++/grpc++.h"
#include "grpcpp/impl/codegen/channel_interface.h"
#include "gRPC/emo/emo_gRPC.grpc.pb.h"
#include "gRPC/emo/emo_gRPC.pb.h"

class EmoResult {
public:
	EmoResult(double x0, double y0, double x1, double y1, std::string laebl);
	double bbox[4];
	std::string label;
};

class EmoConfig {
public:
	EmoConfig();
};

class EmoCommunicateClient {
public:
	bool connect(std::string ip, std::string port);
	bool setEmoConfig(EmoConfig config);
	bool stopEmo();
	bool getEmoResults(std::vector<EmoResult>& results);
	bool getEmoImg(unsigned char*& pBuffer, int& imgW, int& imgH, int labelW, int labelH);
private:
	std::unique_ptr<emo_gRPC::Communicate::Stub> stub;
};