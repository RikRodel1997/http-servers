# Standard Library Imports
import asyncio

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
