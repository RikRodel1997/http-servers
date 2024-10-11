# Standard Library Imports
import asyncio
import glob
import os

# Third-party Library Imports
import aiohttp
from assertpy import assert_that

# Custom File Imports
from tests_utils import *

BASE_URL = "http://localhost:4221"


def test_home() -> bool:
    response = curl_request(BASE_URL)

    assert_that(response).is_equal_to("HTTP/1.1 200 OK")

    return True


def test_echo() -> bool:
    string_data = random_string(6)
    response = curl_request(f"{BASE_URL}/echo/{string_data}")
    parts = response.split("\n")

    assert_that(parts[0]).is_equal_to("HTTP/1.1 200 OK")
    assert_that(parts[1]).is_equal_to("Content-Type: text/plain")
    assert_that(parts[2]).is_equal_to("Content-Length: 6")
    assert_that(parts[4]).is_equal_to(string_data)

    return True


def test_unknown() -> bool:
    string_data = random_string(6)
    response = curl_request(f"{BASE_URL}/{string_data}")

    assert_that(response).is_equal_to("HTTP/1.1 404 Not Found")

    return True


def test_user_agent() -> bool:
    response = curl_request(f"{BASE_URL}/user-agent")

    assert_that(response).contains("Content-Length: 11\n\ncurl/7.81.0")

    return True


def test_save_request_body_as_file() -> bool:
    file_string = random_string(3)
    content = random_string(10)
    file_path = f"tmp/file_{file_string}"
    response = curl_request(
        f"{BASE_URL}/files/file_{file_string}",
        {
            "Content-Type: ": "application/octet-stream",
        },
        content,
    )

    assert_that(response).contains("HTTP/1.1 201 Created")
    assert_that(os.path.exists(file_path)).is_equal_to(True)

    with open(file_path, "r", encoding="utf-8") as file:
        data = file.read().replace("\n", "")
        assert_that(len(data)).is_equal_to(10)
        assert_that(data).is_equal_to(content)

    os.remove(file_path)
    return True


async def test_async() -> bool:
    string_data = random_string(6)
    async with aiohttp.ClientSession(skip_auto_headers=["Accept-Encoding"]) as session:
        tasks = []
        tasks.append(async_fetch(session, BASE_URL))
        tasks.append(async_fetch(session, f"{BASE_URL}/echo/{string_data}"))

        # Actually returns a list of tuples to verify response body
        responses = await asyncio.gather(*tasks)

        assert_that(responses[0][0].status).is_equal_to(200)
        assert_that(responses[1][0].status).is_equal_to(200)
        assert_that(responses[1][0].headers).is_equal_to(
            {"Content-Type": "text/plain", "Content-Length": "6"}
        )
        assert_that(responses[1][1]).is_equal_to(string_data)

    return True
