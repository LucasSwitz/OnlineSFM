#include "remote_reconstruction_agent.h"
#include <grpcpp/grpcpp.h>
#include "worker.pb.h"
#include <glog/logging.h>
#include "config.h"

using namespace grpc;

RemoteReconstructionAgent::RemoteReconstructionAgent(const std::string& reconstruction_id,
                                                     const std::string& address) : _client(grpc::CreateChannel(address, 
                                                                                           grpc::InsecureChannelCredentials())),
                                                                                   _reconstruction_id(reconstruction_id){

}

bool RemoteReconstructionAgent::AddImage(const std::string& image_id){
    ClientContext ctx;
    WorkerAddImageRequest req;
    WorkerAddImageResponse resp;
    req.set_reconstruction_id(this->_reconstruction_id);
    req.set_image_id(image_id);
    auto status = this->_client.AddImage(&ctx, req, &resp);
    return status.ok();
}

bool RemoteReconstructionAgent::ComputeFeatures(const std::set<std::string>& image_ids){
    CompletionQueue cq;
    std::vector<std::string> image_ids_vec(image_ids.begin(), image_ids.end());
    std::vector<Status> statuses(image_ids.size());
    std::vector<WorkerComputeFeaturesResponse> responses(image_ids.size());
    for(int i = 0; i < image_ids_vec.size(); i++){
        std::string image_id = image_ids_vec[i];
        ClientContext ctx;
        WorkerComputeFeaturesRequest req;
        req.set_reconstruction_id(this->_reconstruction_id);
        req.set_image_id(image_id);
        std::unique_ptr<ClientAsyncResponseReader<WorkerComputeFeaturesResponse> > rpc(
            this->_client.AsyncComputeFeatures(&ctx, req, &cq));
        rpc->Finish(&responses[i], &statuses[i], (void*)(i+1));
    }
    bool all_ok = true;
    for(int i = 0; i < image_ids.size(); i++){
        void* got_tag;
        bool ok = false;
        cq.Next(&got_tag, &ok);
        if (!ok) {
            all_ok = false;
        }
    }
    return all_ok;
}

bool RemoteReconstructionAgent::ComputeMatches(const  std::set<std::string>& image_ids){
    CompletionQueue cq;
    std::vector<std::string> image_ids_vec(image_ids.begin(), image_ids.end());
    std::vector<Status> statuses(image_ids.size());
    std::vector<WorkerComputeMatchesResponse> responses(image_ids.size());
    for(int i = 0; i < image_ids_vec.size(); i++){
        std::string image_id = image_ids_vec[i];
        ClientContext ctx;
        WorkerComputeMatchesRequest req;
        req.set_reconstruction_id(this->_reconstruction_id);
        req.set_image_id(image_id);
        std::unique_ptr<ClientAsyncResponseReader<WorkerComputeMatchesResponse> > rpc(
            this->_client.AsyncComputeMatches(&ctx, req, &cq));
        rpc->Finish(&responses[i], &statuses[i], (void*)(i+1));
    }
    bool all_ok = true;
    for(int i = 0; i < image_ids.size(); i++){
        void* got_tag;
        bool ok = false;
        cq.Next(&got_tag, &ok);
        if (!ok) {
            all_ok = false;
            break;
        }
    }
    return all_ok;
}

bool RemoteReconstructionAgent::IncrementalSFM(){
    ClientContext ctx;
    WorkerIncrementalSFMRequest req;
    WorkerIncrementalSFMResponse resp;
    req.set_reconstruction_id(this->_reconstruction_id);
    auto status = this->_client.IncrementalSFM(&ctx, req, &resp);
    return status.ok();
}

bool RemoteReconstructionAgent::ComputeStructure(){
    ClientContext ctx;
    WorkerComputeStructureRequest req;
    WorkerComputeStructureResponse resp;
    req.set_reconstruction_id(this->_reconstruction_id);
    auto status = this->_client.ComputeStructure(&ctx, req, &resp);
    return status.ok();
}

bool RemoteReconstructionAgent::MVS(){
    ClientContext ctx;
    WorkerMVSRequest req;
    WorkerMVSResponse resp;
    req.set_reconstruction_id(this->_reconstruction_id);
    auto status = this->_client.MVS(&ctx, req, &resp);
    return status.ok();
}