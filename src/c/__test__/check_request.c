#include <check.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/request.h"

START_TEST(test_parse_request_valid) {
    request req = parse_request("POST /home");
    ck_assert_str_eq(req.method, "POST");
    ck_assert_str_eq(req.path, "/home");
}
END_TEST

START_TEST(test_parse_request_invalid_method) {
    request req = parse_request("TOOLONGMETHOD /home");
    ck_assert_str_eq(req.method, "");
    ck_assert_str_eq(req.path, "/invalid");
}
END_TEST

START_TEST(test_valid_method_true) {
    ck_assert_int_eq(valid_method("POST"), 1);
    ck_assert_int_eq(valid_method("GET"), 1);
    ck_assert_int_eq(valid_method("PUT"), 1);
    ck_assert_int_eq(valid_method("DELETE"), 1);
}
END_TEST

START_TEST(test_valid_method_false) { ck_assert_int_eq(valid_method("PATCH"), 0); }
END_TEST

START_TEST(test_echo_tail_exists) {
    char tail[6];
    get_echo_tail("/echo/TAIL12", tail);
    ck_assert_str_eq(tail, "TAIL12");
}
END_TEST

START_TEST(test_echo_tail_doesnt_exists) {
    char tail[6];
    get_echo_tail("/echo/", tail);
    ck_assert_str_eq(tail, "");
}
END_TEST

Suite* request_suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("Request");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_parse_request_valid);
    tcase_add_test(tc_core, test_parse_request_invalid_method);
    tcase_add_test(tc_core, test_valid_method_true);
    tcase_add_test(tc_core, test_valid_method_false);
    tcase_add_test(tc_core, test_echo_tail_exists);
    tcase_add_test(tc_core, test_echo_tail_doesnt_exists);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite* s;
    SRunner* sr;

    s = request_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
