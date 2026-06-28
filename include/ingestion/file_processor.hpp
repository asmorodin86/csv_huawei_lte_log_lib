#pragma once
#include "logger.hpp"
#include "md5_registry.hpp"
#include <filesystem>

namespace ingestion {

class FileProcessor {
public:
    FileProcessor(Logger& logger, Md5Registry& registry);

    void process_file(const std::filesystem::path& file_path,
                      const CatalogContext& catalog,
                      const std::string& source_name,
                      const std::string& batch_id);

private:
    Logger& logger_;
    Md5Registry& registry_;

    std::string compute_md5(const std::filesystem::path& file_path);
};

} // namespace ingestion
