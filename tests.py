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
    response = curl_request(f"{BASE_URL}/echo/{random_string(6)}")
    print(response)
