#!/bin/bash

# Exit immediately if any command exits with a non-zero status
set -e

cd ardupilot

echo "=== 1. Configuring Git safe directory ==="
git config --global --add safe.directory '*'

echo "=== 2. Configuring Waf for UShield board ==="
./waf configure --board UShield

echo "=== 3. Compiling Sub firmware ==="
./waf sub

echo "=== Build Process Completed Successfully ==="
