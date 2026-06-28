#include "ingestion/logger.hpp"
#include <iostream>

int main() {
    try {
        ingestion::Logger logger;
        std::cout << "csv_huawei_lte_log_lib scaffold initialized" << std::endl;
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }
}
