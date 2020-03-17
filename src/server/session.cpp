#include "session.h"
#include "config.h"
#include "openmvg.h"
#include "openmvs_strategy.h"
#include <glog/logging.h>
#include <boost/filesystem.hpp>
#include "reconstruction.h"

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
        if(!new_images.empty()){
            /*
             * TODO: Change this to do a new reconstruction every x seconds or after so many images are in the queue
            */
            if(reconstruction->HasReconstructedOnce()){
                    LOG(INFO) << "Localizing " << new_images.size() << " images in reconstruction " << this->_reconstruction_id;
                    reconstruction->SetupRelocalization();
                    /*for(std::string image_id : new_images){
                        reconstruction->AddImageToRelocalization(image_id);
                    }*/
                    if(reconstruction->Relocalize()){
                        if(reconstruction->IncrementalSFM()){
                            LOG(INFO) << "Relocalized and ran SFM. Starting MVS for " << this->_reconstruction_id;
                            //reconstruction->SetupMVS();
                            //reconstruction->MVS();
                        } else {
                            LOG(ERROR) << "Incremental SFM or MVS failed";
                        }
                    }else{
                        LOG(ERROR) << "Reconstruction Relocalization Failed";
                    }
            } else {
                unconstructed_images->insert(new_images.begin(), new_images.end());
                if(unconstructed_images->size() >= 2){
                    LOG(INFO) << "Attempting first reconstruction of " << this->_reconstruction_id << " with " << unconstructed_images->size() << " images";
                    if(reconstruction->Reconstruct()){
                        LOG(INFO) << "Sucessfully reconstructed for the first time: " << this->_reconstruction_id;
                        delete unconstructed_images;
                    }
                }
            }
        }
        using namespace std::chrono_literals;
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