#!/bin/bash
set -e

# Activate Python virtual environment
source /opt/venv/bin/activate

# Source Zephyr environment
source /workspace/zephyr/zephyr-env.sh

# Optional: direkt in Workspace gehen
cd /workspace

# Make scripts executable
chmod +x /workspace/zephyr-sht31-sensor/temp_udp_app/scripts/*.sh

# Falls ein Befehl übergeben wird, ausführen, sonst Bash
if [ $# -eq 0 ]; then
    exec bash
else
    exec "$@"
fi
