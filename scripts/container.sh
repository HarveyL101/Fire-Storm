# Script to spin up a Ubuntu 24 container, ensuring compatibility since this was developed on a Fedora Distro.
podman run --rm -it --network host -v "$PWD":/work:Z -w /work ubuntu:24.04 bash
# Within Ubuntu
apt update && apt install -y build-essential make python3 git