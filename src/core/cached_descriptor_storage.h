#pragma once

#include "descriptor_storage.h"
#include <mutex>
#include <thread>
#include <condition_variable>

template <typename T>
class CachedDescriptorStorage : public DescriptorStorage<T>
{
public:
    CachedDescriptorStorage(std::shared_ptr<DescriptorStorage<T>> delegate) : _delegate(delegate)
    {
    }
    void Store(const std::string &reconstruction_id, const std::string &image_id, SIFT_Descriptor_count_map &descriptors)
    {
        this->_delegate->Store(reconstruction_id, image_id, descriptors);
        CachedDescriptorStorage::count++;
    }

    std::unordered_map<T, unsigned int> GetAllDescriptors(const std::string &reconstruction_id, const std::string &image_id)
    {
        return this->_delegate->GetAllDescriptors(reconstruction_id, image_id);
    }

    void DeleteAllDescriptors(const std::string &image_id)
    {
        this->_delegate->DeleteAllDescriptors(image_id);
    }

    std::set<std::string> GetAllWithCommonDescriptors(const std::string &reconstruction_id,
                                                      const std::string &image_id,
                                                      int page_size = 0,
                                                      std::string *page_start = nullptr)
    {
        return this->_delegate->GetAllWithCommonDescriptors(reconstruction_id, image_id, page_size, page_start);
    }

    unsigned int GetImageCount(const std::string &reconstruction_id)
    {
        return this->_delegate->GetImageCount(reconstruction_id);
    }

    std::shared_ptr<std::unordered_map<T, unsigned int>> GetGlobalDescriptorFrequencies(const std::string &reconstruction_id)
    {
        if (CachedDescriptorStorage::_cached_global_descriptor_frequencies.find(reconstruction_id) == CachedDescriptorStorage::_cached_global_descriptor_frequencies.end())
        {
            // Check if we need to start a new update thread
            std::unique_lock<std::mutex> l(CachedDescriptorStorage::_update_threads_lock);
            if (CachedDescriptorStorage::_update_threads.find(reconstruction_id) == CachedDescriptorStorage::_update_threads.end())
            {
                // Start a new update thread
                CachedDescriptorStorage::_availibility_locks[reconstruction_id] = std::make_shared<std::mutex>();
                CachedDescriptorStorage::_availibility_cv[reconstruction_id] = std::make_shared<std::condition_variable>();
                CachedDescriptorStorage::_writer_active[reconstruction_id] = false;
                CachedDescriptorStorage::_writer_waiting[reconstruction_id] = false;
                CachedDescriptorStorage::_reader_count[reconstruction_id] = 0;
                CachedDescriptorStorage::_rw_lock[reconstruction_id] = std::make_shared<std::mutex>();
                CachedDescriptorStorage::_writer_cv[reconstruction_id] = std::make_shared<std::condition_variable>();
                CachedDescriptorStorage::_reader_done_cv[reconstruction_id] = std::make_shared<std::condition_variable>();
                CachedDescriptorStorage::StartUpdateCache(reconstruction_id);
            }
            l.unlock();
            // Wait for frequencies to be avaiable
            std::unique_lock<std::mutex>
                l2(*CachedDescriptorStorage::_availibility_locks[reconstruction_id]);
            CachedDescriptorStorage::_availibility_cv[reconstruction_id]->wait(l2);
        }
        // reader-write pattern with writer priority
        std::unique_lock<std::mutex> rw_lock(*CachedDescriptorStorage::_rw_lock[reconstruction_id]);
        while (CachedDescriptorStorage::_writer_waiting[reconstruction_id] || CachedDescriptorStorage::_writer_active[reconstruction_id])
        {
            CachedDescriptorStorage::_writer_cv[reconstruction_id]->wait(rw_lock);
        }
        CachedDescriptorStorage::_reader_count[reconstruction_id]++;
        rw_lock.unlock();
        auto v = CachedDescriptorStorage::_cached_global_descriptor_frequencies[reconstruction_id];
        rw_lock.lock();
        CachedDescriptorStorage::_reader_count[reconstruction_id]--;
        if (CachedDescriptorStorage::_reader_count[reconstruction_id] == 0)
        {
            CachedDescriptorStorage::_reader_done_cv[reconstruction_id]->notify_all();
        }
        rw_lock.unlock();
        return v;
    }

