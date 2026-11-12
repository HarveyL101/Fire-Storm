#!/usr/bin/env bash
set -euo pipefail
# Building from a cached instance prevents having to reinstall tools every time.
podman build -t firestorm-ubuntu -f Containerfile .

# Run the stored Ubuntu-24.04 container.
podman run --rm -it \
    --replace \
    --network=host \
    --name firestorm-ubuntu \
    -v "$PWD":/workspace:Z \
    -w /workspace \
    firestorm-ubuntu \
    bash