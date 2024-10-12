import subprocess


def c():
    subprocess.run(
        [
            "gcc",
            "lib/c/main.c",
            "lib/c/src/request.c",
            "lib/c/src/serv_settings.c",
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
            "lib/c/server.sh",
        ],
        check=True,
    )
    subprocess.run(
        [
            "lib/c/server.sh",
            "test",
        ],
        check=True,
    )
    return subprocess.Popen(
        [
            "servers/c_server",
            "--directory",
            "tmp",
        ]
    )


languages = {"c": c}
