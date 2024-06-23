import subprocess


def c():
    subprocess.run(
        [
            "gcc",
            "src/c/server.c",
            "-o",
            "servers/c_server",
            "-lz",
        ]
    )
    return subprocess.Popen(["./servers/c_server"])


c_server = c()
result = subprocess.run(["curl", "http://localhost:4221"], capture_output=True)

if result.returncode == 0:
    output = result.stdout.decode("utf-8")
    headers = result.stdout
    print(output)
else:
    print("Curl command failed!")
    error = result.stderr.decode("utf-8")
    print(error)

c_server.terminate()
