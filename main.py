# Standard Library Imports
import argparse

# Custom Library Imports
import servers
import tests


def main(lang: str):
    lang_options = servers.languages.keys()
    if lang not in lang_options:
        print(f"{lang} is not supported (yet) or it's defined different.")
        print(f"Choose from {', '.join(lang_options)}")
        return

    server = servers.languages[lang.lower()]()

    if tests.test_home():
        print(f"Home directory for {lang} server works as expected!")

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
