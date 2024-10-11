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
        ],
        check=True,
    )
    subprocess.run(
        [
            "chmod",
            "+x",
            "./src/c/server.sh",
        ],
        check=True,
    )
    subprocess.run(
        [
            "./src/c/server.sh",
            "test",
        ],
        check=True,
    )
    return subprocess.Popen(
        [
            "./servers/c_server",
            "--directory",
            "tmp",
        ]
    )


languages = {"c": c}
