# Standard Library Imports
import random
import string
import subprocess

# Third-party Library Imports
import aiohttp


def random_string(len: int) -> str:
    return "".join(random.choices(string.ascii_uppercase + string.digits, k=len))


def curl_request(url: str, headers: dict[str, str] = None):
    cmd = ["curl", "-i", url]

    result = subprocess.run(
        cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
    )
    return result.stdout.strip()


async def async_fetch(session: aiohttp.ClientSession, url):
    async with session.get(url) as response:
        return response, await response.text()
