# http-servers

A repository containing HTTP 1.1 compliant servers in different programming languages. I develop and run everything on a Ubuntu (WSL) instance and explicitely open a VSCode window in WSL. I execute Python with `sudo -E python3 main.py -l c`.
The `servers` folder acts as the output/target folder. Output/executables from compiled languages will end up here.
The `src` folder will contain all the source code for each server.

Every HTTP server in this repository can be tested using the Python test client.
The following behaviors are defined for the servers, the server should accept concurrent connections and respond with:

-   `HTTP/1.1 200 OK\r\n\r\n` from the request to home `GET /`.
-   `HTTP/1.1 404 Not Found\r\n\r\n` from the request `GET /{str}` Where `{str}` is a random string.
-   `HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: {cl}\r\n\r\n{rb}` from the request `GET /user-agent` where `{cl}` is the length of the value in the `User-Agent` header and `{rb}` is the value in the `User-Agent` header.
-   `HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: {cl}\r\n\r\n{rb}` from the request `GET /echo/{str}` where `{cl}` is the length of the value in `{str}` and `{rb}` is the value of `{str}`. If the request has the `Accept-Encoding` header present, the header value should be scanned for `gzip` and compress `{str}`. `{cl}` and `{rb}` should reflect the compressed response body.

When the server is ran with the `--directory {directory}` argument, the following behavior is expected:

-   `HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Length: {cl}\r\n\r\n{rb}` from request `GET /files/{filename}` where `{cl}` is the length of the contents of the file and `{rb}` is the contents of the file.
-   `HTTP/1.1 404 Not Found\r\n\r\n` from request `GET /files/{filename}` when `{filename}` _doesn't_ exist.
-   `HTTP/1.1 404 Not Found\r\n\r\n` from request `GET /files/{filename}` when `{filename}` _does_ exist but has no content.
-   `HTTP/1.1 201 Created\r\n\r\n` from request `POST /files/{filename}` and saves the request body to a file in the `tmp` directory.
