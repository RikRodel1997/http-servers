# Standard Library Imports
import argparse

# Custom Library Imports
import servers
import tests


def main(lang: str):
    server = servers.languages[lang]()

    tests.test_home()

    server.terminate()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Enter an argument to run servers and tests"
    )
    parser.add_argument(
        "-l",
        "--lang",
        type=str,
        help="The programming language that the server is written in.",
        required=True,
    )
    args = parser.parse_args()
    main(args.lang)
