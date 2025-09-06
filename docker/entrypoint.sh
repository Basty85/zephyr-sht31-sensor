#!/bin/sh

# Navigate to the application directory
cd /app/temp_udp_app

# Execute the build script
./scripts/build.sh

# Keep the container running
tail -f /dev/null
