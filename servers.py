import subprocess


def c():
    subprocess.run(
        [
            "gcc",
            "src/c/main.c",
            "src/c/src/request.c",
            "src/c/src/serv_settings.c",
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
            "test",
        ]
    )
    return subprocess.Popen(
        [
            "./servers/c_server",
            "--directory",
            "tmp",
        ]
    )


languages = {"c": c}
