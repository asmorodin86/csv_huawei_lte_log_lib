#include "ingestion/file_processor.hpp"
#include <filesystem>

namespace ingestion {

FileProcessor::FileProcessor(Logger& logger, Md5Registry& registry)
    : logger_(logger), registry_(registry) {}

void FileProcessor::process_file(const std::filesystem::path& file_path,
                                 const CatalogContext& catalog,
                                 const std::string& source_name,
                                 const std::string& batch_id) {
    FileMeta meta;
    meta.file_name = file_path.filename().string();
    meta.file_path = file_path.string();
    meta.source_name = source_name;
    meta.file_size_bytes = std::filesystem::file_size(file_path);
    meta.batch_id = batch_id;
    meta.catalog = catalog;
    meta.md5 = compute_md5(file_path);

    logger_.info("FILE_DISCOVERED", meta, "File discovered");

    if (registry_.exists_success(source_name, catalog, meta.md5)) {
        Md5RegistryRecord rec{meta.md5, meta.file_name, meta.file_path, source_name, meta.file_size_bytes, batch_id, "SKIPPED", "already_processed", 0, catalog};
        logger_.md5("MD5_DUPLICATE_DETECTED", rec, "Duplicate by md5 - skipped");
        logger_.warn("FILE_SKIPPED_ALREADY_PROCESSED", meta, "Already processed");
        return;
    }

    Md5RegistryRecord rec{meta.md5, meta.file_name, meta.file_path, source_name, meta.file_size_bytes, batch_id, "IN_PROGRESS", "", 0, catalog};
    registry_.insert_in_progress(rec);
    logger_.md5("MD5_REGISTRY_IN_PROGRESS", rec, "Registered in progress");

    try {
        rec.status = "SUCCESS";
        rec.rows_loaded = 100;
        registry_.update_success(rec);
        logger_.md5("MD5_REGISTRY_SUCCESS", rec, "Registered success");
        logger_.info("FILE_PROCESS_SUCCESS", meta, "Processed successfully");
    } catch (const std::exception& ex) {
        rec.status = "FAILED";
        registry_.update_failed(rec, ex.what());
        logger_.md5("MD5_REGISTRY_FAILED", rec, "Registered failure");

        ErrorInfo err{"PROCESSING_FAILED", ex.what(), "processing", std::nullopt};
        logger_.error("FILE_PROCESS_FAILED", meta, err);
    }
}

std::string FileProcessor::compute_md5(const std::filesystem::path&) {
    return "dummy_md5_replace_me";
}

} // namespace ingestion
