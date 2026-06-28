#pragma once
#include "models.hpp"

namespace ingestion {

class Md5Registry {
public:
    virtual ~Md5Registry() = default;
    virtual bool exists_success(const std::string& source_name, const CatalogContext& c, const std::string& md5) = 0;
    virtual void insert_in_progress(const Md5RegistryRecord& rec) = 0;
    virtual void update_success(const Md5RegistryRecord& rec) = 0;
    virtual void update_failed(const Md5RegistryRecord& rec, const std::string& error_message) = 0;
};

} // namespace ingestion
