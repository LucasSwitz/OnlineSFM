// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: storage.proto
#ifndef GRPC_storage_2eproto__INCLUDED
#define GRPC_storage_2eproto__INCLUDED

#include "storage.pb.h"

#include <functional>
#include <grpc/impl/codegen/port_platform.h>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

class Storage final {
 public:
  static constexpr char const* service_full_name() {
    return "Storage";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    std::unique_ptr< ::grpc::ClientWriterInterface< ::StoreItemRequest>> Store(::grpc::ClientContext* context, ::StoreItemResponse* response) {
      return std::unique_ptr< ::grpc::ClientWriterInterface< ::StoreItemRequest>>(StoreRaw(context, response));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::StoreItemRequest>> AsyncStore(::grpc::ClientContext* context, ::StoreItemResponse* response, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::StoreItemRequest>>(AsyncStoreRaw(context, response, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::StoreItemRequest>> PrepareAsyncStore(::grpc::ClientContext* context, ::StoreItemResponse* response, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::StoreItemRequest>>(PrepareAsyncStoreRaw(context, response, cq));
    }
    std::unique_ptr< ::grpc::ClientReaderInterface< ::GetItemResponse>> Get(::grpc::ClientContext* context, const ::GetItemRequest& request) {
      return std::unique_ptr< ::grpc::ClientReaderInterface< ::GetItemResponse>>(GetRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::GetItemResponse>> AsyncGet(::grpc::ClientContext* context, const ::GetItemRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::GetItemResponse>>(AsyncGetRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::GetItemResponse>> PrepareAsyncGet(::grpc::ClientContext* context, const ::GetItemRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::GetItemResponse>>(PrepareAsyncGetRaw(context, request, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void Store(::grpc::ClientContext* context, ::StoreItemResponse* response, ::grpc::ClientWriteReactor< ::StoreItemRequest>* reactor) = 0;
      #else
      virtual void Store(::grpc::ClientContext* context, ::StoreItemResponse* response, ::grpc::experimental::ClientWriteReactor< ::StoreItemRequest>* reactor) = 0;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void Get(::grpc::ClientContext* context, ::GetItemRequest* request, ::grpc::ClientReadReactor< ::GetItemResponse>* reactor) = 0;
      #else
      virtual void Get(::grpc::ClientContext* context, ::GetItemRequest* request, ::grpc::experimental::ClientReadReactor< ::GetItemResponse>* reactor) = 0;
      #endif
    };
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    typedef class experimental_async_interface async_interface;
    #endif
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    async_interface* async() { return experimental_async(); }
    #endif
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientWriterInterface< ::StoreItemRequest>* StoreRaw(::grpc::ClientContext* context, ::StoreItemResponse* response) = 0;
    virtual ::grpc::ClientAsyncWriterInterface< ::StoreItemRequest>* AsyncStoreRaw(::grpc::ClientContext* context, ::StoreItemResponse* response, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncWriterInterface< ::StoreItemRequest>* PrepareAsyncStoreRaw(::grpc::ClientContext* context, ::StoreItemResponse* response, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientReaderInterface< ::GetItemResponse>* GetRaw(::grpc::ClientContext* context, const ::GetItemRequest& request) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::GetItemResponse>* AsyncGetRaw(::grpc::ClientContext* context, const ::GetItemRequest& request, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::GetItemResponse>* PrepareAsyncGetRaw(::grpc::ClientContext* context, const ::GetItemRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    std::unique_ptr< ::grpc::ClientWriter< ::StoreItemRequest>> Store(::grpc::ClientContext* context, ::StoreItemResponse* response) {
      return std::unique_ptr< ::grpc::ClientWriter< ::StoreItemRequest>>(StoreRaw(context, response));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriter< ::StoreItemRequest>> AsyncStore(::grpc::ClientContext* context, ::StoreItemResponse* response, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncWriter< ::StoreItemRequest>>(AsyncStoreRaw(context, response, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriter< ::StoreItemRequest>> PrepareAsyncStore(::grpc::ClientContext* context, ::StoreItemResponse* response, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncWriter< ::StoreItemRequest>>(PrepareAsyncStoreRaw(context, response, cq));
    }
    std::unique_ptr< ::grpc::ClientReader< ::GetItemResponse>> Get(::grpc::ClientContext* context, const ::GetItemRequest& request) {
      return std::unique_ptr< ::grpc::ClientReader< ::GetItemResponse>>(GetRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::GetItemResponse>> AsyncGet(::grpc::ClientContext* context, const ::GetItemRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::GetItemResponse>>(AsyncGetRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::GetItemResponse>> PrepareAsyncGet(::grpc::ClientContext* context, const ::GetItemRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::GetItemResponse>>(PrepareAsyncGetRaw(context, request, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void Store(::grpc::ClientContext* context, ::StoreItemResponse* response, ::grpc::ClientWriteReactor< ::StoreItemRequest>* reactor) override;
      #else
      void Store(::grpc::ClientContext* context, ::StoreItemResponse* response, ::grpc::experimental::ClientWriteReactor< ::StoreItemRequest>* reactor) override;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void Get(::grpc::ClientContext* context, ::GetItemRequest* request, ::grpc::ClientReadReactor< ::GetItemResponse>* reactor) override;
      #else
      void Get(::grpc::ClientContext* context, ::GetItemRequest* request, ::grpc::experimental::ClientReadReactor< ::GetItemResponse>* reactor) override;
      #endif
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientWriter< ::StoreItemRequest>* StoreRaw(::grpc::ClientContext* context, ::StoreItemResponse* response) override;
    ::grpc::ClientAsyncWriter< ::StoreItemRequest>* AsyncStoreRaw(::grpc::ClientContext* context, ::StoreItemResponse* response, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncWriter< ::StoreItemRequest>* PrepareAsyncStoreRaw(::grpc::ClientContext* context, ::StoreItemResponse* response, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientReader< ::GetItemResponse>* GetRaw(::grpc::ClientContext* context, const ::GetItemRequest& request) override;
    ::grpc::ClientAsyncReader< ::GetItemResponse>* AsyncGetRaw(::grpc::ClientContext* context, const ::GetItemRequest& request, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncReader< ::GetItemResponse>* PrepareAsyncGetRaw(::grpc::ClientContext* context, const ::GetItemRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_Store_;
    const ::grpc::internal::RpcMethod rpcmethod_Get_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status Store(::grpc::ServerContext* context, ::grpc::ServerReader< ::StoreItemRequest>* reader, ::StoreItemResponse* response);
    virtual ::grpc::Status Get(::grpc::ServerContext* context, const ::GetItemRequest* request, ::grpc::ServerWriter< ::GetItemResponse>* writer);
  };
  template <class BaseClass>
  class WithAsyncMethod_Store : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Store() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_Store() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Store(::grpc::ServerContext* /*context*/, ::grpc::ServerReader< ::StoreItemRequest>* /*reader*/, ::StoreItemResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestStore(::grpc::ServerContext* context, ::grpc::ServerAsyncReader< ::StoreItemResponse, ::StoreItemRequest>* reader, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncClientStreaming(0, context, reader, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_Get : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Get() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_Get() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Get(::grpc::ServerContext* /*context*/, const ::GetItemRequest* /*request*/, ::grpc::ServerWriter< ::GetItemResponse>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGet(::grpc::ServerContext* context, ::GetItemRequest* request, ::grpc::ServerAsyncWriter< ::GetItemResponse>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncServerStreaming(1, context, request, writer, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_Store<WithAsyncMethod_Get<Service > > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_Store : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_Store() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(0,
          new ::grpc_impl::internal::CallbackClientStreamingHandler< ::StoreItemRequest, ::StoreItemResponse>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, ::StoreItemResponse* response) { return this->Store(context, response); }));
    }
    ~ExperimentalWithCallbackMethod_Store() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Store(::grpc::ServerContext* /*context*/, ::grpc::ServerReader< ::StoreItemRequest>* /*reader*/, ::StoreItemResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerReadReactor< ::StoreItemRequest>* Store(
      ::grpc::CallbackServerContext* /*context*/, ::StoreItemResponse* /*response*/)
    #else
    virtual ::grpc::experimental::ServerReadReactor< ::StoreItemRequest>* Store(
      ::grpc::experimental::CallbackServerContext* /*context*/, ::StoreItemResponse* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_Get : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_Get() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(1,
          new ::grpc_impl::internal::CallbackServerStreamingHandler< ::GetItemRequest, ::GetItemResponse>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::GetItemRequest* request) { return this->Get(context, request); }));
    }
    ~ExperimentalWithCallbackMethod_Get() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Get(::grpc::ServerContext* /*context*/, const ::GetItemRequest* /*request*/, ::grpc::ServerWriter< ::GetItemResponse>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerWriteReactor< ::GetItemResponse>* Get(
      ::grpc::CallbackServerContext* /*context*/, const ::GetItemRequest* /*request*/)
    #else
    virtual ::grpc::experimental::ServerWriteReactor< ::GetItemResponse>* Get(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::GetItemRequest* /*request*/)
    #endif
      { return nullptr; }
  };
  #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
  typedef ExperimentalWithCallbackMethod_Store<ExperimentalWithCallbackMethod_Get<Service > > CallbackService;
  #endif

  typedef ExperimentalWithCallbackMethod_Store<ExperimentalWithCallbackMethod_Get<Service > > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_Store : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Store() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_Store() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Store(::grpc::ServerContext* /*context*/, ::grpc::ServerReader< ::StoreItemRequest>* /*reader*/, ::StoreItemResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_Get : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Get() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_Get() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Get(::grpc::ServerContext* /*context*/, const ::GetItemRequest* /*request*/, ::grpc::ServerWriter< ::GetItemResponse>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_Store : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Store() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_Store() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Store(::grpc::ServerContext* /*context*/, ::grpc::ServerReader< ::StoreItemRequest>* /*reader*/, ::StoreItemResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestStore(::grpc::ServerContext* context, ::grpc::ServerAsyncReader< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* reader, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncClientStreaming(0, context, reader, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_Get : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Get() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_Get() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Get(::grpc::ServerContext* /*context*/, const ::GetItemRequest* /*request*/, ::grpc::ServerWriter< ::GetItemResponse>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGet(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncWriter< ::grpc::ByteBuffer>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncServerStreaming(1, context, request, writer, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_Store : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_Store() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(0,
          new ::grpc_impl::internal::CallbackClientStreamingHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, ::grpc::ByteBuffer* response) { return this->Store(context, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_Store() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Store(::grpc::ServerContext* /*context*/, ::grpc::ServerReader< ::StoreItemRequest>* /*reader*/, ::StoreItemResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerReadReactor< ::grpc::ByteBuffer>* Store(
      ::grpc::CallbackServerContext* /*context*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerReadReactor< ::grpc::ByteBuffer>* Store(
      ::grpc::experimental::CallbackServerContext* /*context*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_Get : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_Get() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(1,
          new ::grpc_impl::internal::CallbackServerStreamingHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const::grpc::ByteBuffer* request) { return this->Get(context, request); }));
    }
    ~ExperimentalWithRawCallbackMethod_Get() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Get(::grpc::ServerContext* /*context*/, const ::GetItemRequest* /*request*/, ::grpc::ServerWriter< ::GetItemResponse>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerWriteReactor< ::grpc::ByteBuffer>* Get(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/)
    #else
    virtual ::grpc::experimental::ServerWriteReactor< ::grpc::ByteBuffer>* Get(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/)
    #endif
      { return nullptr; }
  };
  typedef Service StreamedUnaryService;
  template <class BaseClass>
  class WithSplitStreamingMethod_Get : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithSplitStreamingMethod_Get() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::SplitServerStreamingHandler< ::GetItemRequest, ::GetItemResponse>(std::bind(&WithSplitStreamingMethod_Get<BaseClass>::StreamedGet, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithSplitStreamingMethod_Get() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Get(::grpc::ServerContext* /*context*/, const ::GetItemRequest* /*request*/, ::grpc::ServerWriter< ::GetItemResponse>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with split streamed
    virtual ::grpc::Status StreamedGet(::grpc::ServerContext* context, ::grpc::ServerSplitStreamer< ::GetItemRequest,::GetItemResponse>* server_split_streamer) = 0;
  };
  typedef WithSplitStreamingMethod_Get<Service > SplitStreamedService;
  typedef WithSplitStreamingMethod_Get<Service > StreamedService;
};


#endif  // GRPC_storage_2eproto__INCLUDED
