#pragma once
#include <string>
#include <cstdint>
#include <optional>

namespace ingestion {

struct CatalogContext {
    std::string vendor;
    std::string generation;
    std::string segment;
    std::string model;
};

struct FileMeta {
    std::string file_name;
    std::string file_path;
    std::string source_name;
    std::uintmax_t file_size_bytes{};
    std::string md5;
    std::string batch_id;
    CatalogContext catalog;
};

struct ErrorInfo {
    std::string error_code;
    std::string error_message;
    std::string stage;
    std::optional<std::string> stack_trace;
};

struct Md5RegistryRecord {
    std::string md5;
    std::string file_name;
    std::string file_path;
    std::string source_name;
    std::uintmax_t file_size_bytes{};
    std::string batch_id;
    std::string status;
    std::string skip_reason;
    std::size_t rows_loaded{};
    CatalogContext catalog;
};

} // namespace ingestion
