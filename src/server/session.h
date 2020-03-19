#pragma once
#include <string>
#include <thread>
#include "image_storage.h"
#include "session_image_queue_adapter.h"

class Session {
    public:
        Session(const std::string& session_id,
                const std::string& reconstruction_id,
                SessionImageQueueAdapter* image_queue);
        ~Session();
        void Start();
        void Stop();
        void AddImage(const std::string& image_id);
    private:
        std::string _id;
        std::string _reconstruction_id;
        std::thread* _session_thread = nullptr;
        SessionImageQueueAdapter* _image_queue_adapter = nullptr;
        bool _running = false;
        bool _blocked_reconstruction = false;
        void _Run();
};