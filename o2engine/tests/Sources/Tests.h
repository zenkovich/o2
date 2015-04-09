#pragma once

#define CHECK(condition, fail_str, ...) \
if (!(condition)) { printf(fail_str, __VA_ARGS__); printf("\n"); }