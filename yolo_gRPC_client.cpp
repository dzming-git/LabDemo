#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "user32.lib")

#include "yolo_gRPC_client.h"

#define MAX_LEN 256*1024*1024

YoloResult::YoloResult(double x0, double y0, double x1, double y1, double conf, std::string label) :
	conf(conf), label(label) {
	bbox[0] = x0;
	bbox[1] = y0;
	bbox[2] = x1;
	bbox[3] = y1;
}

YoloConfig::YoloConfig(std::string source, double confThres, double iouThres, std::string device) :
	source(source), confThres(confThres), iouThres(iouThres), device(device) {
}


bool YoloCommunicateClient::connect(std::string ip, std::string port) {
	grpc::ChannelArguments arg;
	arg.SetMaxReceiveMessageSize(MAX_LEN);
	arg.SetMaxSendMessageSize(MAX_LEN);
	auto channel = grpc::CreateCustomChannel(ip + ":" + port, grpc::InsecureChannelCredentials(), arg);
	stub = yolo_gRPC::Communicate::NewStub(channel);
	yolo_gRPC::NullMsg nullMsg;
	grpc::ClientContext context;
	grpc::Status status = stub->check(&context, yolo_gRPC::NullMsg(), &nullMsg);
	return status.ok();
}

bool YoloCommunicateClient::setYoloConfig(YoloConfig config) {
	yolo_gRPC::NullMsg nullMsg;
	yolo_gRPC::YoloConfig configSend;
	grpc::ClientContext context;

	configSend.set_source(config.source);
	configSend.set_conf_thres(config.confThres);
	configSend.set_iou_thres(config.iouThres);
	configSend.set_device(config.device);
	grpc::Status status = stub->setYoloConf(&context, configSend, &nullMsg);
	return status.ok();
}

bool YoloCommunicateClient::stopYolo() {
	yolo_gRPC::NullMsg nullMsg;
	grpc::ClientContext context;
	grpc::Status status = stub->stopYolo(&context, yolo_gRPC::NullMsg(), &nullMsg);
	return status.ok();
}

bool YoloCommunicateClient::getYoloResults(std::vector<YoloResult>& results) {
	yolo_gRPC::NullMsg nullMsg;
	yolo_gRPC::ResultList resultList;
	grpc::ClientContext context;
	grpc::Status status = stub->getYoloResult(&context, nullMsg, &resultList);
	if (status.ok()) {
		int resultCount = resultList.re_result_size();
		for (int resultIndex = 0; resultIndex < resultCount; resultIndex++) {
			yolo_gRPC::ResultList::Result resultRpc = resultList.re_result(resultIndex);
			YoloResult result(resultRpc.x0(), resultRpc.y0(), resultRpc.x1(), resultRpc.y1(), resultRpc.conf(), resultRpc.label());
			results.push_back(result);
		}
		return true;
	}
	return false;
}

bool YoloCommunicateClient::getYoloImg(unsigned char*& pBuffer, int& imgW, int& imgH, int labelW, int labelH) {
	yolo_gRPC::LabelSize labelSize;
	yolo_gRPC::YoloImg imgMsg;
	grpc::ClientContext context;

	labelSize.set_w(labelW); 
	labelSize.set_h(labelH);
	grpc::Status status = stub->getYoloImg(&context, labelSize, &imgMsg);
	if (status.ok()) {
		imgW = imgMsg.w();
		imgH = imgMsg.h();
		int dataCount = imgW * imgH * 3;
		if(0 == dataCount) return false;
		pBuffer = new unsigned char[dataCount];
		memcpy(pBuffer, imgMsg.imgbuffer().c_str(), dataCount);
		return true;
	}
	return false;
}

