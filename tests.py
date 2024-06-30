# Third-party Library Imports
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
