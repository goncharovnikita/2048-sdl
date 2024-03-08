#!/bin/bash

set -e

./build.sh

cd build && make

./2048
