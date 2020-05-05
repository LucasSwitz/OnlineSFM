#pragma once

#include <unordered_map>
#include <grpcpp/grpcpp.h>

#define GRPC_PROVIDER GRPCServiceProvider::Instance()

class GRPCServiceProvider {
    typedef typename std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<grpc::Channel>>> ServiceHostMap;

    public:
        static std::shared_ptr<GRPCServiceProvider> Instance(){
            if(!GRPCServiceProvider::_instance){
                _instance = std::make_shared<GRPCServiceProvider>();
            }
            return _instance;
        }
        
        template<typename T> 
        std::unique_ptr<typename T::Stub> Get(const std::string& address){
            const std::string service_name = T::service_full_name();
            if(this->_services.find(service_name) == this->_services.end()){
                this->_services[service_name] = std::unordered_map<std::string, std::shared_ptr<grpc::Channel>>();
            }
            if(this->_services[service_name].find(address)  == this->_services[address].end()){
                this->_services[service_name][address] = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
            }
            return T::NewStub(this->_services[service_name][address]);
        }
    private:
        static std::shared_ptr<GRPCServiceProvider> _instance;
        ServiceHostMap _services; 
};