# Standard Library Imports
import random
import string
import subprocess

# Third-party Library Imports
import aiohttp


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
