#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "request.h"

request parse_request(const char* received_request) {
    request req;
    char method[METHOD_LENGTH];
    char path[PATH_LENGTH];

    int ret = sscanf(received_request, "%6s %255s", method, path);
    int method_valid = valid_method(method);

    if (ret != 2 || method_valid == 0) {
        // invalid the whole request if something went wrong with checking lengths
        strncpy(req.method, "", METHOD_LENGTH - 1);
        req.method[METHOD_LENGTH - 1] = '\0';
        strncpy(req.path, "/invalid", PATH_LENGTH - 1);
        req.path[PATH_LENGTH - 1] = '\0';
        return req;
    }

    // return a valid request if everything went well with checking lengths
    strncpy(req.method, method, METHOD_LENGTH - 1);
    req.method[METHOD_LENGTH - 1] = '\0';
    strncpy(req.path, path, PATH_LENGTH - 1);
    req.path[PATH_LENGTH - 1] = '\0';
    return req;
}

int valid_method(char* method) {
    if (strncmp(method, "GET", 3) == 0 || strncmp(method, "POST", 4) == 0 || strncmp(method, "PUT", 3) == 0 ||
        strncmp(method, "DELETE", 6) == 0) {
        return 1;
    }
    return 0;
}