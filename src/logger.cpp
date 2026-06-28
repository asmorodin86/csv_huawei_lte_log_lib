#include "ingestion/logger.hpp"
#include "ingestion/catalog_path_resolver.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <filesystem>
#include <unordered_map>
#include <mutex>

namespace ingestion {

namespace {
std::mutex g_mx;
std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> g_loggers;

std::shared_ptr<spdlog::logger> make_or_get(const std::string& name, const std::string& path) {
    std::lock_guard<std::mutex> lk(g_mx);
    auto it = g_loggers.find(name);
    if (it != g_loggers.end()) return it->second;

    std::filesystem::create_directories(std::filesystem::path(path).parent_path());
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path, true);
    auto lg = std::make_shared<spdlog::logger>(name, sink);
    lg->set_pattern("%Y-%m-%dT%H:%M:%S.%eZ [%l] %v");
    lg->set_level(spdlog::level::debug);
    g_loggers[name] = lg;
    return lg;
}

std::string key(const CatalogContext& c) {
    return c.vendor + "_" + c.generation + "_" + c.segment + "_" + c.model;
}
} // namespace

Logger::Logger() {
    global_app_ = make_or_get("global_app", CatalogPathResolver::global_app_log_path());
    global_error_ = make_or_get("global_error", CatalogPathResolver::global_error_log_path());
    global_md5_ = make_or_get("global_md5", CatalogPathResolver::global_md5_log_path());
}

std::shared_ptr<spdlog::logger> Logger::get_local_app(const CatalogContext& c) {
    return make_or_get("app_" + key(c), CatalogPathResolver::app_log_path(c));
}

std::shared_ptr<spdlog::logger> Logger::get_local_error(const CatalogContext& c) {
    return make_or_get("err_" + key(c), CatalogPathResolver::error_log_path(c));
}

std::shared_ptr<spdlog::logger> Logger::get_local_md5(const CatalogContext& c) {
    return make_or_get("md5_" + key(c), CatalogPathResolver::md5_log_path(c));
}

void Logger::info(const std::string& event, const FileMeta& meta, const std::string& message) {
    auto local = get_local_app(meta.catalog);
    auto payload = spdlog::fmt_lib::format(
        R"({"event":"{}","vendor":"{}","generation":"{}","segment":"{}","model":"{}","source_name":"{}","file_name":"{}","file_path":"{}","md5":"{}","batch_id":"{}","message":"{}"})",
        event, meta.catalog.vendor, meta.catalog.generation, meta.catalog.segment, meta.catalog.model,
        meta.source_name, meta.file_name, meta.file_path, meta.md5, meta.batch_id, message);
    local->info(payload);
    global_app_->info(payload);
}

void Logger::warn(const std::string& event, const FileMeta& meta, const std::string& message) {
    auto local = get_local_app(meta.catalog);
    auto payload = spdlog::fmt_lib::format(
        R"({"event":"{}","vendor":"{}","generation":"{}","segment":"{}","model":"{}","source_name":"{}","file_name":"{}","file_path":"{}","md5":"{}","batch_id":"{}","message":"{}"})",
        event, meta.catalog.vendor, meta.catalog.generation, meta.catalog.segment, meta.catalog.model,
        meta.source_name, meta.file_name, meta.file_path, meta.md5, meta.batch_id, message);
    local->warn(payload);
    global_app_->warn(payload);
}

void Logger::error(const std::string& event, const FileMeta& meta, const ErrorInfo& err) {
    auto local = get_local_error(meta.catalog);
    auto payload = spdlog::fmt_lib::format(
        R"({"event":"{}","vendor":"{}","generation":"{}","segment":"{}","model":"{}","source_name":"{}","file_name":"{}","file_path":"{}","md5":"{}","batch_id":"{}","stage":"{}","error_code":"{}","error_message":"{}","stack_trace":"{}"})",
        event, meta.catalog.vendor, meta.catalog.generation, meta.catalog.segment, meta.catalog.model,
        meta.source_name, meta.file_name, meta.file_path, meta.md5, meta.batch_id,
        err.stage, err.error_code, err.error_message, err.stack_trace.value_or(""));
    local->error(payload);
    global_error_->error(payload);
}

void Logger::md5(const std::string& event, const Md5RegistryRecord& rec, const std::string& message) {
    auto local = get_local_md5(rec.catalog);
    auto payload = spdlog::fmt_lib::format(
        R"({"event":"{}","vendor":"{}","generation":"{}","segment":"{}","model":"{}","source_name":"{}","file_name":"{}","file_path":"{}","md5":"{}","batch_id":"{}","status":"{}","skip_reason":"{}","rows_loaded":{},"message":"{}"})",
        event, rec.catalog.vendor, rec.catalog.generation, rec.catalog.segment, rec.catalog.model,
        rec.source_name, rec.file_name, rec.file_path, rec.md5, rec.batch_id,
        rec.status, rec.skip_reason, rec.rows_loaded, message);
    local->info(payload);
    global_md5_->info(payload);
}

} // namespace ingestion
