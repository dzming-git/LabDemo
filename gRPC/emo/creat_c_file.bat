del emo_gRPC.grpc.pb.cc
del emo_gRPC.grpc.pb.h
del emo_gRPC.pb.cc
del emo_gRPC.pb.h
.\protoc.exe --grpc_out=. --plugin=protoc-gen-grpc=grpc_cpp_plugin.exe emo_gRPC.proto
.\protoc.exe --cpp_out=. emo_gRPC.proto