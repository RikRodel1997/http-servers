# Standard Library Imports
import random
import string
import subprocess


def random_string(len: int) -> str:
    return "".join(random.choices(string.ascii_uppercase + string.digits, k=len))


def curl_request(url: str):
    result = subprocess.run(
        ["curl", "-i", url], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
    )
    return result.stdout.strip()
