#!/bin/bash

TST_PATH="src/c/__test__"
LIBS="-lcheck -lm -lpthread -lrt -lsubunit"

gcc -o ./servers/check_tests_request ${TST_PATH}/check_request.c src/c/src/request.c ${LIBS} && ./servers/check_tests_request
gcc -o ./servers/check_tests_serv_settings ${TST_PATH}/check_serv_settings.c src/c/src/serv_settings.c ${LIBS} && ./servers/check_tests_serv_settings
