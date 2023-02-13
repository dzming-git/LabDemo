#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "user32.lib")

#include "hand_gRPC_client.h"

#define MAX_LEN 256*1024*1024

HandResult::HandResult(std::string label) :
	label(label) {
}

HandConfig::HandConfig() {
}


bool HandCommunicateClient::connect(std::string ip, std::string port) {
	grpc::ChannelArguments arg;
	arg.SetMaxReceiveMessageSize(MAX_LEN);
	arg.SetMaxSendMessageSize(MAX_LEN);
	auto channel = grpc::CreateCustomChannel(ip + ":" + port, grpc::InsecureChannelCredentials(), arg);
	stub = hand_gRPC::Communicate::NewStub(channel);
	hand_gRPC::NullMsg nullMsg;
	grpc::ClientContext context;
	grpc::Status status = stub->check(&context, hand_gRPC::NullMsg(), &nullMsg);
	return status.ok();
}

bool HandCommunicateClient::setHandConfig(HandConfig config) {
	hand_gRPC::NullMsg nullMsg;
	hand_gRPC::HandConfig configSend;
	grpc::ClientContext context;

	grpc::Status status = stub->setHandConf(&context, configSend, &nullMsg);
	return status.ok();
}

bool HandCommunicateClient::stopHand() {
	hand_gRPC::NullMsg nullMsg;
	grpc::ClientContext context;
	grpc::Status status = stub->stopHand(&context, hand_gRPC::NullMsg(), &nullMsg);
	return status.ok();
}

bool HandCommunicateClient::getHandResults(std::vector<HandResult>& results) {
	hand_gRPC::NullMsg nullMsg;
	hand_gRPC::ResultList resultList;
	grpc::ClientContext context;
	grpc::Status status = stub->getHandResult(&context, nullMsg, &resultList);
	if (status.ok()) {
		int resultCount = resultList.re_result_size();
		for (int resultIndex = 0; resultIndex < resultCount; resultIndex++) {
			hand_gRPC::ResultList::Result resultRpc = resultList.re_result(resultIndex);
			HandResult result(resultRpc.label());
			results.push_back(result);
		}
		return true;
	}
	return false;
}

bool HandCommunicateClient::getHandImg(unsigned char*& pBuffer, int& imgW, int& imgH, int labelW, int labelH) {
	hand_gRPC::LabelSize labelSize;
	hand_gRPC::HandImg imgMsg;
	grpc::ClientContext context;

	labelSize.set_w(labelW);
	labelSize.set_h(labelH);
	grpc::Status status = stub->getHandImg(&context, labelSize, &imgMsg);
	if (status.ok()) {
		imgW = imgMsg.w();
		imgH = imgMsg.h();
		int dataCount = imgW * imgH * 3;
		if (0 == dataCount) return false;
		pBuffer = new unsigned char[dataCount];
		memcpy(pBuffer, imgMsg.imgbuffer().c_str(), dataCount);
		return true;
	}
	return false;
}

