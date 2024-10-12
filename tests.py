# Standard Library Imports
import asyncio
import os

# Third-party Library Imports
import aiohttp
from assertpy import assert_that

# Custom File Imports
import utils

BASE_URL = "http://localhost:4221"


def test_home() -> bool:
    response = utils.curl_request(BASE_URL)

    try:
        assert_that(response).is_equal_to("HTTP/1.1 200 OK")
    except AssertionError as e:
        utils.format_assert_msg("/ path", "fail")
        utils.format_assert_exception(f"Response: {response}")
        utils.format_assert_exception(f"Exception: {e}")
        return False

    utils.format_assert_msg("/ path", "pass")
    return True


def test_echo() -> bool:
    string_data = utils.random_string(6)
    response = utils.curl_request(f"{BASE_URL}/echo/{string_data}")
    parts = response.split("\n")

    try:
        assert_that(parts[0]).is_equal_to("HTTP/1.1 200 OK")
        assert_that(parts[1]).is_equal_to("Content-Type: text/plain")
        assert_that(parts[2]).is_equal_to("Content-Length: 6")
        assert_that(parts[4]).is_equal_to(string_data)
    except AssertionError as e:
        utils.format_assert_msg("/echo path", "fail")
        utils.format_assert_exception(f"Response: {response}")
        utils.format_assert_exception(f"Exception: {e}")
        return False

    utils.format_assert_msg("/echo path", "pass")
    return True


def test_unknown() -> bool:
    string_data = utils.random_string(6)
    response = utils.curl_request(f"{BASE_URL}/{string_data}")

    try:
        assert_that(response).is_equal_to("HTTP/1.1 404 Not Found")
    except AssertionError as e:
        utils.format_assert_msg("unknown path handling", "fail")
        utils.format_assert_exception(f"Response: {response}")
        utils.format_assert_exception(f"Exception: {e}")
        return False

    utils.format_assert_msg("unknown path handling", "pass")
    return True


def test_user_agent() -> bool:
    response = utils.curl_request(f"{BASE_URL}/user-agent")

    try:
        assert_that(response).contains("Content-Length: 11\n\ncurl/7.81.0")
    except AssertionError as e:
        utils.format_assert_msg("/user-agent path", "fail")
        utils.format_assert_exception(f"Response: {response}")
        utils.format_assert_exception(f"Exception: {e}")
        return False

    utils.format_assert_msg("/user-agent path", "pass")
    return True


def test_post_files_filename() -> bool:
    file_string = utils.random_string(3)
    content = utils.random_string(10)
    file_path = f"tmp/file_{file_string}"
    response = utils.curl_request(
        f"{BASE_URL}/files/file_{file_string}",
        {
            "Content-Type: ": "application/octet-stream",
        },
        content,
    )

    try:
        assert_that(response).contains("HTTP/1.1 201 Created")
    except AssertionError as e:
        utils.format_assert_msg("POST /files/[filename]", "fail")
        utils.format_assert_exception(f"Response: {response}")
        utils.format_assert_exception(f"Exception: {e}")
        return False

    with open(file_path, "r", encoding="utf-8") as file:
        data = file.read().replace("\n", "")
        try:
            assert_that(os.path.exists(file_path)).is_equal_to(True)
            assert_that(len(data)).is_equal_to(10)
            assert_that(data).is_equal_to(content)
        except AssertionError as e:
            utils.format_assert_msg("POST /files/[filename]", "fail")
            utils.format_assert_exception(f"Data: {data}")
            utils.format_assert_exception(f"Exception: {e}")
            return False

    os.remove(file_path)
    utils.format_assert_msg("POST /files/[filename]", "pass")
    return True


async def test_async() -> bool:
    string_data = utils.random_string(6)
    async with aiohttp.ClientSession(skip_auto_headers=["Accept-Encoding"]) as session:
        tasks = []
        tasks.append(utils.async_fetch(session, BASE_URL))
        tasks.append(utils.async_fetch(session, f"{BASE_URL}/echo/{string_data}"))

        # Actually returns a list of tuples to verify response body
        responses = await asyncio.gather(*tasks)

        try:
            assert_that(responses[0][0].status).is_equal_to(200)
            assert_that(responses[1][0].status).is_equal_to(200)
            assert_that(responses[1][0].headers).is_equal_to(
                {"Content-Type": "text/plain", "Content-Length": "6"}
            )
            assert_that(responses[1][1]).is_equal_to(string_data)
        except AssertionError as e:
            utils.format_assert_msg("async requests", "fail")
            utils.format_assert_exception(f"Response: {responses}")
            utils.format_assert_exception(f"Exception: {e}")
            return False

        utils.format_assert_msg("async requests", "pass")
        return True

    return True


available = {
    "test_async": test_async,
    "test_home": test_home,
    "test_echo": test_echo,
    "test_unknown": test_unknown,
    "test_user_agent": test_user_agent,
    "test_post_files_filename": test_post_files_filename,
}
