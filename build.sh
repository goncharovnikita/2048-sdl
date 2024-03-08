#!/bin/bash

set -e

cmake -DASSETS_DIR=assets -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B build
