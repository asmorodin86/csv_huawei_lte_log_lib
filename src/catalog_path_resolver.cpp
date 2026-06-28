#include "ingestion/catalog_path_resolver.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace ingestion {

static std::string catalog_base(const CatalogContext& c) {
    return c.vendor + "/" + c.generation + "/" + c.segment + "/" + c.model;
}

static std::string today_ymd() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::gmtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    return oss.str();
}

static std::string current_ym() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::gmtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m");
    return oss.str();
}

std::string CatalogPathResolver::app_log_path(const CatalogContext& c) {
    return "./logs/app/" + catalog_base(c) + "/app-" + today_ymd() + ".log";
}

std::string CatalogPathResolver::error_log_path(const CatalogContext& c) {
    return "./logs/errors/" + catalog_base(c) + "/errors-" + today_ymd() + ".log";
}

std::string CatalogPathResolver::md5_log_path(const CatalogContext& c) {
    return "./logs/md5/" + catalog_base(c) + "/md5-" + today_ymd() + ".log";
}

std::string CatalogPathResolver::global_app_log_path() {
    return "./logs/global/app/global-app-" + current_ym() + ".log";
}

std::string CatalogPathResolver::global_error_log_path() {
    return "./logs/global/errors/global-errors-" + current_ym() + ".log";
}

std::string CatalogPathResolver::global_md5_log_path() {
    return "./logs/global/md5/global-md5-" + current_ym() + ".log";
}

} // namespace ingestion
