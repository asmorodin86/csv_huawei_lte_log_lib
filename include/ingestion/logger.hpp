#pragma once
#include "models.hpp"
#include <memory>

namespace spdlog { class logger; }

namespace ingestion {

class Logger {
public:
    Logger();

    void info(const std::string& event, const FileMeta& meta, const std::string& message);
    void warn(const std::string& event, const FileMeta& meta, const std::string& message);
    void error(const std::string& event, const FileMeta& meta, const ErrorInfo& err);
    void md5(const std::string& event, const Md5RegistryRecord& rec, const std::string& message);

private:
    std::shared_ptr<spdlog::logger> get_local_app(const CatalogContext& c);
    std::shared_ptr<spdlog::logger> get_local_error(const CatalogContext& c);
    std::shared_ptr<spdlog::logger> get_local_md5(const CatalogContext& c);

    std::shared_ptr<spdlog::logger> global_app_;
    std::shared_ptr<spdlog::logger> global_error_;
    std::shared_ptr<spdlog::logger> global_md5_;
};

} // namespace ingestion
