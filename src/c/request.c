#include <stdio.h>
#include <string.h>

#include "request.h"

request parse_request(const char* received_request) {
    request req;
    char method[METHOD_LENGTH];
    char path[PATH_LENGTH];

    // FIXME: this isnt working, sscanf simply copies the first 6 characters into method
    int ret = sscanf(received_request, "%6s %255s", method, path);
    printf("%d, %s\n", ret, received_request);

    if (ret == 2) {
        // return a valid request if everything went well with checking lengths
        strncpy(req.method, method, METHOD_LENGTH - 1);
        req.method[METHOD_LENGTH - 1] = '\0';
        strncpy(req.path, path, PATH_LENGTH - 1);
        req.path[PATH_LENGTH - 1] = '\0';
        return req;
    }

    // invalid the whole request if something went wrong with checking lengths
    strncpy(req.method, "INVALID", METHOD_LENGTH - 1);
    req.method[METHOD_LENGTH - 1] = '\0';
    strncpy(req.path, "/invalid", PATH_LENGTH - 1);
    req.path[PATH_LENGTH - 1] = '\0';
    return req;
}
