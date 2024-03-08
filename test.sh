#!/bin/bash

set -e

./build.sh

cd build && make

./board_test
