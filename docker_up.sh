#!/bin/bash
set -e

docker build -t kata-cpp .

docker run --rm -it -v $(pwd):/workspaces/kata-bootstrap kata-cpp