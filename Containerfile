# Ensures build-essential + make tools persist across sessions.
FROM ubuntu:24.04
RUN apt-get update && \
    apt-get install -y build-essential make && \
    rm -rf /var/lib/apt/lists/*
WORKDIR /workspace