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
    all_tests = tests.available

    try:

        print(f"\nRunning tests for {lang.capitalize()} server")
        for test in all_tests.items():
            if "async" in test[0]:
                asyncio.run(test[1]())
            else:
                test[1]()

    except Exception as e:
        print(f"Caught an exception while executing tests for {lang.capitalize()} server")
        print(e)
    finally:
        print(f"Terminating {lang.capitalize()} server\n")
        server.terminate()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Enter an argument to run servers and tests")
    parser.add_argument(
        "-l",
        "--lang",
        type=str,
        help="The programming language that the server is written in.",
        required=True,
    )
    args = parser.parse_args()
    main(args.lang)
