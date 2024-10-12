# Standard Library Imports
import random
import string
import subprocess

# Third-party Library Imports
import aiohttp

CHECKMARK = "\u0020\u2714\ufe0f\u0020\u0020"
CROSS = "\u0020\033[31m✗\033[0m\u0020\u0020"


def random_string(n: int) -> str:
    return "".join(random.choices(string.ascii_uppercase + string.digits, k=n))


def curl_request(url: str, headers: dict[str, str] = None, data: str = None):
    cmd = ["curl", "-i"]

    if headers:
        for k, v in headers.items():
            cmd += ["-H", f"{k}: {v}"]

    if data:
        cmd.append("--data")
        cmd.append(data)

    cmd.append(url)

    result = subprocess.run(
        cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, check=False
    )
    return result.stdout.strip()


async def async_fetch(session: aiohttp.ClientSession, url):
    async with session.get(url) as response:
        return response, await response.text()


def format_assert_msg(msg: str, fail_or_pass: str):
    if fail_or_pass == "pass":
        print(f"{CHECKMARK} {msg} test passed!")
    else:
        print(f"{CROSS} {msg} test failed!")


def format_assert_exception(msg: str):
    print(f"\u0020\u0020\u0020\u0020 -> {msg}")
