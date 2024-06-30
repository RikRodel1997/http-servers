#ifndef REQUEST_H
#define REQUEST_H

#define METHOD_LENGTH 7     // 6 characters + 1 for null terminator
#define PATH_LENGTH   256   // Adjust based on expected maximum path length

typedef struct {
    char method[METHOD_LENGTH];
    char path[PATH_LENGTH];
} request;

request parse_request(const char* received_request);
void get_echo_tail(const char* path, char* tail);
int valid_method(char* method);

#endif   // REQUEST_H
