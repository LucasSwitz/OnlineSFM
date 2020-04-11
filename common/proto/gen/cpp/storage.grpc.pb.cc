// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: storage.proto

#include "storage.pb.h"
#include "storage.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>

static const char* Storage_method_names[] = {
  "/Storage/Store",
  "/Storage/Get",
};

std::unique_ptr< Storage::Stub> Storage::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Storage::Stub> stub(new Storage::Stub(channel));
  return stub;
}

Storage::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_Store_(Storage_method_names[0], ::grpc::internal::RpcMethod::CLIENT_STREAMING, channel)
  , rpcmethod_Get_(Storage_method_names[1], ::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  {}

::grpc::ClientWriter< ::StoreItemRequest>* Storage::Stub::StoreRaw(::grpc::ClientContext* context, ::StoreItemResponse* response) {
  return ::grpc_impl::internal::ClientWriterFactory< ::StoreItemRequest>::Create(channel_.get(), rpcmethod_Store_, context, response);
}

void Storage::Stub::experimental_async::Store(::grpc::ClientContext* context, ::StoreItemResponse* response, ::grpc::experimental::ClientWriteReactor< ::StoreItemRequest>* reactor) {
  ::grpc_impl::internal::ClientCallbackWriterFactory< ::StoreItemRequest>::Create(stub_->channel_.get(), stub_->rpcmethod_Store_, context, response, reactor);
}

::grpc::ClientAsyncWriter< ::StoreItemRequest>* Storage::Stub::AsyncStoreRaw(::grpc::ClientContext* context, ::StoreItemResponse* response, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncWriterFactory< ::StoreItemRequest>::Create(channel_.get(), cq, rpcmethod_Store_, context, response, true, tag);
}

::grpc::ClientAsyncWriter< ::StoreItemRequest>* Storage::Stub::PrepareAsyncStoreRaw(::grpc::ClientContext* context, ::StoreItemResponse* response, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncWriterFactory< ::StoreItemRequest>::Create(channel_.get(), cq, rpcmethod_Store_, context, response, false, nullptr);
}

::grpc::ClientReader< ::GetItemResponse>* Storage::Stub::GetRaw(::grpc::ClientContext* context, const ::GetItemRequest& request) {
  return ::grpc_impl::internal::ClientReaderFactory< ::GetItemResponse>::Create(channel_.get(), rpcmethod_Get_, context, request);
}

void Storage::Stub::experimental_async::Get(::grpc::ClientContext* context, ::GetItemRequest* request, ::grpc::experimental::ClientReadReactor< ::GetItemResponse>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderFactory< ::GetItemResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_Get_, context, request, reactor);
}

::grpc::ClientAsyncReader< ::GetItemResponse>* Storage::Stub::AsyncGetRaw(::grpc::ClientContext* context, const ::GetItemRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderFactory< ::GetItemResponse>::Create(channel_.get(), cq, rpcmethod_Get_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::GetItemResponse>* Storage::Stub::PrepareAsyncGetRaw(::grpc::ClientContext* context, const ::GetItemRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderFactory< ::GetItemResponse>::Create(channel_.get(), cq, rpcmethod_Get_, context, request, false, nullptr);
}

Storage::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Storage_method_names[0],
      ::grpc::internal::RpcMethod::CLIENT_STREAMING,
      new ::grpc::internal::ClientStreamingHandler< Storage::Service, ::StoreItemRequest, ::StoreItemResponse>(
          std::mem_fn(&Storage::Service::Store), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Storage_method_names[1],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< Storage::Service, ::GetItemRequest, ::GetItemResponse>(
          std::mem_fn(&Storage::Service::Get), this)));
}

Storage::Service::~Service() {
}

::grpc::Status Storage::Service::Store(::grpc::ServerContext* context, ::grpc::ServerReader< ::StoreItemRequest>* reader, ::StoreItemResponse* response) {
  (void) context;
  (void) reader;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Storage::Service::Get(::grpc::ServerContext* context, const ::GetItemRequest* request, ::grpc::ServerWriter< ::GetItemResponse>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

