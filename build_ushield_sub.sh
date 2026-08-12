#!/bin/bash

# @file        build_ushield_sub.sh
# @brief       Script used to build ArduSub for the UShield FC in the Docker container
# @author      David Rodriguez Ferrero

# Exit immediately if any command exits with a non-zero status
set -e

# Navigate to ardupilot directory
cd ardupilot

echo "=== 1. Configuring Git safe directory ==="
git config --global --add safe.directory '*'

echo "=== 2. Validating Board Target ==="
BOARD_NAME="UShield"
if [ ! -d "libraries/AP_HAL_ChibiOS/hwdef/${BOARD_NAME}" ]; then
    echo "Error: Board directory 'libraries/AP_HAL_ChibiOS/hwdef/${BOARD_NAME}' not found!"
    exit 1
fi

echo "=== 3. Configuring Waf for ${BOARD_NAME} ==="
./waf configure --board "${BOARD_NAME}"

echo "=== 5. Compiling Sub Firmware ==="
./waf sub -j$(nproc)

echo "=== Build Process Completed Successfully ==="