#!/bin/bash

TST_PATH="src/c/__test__"
LIBS="-lcheck -lm -lpthread -lrt -lsubunit"

gcc -o ${TST_PATH}/check_request ${TST_PATH}/check_request.c src/c/src/request.c ${LIBS} && ./${TST_PATH}/check_request