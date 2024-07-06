# Standard Library Imports
import argparse
import asyncio

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

    try:
        if tests.test_home():
            print(f"Home path for {lang} server works as expected!")

        if tests.test_echo():
            print(f"Echo path with random string for {lang} server works as expected!")

        if asyncio.run(tests.test_async()):
            print(f"Async test for {lang} server works as expected!")

    except Exception as e:
        print(f"Caught an exception while executing tests for {lang} server")
        print(e)
    finally:
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
