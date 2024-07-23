import subprocess


def c():
    subprocess.run(
        [
            "gcc",
            "src/c/main.c",
            "src/c/src/request.c",
            "-o",
            "servers/c_server",
            "-lz",
        ]
    )
    subprocess.run(
        [
            "chmod",
            "+x",
            "./src/c/server.sh",
        ]
    )
    subprocess.run(
        [
            "./src/c/server.sh",
        ]
    )
    return subprocess.Popen(
        [
            "./servers/c_server",
        ]
    )


languages = {"c": c}
