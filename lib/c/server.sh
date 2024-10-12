#!/bin/bash

BASE_PATH="lib/c"

if [ "$1" == "test" ]; then
    gcc ${BASE_PATH}/main.c ${BASE_PATH}/src/request.c ${BASE_PATH}/src/serv_settings.c -o servers/c_server -lz
elif [ "$1" == "debug" ]; then
    echo "Running server in debug mode"
    gcc ${BASE_PATH}/main.c ${BASE_PATH}/src/request.c ${BASE_PATH}/src/serv_settings.c -o servers/c_server -lz && servers/c_server
else
    echo "The first argument should be test or debug"
    exit 1
fi