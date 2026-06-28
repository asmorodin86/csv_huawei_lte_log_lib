# csv_huawei_lte_log_lib

Production scaffold for Huawei LTE file ingestion logging:
- local daily logs by catalog node (vendor/generation/segment/model)
- global monthly logs
- md5 registry interface for dedup

## Structure
- `config/app.yaml` - runtime configuration
- `sql/md5_registry.sql` - registry DDL
- `include/ingestion` - public headers
- `src` - implementation

## Build
```bash
cmake -S . -B build
cmake --build build -j
```

## Notes
- `compute_md5` is a stub and should be replaced with OpenSSL/Crypto++ implementation.
- `Md5Registry` is an interface; connect PostgreSQL/Oracle backend as needed.
