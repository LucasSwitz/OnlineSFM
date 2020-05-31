#pragma once
#include <string>
#include <thread>
#include "image_storage.h"
#include "sfm_backlog_counter.h"

class Session
{
public:
    Session(const std::string &session_id,
            const std::string &reconstruction_id,
            SFMBacklogCounter *session_backlog);
    ~Session();
    void Start();
    void Stop();

private:
    std::string _id;
    std::string _reconstruction_id;
    std::thread *_session_thread = nullptr;
    SFMBacklogCounter *_session_backlog = nullptr;
    bool _running = false;
    bool _blocked_reconstruction = false;
    void _Run();
};