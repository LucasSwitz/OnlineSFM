#pragma once
#include "reconstruction_storage.h"
#include "sql_storage.h"
#include "server.pb.h"

class SQLReconstructionStorage : public ReconstructionStorageAdapter, public SQLStorage {
    public:
        SQLReconstructionStorage(
                       sql::Driver* driver, 
                       std::shared_ptr<sql::Connection> connection,
                       const std::string& table);
        void Store(const ReconstructionData& data);
        ReconstructionData Get(const std::string& id);
        void Delete(const std::string& id);
    private:
        std::string _table;
};