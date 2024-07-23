#ifndef REQUEST_H
#define REQUEST_H

#define METHOD_LENGTH   7      // 6 characters + 1 for null terminator
#define PATH_LENGTH     256    // Adjust based on expected maximum path length
#define HEADER_LENGTH   8190   // As defined by apache
#define PROTOCOL_LENGTH 9      // 8 characters for HTTP/1.1 +1 for null terminator

typedef struct {
    char method[METHOD_LENGTH];
    char path[PATH_LENGTH];
    char protocol[PROTOCOL_LENGTH];
    char headers[HEADER_LENGTH];
} request;

request parse_request(char* received_request);
void get_echo_tail(const char* path, char* tail);
int valid_method(char* method);
void parse_user_agent(char* headers, char* user_agent);

#endif   // REQUEST_H
