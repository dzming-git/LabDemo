del hand_gRPC.grpc.pb.cc
del hand_gRPC.grpc.pb.h
del hand_gRPC.pb.cc
del hand_gRPC.pb.h
..\protoc.exe --grpc_out=. --plugin=protoc-gen-grpc=..\grpc_cpp_plugin.exe hand_gRPC.proto
..\protoc.exe --cpp_out=. hand_gRPC.proto