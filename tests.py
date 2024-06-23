# Third-party Library Imports
import requests
from assertpy import assert_that


def test_home():
    response = requests.get("http://localhost:4221/")
    assert_that(response.status_code).is_equal_to(200)
    assert_that(response.headers).is_equal_to({})
    print("Home directory works as expected!")
