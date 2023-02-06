#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "user32.lib")

#include "emo_gRPC_client.h"

#define MAX_LEN 256*1024*1024

EmoResult::EmoResult(double x0, double y0, double x1, double y1, std::string label) :
	label(label) {
	bbox[0] = x0;
	bbox[1] = y0;
	bbox[2] = x1;
	bbox[3] = y1;
}

EmoConfig::EmoConfig() {
}


bool EmoCommunicateClient::connect(std::string ip, std::string port) {
	grpc::ChannelArguments arg;
	arg.SetMaxReceiveMessageSize(MAX_LEN);
	arg.SetMaxSendMessageSize(MAX_LEN);
	auto channel = grpc::CreateCustomChannel(ip + ":" + port, grpc::InsecureChannelCredentials(), arg);
	stub = emo_gRPC::Communicate::NewStub(channel);
	emo_gRPC::NullMsg nullMsg;
	grpc::ClientContext context;
	grpc::Status status = stub->check(&context, emo_gRPC::NullMsg(), &nullMsg);
	return status.ok();
}

bool EmoCommunicateClient::setEmoConfig(EmoConfig config) {
	emo_gRPC::NullMsg nullMsg;
	emo_gRPC::EmoConfig configSend;
	grpc::ClientContext context;

	grpc::Status status = stub->setEmoConf(&context, configSend, &nullMsg);
	return status.ok();
}

bool EmoCommunicateClient::stopEmo() {
	emo_gRPC::NullMsg nullMsg;
	grpc::ClientContext context;
	grpc::Status status = stub->stopEmo(&context, emo_gRPC::NullMsg(), &nullMsg);
	return status.ok();
}

bool EmoCommunicateClient::getEmoResults(std::vector<EmoResult>& results) {
	emo_gRPC::NullMsg nullMsg;
	emo_gRPC::ResultList resultList;
	grpc::ClientContext context;
	grpc::Status status = stub->getEmoResult(&context, nullMsg, &resultList);
	if (status.ok()) {
		int resultCount = resultList.re_result_size();
		for (int resultIndex = 0; resultIndex < resultCount; resultIndex++) {
			emo_gRPC::ResultList::Result resultRpc = resultList.re_result(resultIndex);
			EmoResult result(resultRpc.x0(), resultRpc.y0(), resultRpc.x1(), resultRpc.y1(), resultRpc.label());
			results.push_back(result);
		}
		return true;
	}
	return false;
}

bool EmoCommunicateClient::getEmoImg(unsigned char*& pBuffer, int& imgW, int& imgH, int labelW, int labelH) {
	emo_gRPC::LabelSize labelSize;
	emo_gRPC::EmoImg imgMsg;
	grpc::ClientContext context;

	labelSize.set_w(labelW);
	labelSize.set_h(labelH);
	grpc::Status status = stub->getEmoImg(&context, labelSize, &imgMsg);
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

