del yolo_gRPC.grpc.pb.cc
del yolo_gRPC.grpc.pb.h
del yolo_gRPC.pb.cc
del yolo_gRPC.pb.h
.\protoc.exe --grpc_out=. --plugin=protoc-gen-grpc=grpc_cpp_plugin.exe yolo_gRPC.proto
.\protoc.exe --cpp_out=. yolo_gRPC.proto