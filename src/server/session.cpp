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
                 SFMBacklogCounter* session_backlog) : _id(id),
                                                       _reconstruction_id(reconstruction_id),
                                                       _session_backlog(session_backlog){

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
        if(this->_blocked_reconstruction){
            if(!reconstruction->IsRunningMVS()){
                LOG(INFO) << "Reconstruction unblocked";
                //reconstruction->SetupMVS();
                //reconstruction->MVS();
                this->_blocked_reconstruction = false;
            }
        }

        if(this->_session_backlog->CheckCounter(this->_reconstruction_id) > 0){
            LOG(INFO) << "New matches avilable. Running SFM.... " << this->_reconstruction_id;
            if(reconstruction->SparseReconstruct()){
                LOG(INFO) << "Relocalized and ran SFM. Starting MVS for " << this->_reconstruction_id;
                if(!reconstruction->IsRunningMVS()){
                    //reconstruction->SetupMVS();
                    //reconstruction->MVS();
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

Session::~Session(){
    if(this->_session_thread) delete this->_session_thread;
}