#include "session.h"
#include "config.h"
#include "openmvg.h"
#include "openmvs_strategy.h"
#include <glog/logging.h>
#include <boost/filesystem.hpp>
#include "reconstruction.h"
#include <chrono>

using namespace std::literals::chrono_literals;

Session::Session(const std::string& id, 
                 const std::string& reconstruction_id,
                 SessionImageQueueAdapter* image_queue_adapter) : _id(id),
                                                       _reconstruction_id(reconstruction_id),
                                                       _image_queue_adapter(image_queue_adapter){

}

void Session::Start(){
    this->_session_thread = new std::thread(&Session::_Run, this);
}

void Session::_Run(){
    this->_running = true;
    ReconstructionFetcher rf;
    Reconstruction * reconstruction = rf.Fetch(this->_reconstruction_id);
    std::set<std::string>* unconstructed_images = new std::set<std::string>();
    while(this->_running){
        std::set<std::string> new_images = this->_image_queue_adapter->Get(this->_id);
        if(this->_blocked_reconstruction){
            if(!reconstruction->IsRunningMVS()){
                LOG(INFO) << "Reconstruction unblocked";
                reconstruction->SetupMVS();
                reconstruction->MVS();
                this->_blocked_reconstruction = false;
            }
        }

        if(!new_images.empty()){
            /*
             * TODO: Change this to do a new reconstruction every x seconds or after so many images are in the queue
            */
            LOG(INFO) << "Adding " << new_images.size() << " images to reconstruction " << this->_reconstruction_id;
            for(std::string image_id : new_images){
                reconstruction->AddImage(image_id);
            }
            if(reconstruction->Reconstruct(new_images)){
                LOG(INFO) << "Relocalized and ran SFM. Starting MVS for " << this->_reconstruction_id;
                if(!reconstruction->IsRunningMVS()){
                    reconstruction->SetupMVS();
                    reconstruction->MVS();
                    this->_blocked_reconstruction = false;
                }else{
                    LOG(INFO) << "Reconstruction blocked";
                    this->_blocked_reconstruction = true;
                }
            } else {
                LOG(ERROR) << "Incremental SFM or MVS failed";
            }
            
        }
        std::this_thread::sleep_for(2s);
    }
}

void Session::Stop(){
    this->_running = false;
    this->_session_thread->join();
}

void Session::AddImage(const std::string& image_id){
    this->_image_queue_adapter->Put(this->_id, image_id);
}

Session::~Session(){
    if(this->_session_thread) delete this->_session_thread;
}