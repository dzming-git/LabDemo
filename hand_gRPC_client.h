#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "grpc++/grpc++.h"
#include "grpcpp/impl/codegen/channel_interface.h"
#include "gRPC/hand/hand_gRPC.grpc.pb.h"
#include "gRPC/hand/hand_gRPC.pb.h"

class HandResult {
public:
	HandResult(std::string laebl);
	std::string label;
};

class HandConfig {
public:
	HandConfig();
};

class HandCommunicateClient {
public:
	bool connect(std::string ip, std::string port);
	bool setHandConfig(HandConfig config);
	bool stopHand();
	bool getHandResults(std::vector<HandResult>& results);
	bool getHandImg(unsigned char*& pBuffer, int& imgW, int& imgH, int labelW, int labelH);
private:
	std::unique_ptr<hand_gRPC::Communicate::Stub> stub;
};