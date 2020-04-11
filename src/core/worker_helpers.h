#pragma once
#include "worker.grpc.pb.h"
#include <grpcpp/grpcpp.h>

std::unique_ptr<Worker::Stub> GetWorkerClient(const std::string& address);