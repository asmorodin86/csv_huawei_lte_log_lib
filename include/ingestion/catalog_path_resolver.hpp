#pragma once
#include "models.hpp"
#include <string>

namespace ingestion {

class CatalogPathResolver {
public:
    static std::string app_log_path(const CatalogContext& c);
    static std::string error_log_path(const CatalogContext& c);
    static std::string md5_log_path(const CatalogContext& c);

    static std::string global_app_log_path();
    static std::string global_error_log_path();
    static std::string global_md5_log_path();
};

} // namespace ingestion
