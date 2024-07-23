#!/bin/bash
if [ "$1" == "tests" ]; then
    echo "Running tests for server"
    gcc src/c/main.c src/c/src/request.c -o servers/c_server -lz
elif [ "$1" == "debug" ]; then
    echo "Running server in debug mode"
    gcc src/c/main.c src/c/src/request.c -o servers/c_server -lz && servers/c_server
else
    echo "The first argument should be run or debug"
    exit 1
fi