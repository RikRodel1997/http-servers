#include <arpa/inet.h>
#include <check.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../serv_settings.h"

START_TEST(test_create_addr_in) {
    addr_in addr = create_addr_in(9999);

    char actual_addr[INET_ADDRSTRLEN];
    const char* expected_addr = "0.0.0.0";
    inet_ntop(AF_INET, &addr.sin_addr, actual_addr, sizeof(actual_addr));

    ck_assert_str_eq(actual_addr, expected_addr);
    ck_assert_int_eq(htons(addr.sin_port), 9999);
    ck_assert_int_eq(htons(addr.sin_family), 512);
}
END_TEST

Suite* serv_settings_suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("Server Settings");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_create_addr_in);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite* s;
    SRunner* sr;

    s = serv_settings_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    printf("\n");

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
