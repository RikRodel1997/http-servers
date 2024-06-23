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


languages = {"c": c}
