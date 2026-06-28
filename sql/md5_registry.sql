CREATE TABLE IF NOT EXISTS md5_registry (
    id BIGSERIAL PRIMARY KEY,
    vendor TEXT NOT NULL,
    generation TEXT NOT NULL,
    segment TEXT NOT NULL,
    model TEXT NOT NULL,

    source_name TEXT NOT NULL,
    file_name TEXT NOT NULL,
    file_path TEXT,
    file_size_bytes BIGINT NOT NULL,
    md5 TEXT NOT NULL,

    status TEXT NOT NULL,
    skip_reason TEXT,
    rows_loaded BIGINT DEFAULT 0,
    error_message TEXT,
    batch_id TEXT,

    created_at TIMESTAMPTZ NOT NULL DEFAULT now(),
    finished_at TIMESTAMPTZ
);

CREATE UNIQUE INDEX IF NOT EXISTS uq_md5_registry_success
ON md5_registry(vendor, generation, segment, model, source_name, md5)
WHERE status = 'SUCCESS';

CREATE INDEX IF NOT EXISTS idx_md5_registry_status ON md5_registry(status);
CREATE INDEX IF NOT EXISTS idx_md5_registry_batch_id ON md5_registry(batch_id);
CREATE INDEX IF NOT EXISTS idx_md5_registry_created_at ON md5_registry(created_at);
