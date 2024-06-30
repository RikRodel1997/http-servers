import subprocess


def c():
    subprocess.run(["chmod", "+x", "./src/c/server.sh"])
    subprocess.run(["./src/c/server.sh"])
    return subprocess.Popen(["./servers/c_server"])


languages = {"c": c}