    virtual ~CachedDescriptorStorage()
    {
        CachedDescriptorStorage::count--;
        if (CachedDescriptorStorage::count == 0)
        {
            CachedDescriptorStorage::do_update = false;
            for (auto &t : CachedDescriptorStorage::_update_threads)
            {
                t.second->detach();
            }
        }
    };

private:
    void StartUpdateCache(const std::string &reconstruction_id)
    {
        CachedDescriptorStorage::_update_threads[reconstruction_id] = std::make_unique<std::thread>(CachedDescriptorStorage::_UpdateCache,
                                                                                                    this->_delegate,
                                                                                                    reconstruction_id);
    }

    static void _UpdateCache(std::shared_ptr<DescriptorStorage<T>> delegate, const std::string &reconstruction_id)
    {
        do_update = true;
        while (do_update)
        {
            std::unique_lock<std::mutex> rw_lock(*CachedDescriptorStorage::_rw_lock[reconstruction_id]);
            CachedDescriptorStorage::_writer_waiting[reconstruction_id] = true;
            while (CachedDescriptorStorage::_reader_count[reconstruction_id] > 0)
            {
                CachedDescriptorStorage::_reader_done_cv[reconstruction_id]->wait(rw_lock);
            }
            CachedDescriptorStorage::_writer_waiting[reconstruction_id] = false;
            CachedDescriptorStorage::_writer_active[reconstruction_id] = true;
            rw_lock.unlock();
            CachedDescriptorStorage::_cached_global_descriptor_frequencies[reconstruction_id] = delegate->GetGlobalDescriptorFrequencies(reconstruction_id);
            rw_lock.lock();
            CachedDescriptorStorage::_availibility_cv[reconstruction_id]->notify_all();
            CachedDescriptorStorage::_writer_active[reconstruction_id] = false;
            CachedDescriptorStorage::_writer_cv[reconstruction_id]->notify_all();
            rw_lock.unlock();
            using namespace std::literals::chrono_literals;
            std::this_thread::sleep_for(120s);
        }
    }
    std::shared_ptr<DescriptorStorage<T>> _delegate;
    static std::unordered_map<std::string, std::shared_ptr<std::unordered_map<T, unsigned int>>> _cached_global_descriptor_frequencies;
    static std::unordered_map<std::string, std::unique_ptr<std::thread>> _update_threads;
    static std::unordered_map<std::string, std::shared_ptr<std::mutex>> _availibility_locks;
    static std::unordered_map<std::string, std::shared_ptr<std::condition_variable>> _availibility_cv;

    static std::unordered_map<std::string, bool> _writer_waiting;
    static std::unordered_map<std::string, bool> _writer_active;
    static std::unordered_map<std::string, int> _reader_count;
    static std::unordered_map<std::string, std::shared_ptr<std::mutex>> _rw_lock;
    static std::unordered_map<std::string, std::shared_ptr<std::condition_variable>> _writer_cv;
    static std::unordered_map<std::string, std::shared_ptr<std::condition_variable>> _reader_done_cv;

    static std::mutex _update_threads_lock;
    static bool do_update;
    static int count;
};

template <class T>
bool CachedDescriptorStorage<T>::do_update = false;
template <class T>
int CachedDescriptorStorage<T>::count = 1;
template <class T>
std::unordered_map<std::string, std::shared_ptr<std::unordered_map<T, unsigned int>>> CachedDescriptorStorage<T>::_cached_global_descriptor_frequencies;
template <class T>
std::unordered_map<std::string, std::unique_ptr<std::thread>> CachedDescriptorStorage<T>::_update_threads;
template <class T>
std::unordered_map<std::string, std::shared_ptr<std::mutex>> CachedDescriptorStorage<T>::_availibility_locks;
template <class T>
std::unordered_map<std::string, std::shared_ptr<std::condition_variable>> CachedDescriptorStorage<T>::_availibility_cv;
template <class T>
std::mutex CachedDescriptorStorage<T>::_update_threads_lock;
template <class T>
std::unordered_map<std::string, bool> CachedDescriptorStorage<T>::_writer_waiting;
template <class T>
std::unordered_map<std::string, bool> CachedDescriptorStorage<T>::_writer_active;
template <class T>
std::unordered_map<std::string, int> CachedDescriptorStorage<T>::_reader_count;
template <class T>
std::unordered_map<std::string, std::shared_ptr<std::mutex>> CachedDescriptorStorage<T>::_rw_lock;
template <class T>
std::unordered_map<std::string, std::shared_ptr<std::condition_variable>> CachedDescriptorStorage<T>::_writer_cv;
template <class T>
std::unordered_map<std::string, std::shared_ptr<std::condition_variable>> CachedDescriptorStorage<T>::_reader_done_cv;