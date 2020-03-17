#pragma once
#include "reconstruction_storage.h"
#include "sql_storage.h"
#include "server.pb.h"

class SQLReconstructionStorage : public ReconstructionStorageAdapter, public SQLStorage {
    public:
        SQLReconstructionStorage(const std::string& address, 
                   const std::string& user, 
                   const std::string& pass, 
                   const std::string& db,
                   const std::string& table);
        void CreateIfNew(const std::string& id, const std::string& path);
        ReconstructionData Get(const std::string& id);
        void Delete(const std::string& id);
    private:
        std::string _table;
};