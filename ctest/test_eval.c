#include "test.h"
#include "../src/jslite.h"
#include "../src/nodes.h"
#include "../src/jslite.c"
#include "../src/nodes.c"
#include "../src/eval.c"

void
test_add_handles_numbers()
{
  TEST(jl_add(JLNUM(2), JLNUM(2))->number.val == 4);
  TEST(jl_add(JLNUM(1234.56), JLNUM(789.123))->number.val == 2023.683);
}

void
test_add_concatenates_strings()
{
  TEST(STREQ(jl_add(JLSTR("hello "), JLSTR("world"))->string.ptr, "hello world"));
  TEST(STREQ(jl_add(JLSTR("1"), JLSTR("2"))->string.ptr, "12"));
}

void
test_add_handles_mixed_types()
{
  TEST(STREQ(jl_add(JLSTR("hello"), JLNUM(12))->string.ptr, "hello12"));
  TEST(STREQ(jl_add(JLSTR("hello"), JLUNDEF())->string.ptr, "helloundefined"));
  TEST(jl_add(JLNUM(79), JLNULL())->number.val == 79);
  TEST(jl_add(JLNUM(42), JLUNDEF())->number.is_nan == 1);
  TEST(jl_add(JLNUM(12), JLBOOL(1))->number.val == 13);
  TEST(jl_add(JLNUM(4), JLBOOL(0))->number.val == 4);
  TEST(jl_add(JLNAN(), JLNUM(1))->number.is_nan == 1);
}

void 
test_sub_handles_numbers()
{
  TEST(jl_sub(JLNUM(5), JLNUM(3))->number.val == 2);
  TEST(jl_sub(JLNUM(19822.432), JLNUM(4.230))->number.val == 19818.202);
  TEST(jl_sub(JLNUM(2), JLNUM(5))->number.val == -3);
}

void
test_sub_handles_non_numeric_types()
{
  TEST(jl_sub(JLSTR("hello"), JLSTR("world"))->number.is_nan == 1);
  TEST(jl_sub(JLSTR("hello"), JLUNDEF())->number.is_nan == 1);
  TEST(jl_sub(JLSTR("12"), JLUNDEF())->number.is_nan == 1);
  TEST(jl_sub(JLSTR("5"), JLNULL())->number.val == 5);
  TEST(jl_sub(JLSTR("14"), JLBOOL(1))->number.val == 13);
  TEST(jl_sub(JLSTR("42"), JLSTR("2.4124"))->number.val == 39.5876);
}

void
test_mul_handles_numbers()
{
  TEST(jl_mul(JLNUM(2), JLNUM(2))->number.val == 4);
  TEST(jl_mul(JLNUM(-4), JLNUM(8))->number.val == -32);
  TEST(jl_mul(JLNUM(-457.6412), JLNUM(-2.1409))->number.val == 979.76404508);
}

void
test_mul_handles_non_numeric_types()
{
  TEST(jl_mul(JLSTR("joe"), JLSTR("sixpack"))->number.is_nan == 1);
  TEST(jl_mul(JLSTR("5"), JLSTR("3"))->number.val == 15);
  TEST(jl_mul(JLSTR("1000"), JLNULL())->number.val == 0);
  TEST(jl_mul(JLSTR("4"), JLUNDEF())->number.is_nan == 1);
}

void
test_div_handles_numbers()
{
  TEST(jl_div(JLNUM(9), JLNUM(3))->number.val == 3);
  TEST(jl_div(JLNUM(-4), JLNUM(8))->number.val == -0.5);
  TEST(jl_div(JLNUM(50201513), JLNUM(0.0024))->number.val == 20917297083.333336);
}

void
test_div_handles_non_numeric_types()
{
  TEST(jl_div(JLSTR("joe"), JLSTR("sixpack"))->number.is_nan == 1);
  TEST(jl_div(JLSTR("15"), JLSTR("3"))->number.val == 5);
  TEST(jl_div(JLSTR("40"), JLSTR("-8"))->number.val == -5);
  TEST(jl_div(JLSTR("1000"), JLNULL())->number.is_inf == 1);
  TEST(jl_div(JLSTR("4"), JLUNDEF())->number.is_nan == 1);
}

void
test_mod_handles_numbers()
{
  TEST(jl_mod(JLNUM(9), JLNUM(3))->number.val == 0);
  TEST(jl_mod(JLNUM(-4), JLNUM(3))->number.val == -1);
  TEST(jl_mod(JLNUM(13241), JLNUM(13.3))->number.val == 7.499999999999293);
}

void
test_mod_handles_non_numeric_types()
{
  TEST(jl_mod(JLSTR("joe"), JLSTR("sixpack"))->number.is_nan == 1);
  TEST(jl_mod(JLSTR("5"), JLSTR("3"))->number.val == 2);
  TEST(jl_mod(JLSTR("1000"), JLBOOL(1))->number.val == 0);
  TEST(jl_mod(JLSTR("4"), JLUNDEF())->number.is_nan == 1);
}

void
test_equality()
{
  TEST(jl_eq(JLSTR("Hello"), JLSTR("Hello"), false)->boolean.val == 1);
  TEST(jl_eq(JLNUM(2), JLNUM(2), false)->boolean.val == 1);
  TEST(jl_eq(JLNULL(), JLUNDEF(), false)->boolean.val == 1);
  TEST(jl_eq(JLNULL(), JLNULL(), false)->boolean.val == 1);
  TEST(jl_eq(JLUNDEF(), JLUNDEF(), false)->boolean.val == 1);
  TEST(jl_eq(JLSTR("3.14"), JLNUM(3.14), false)->boolean.val == 1);
  TEST(jl_eq(JLBOOL(1), JLBOOL(1), false)->boolean.val == 1);
  TEST(jl_eq(JLBOOL(1), JLNUM(1), false)->boolean.val == 1);

  JLVALUE *obj = JLOBJ();
  TEST(jl_eq(obj, obj, false)->boolean.val == 1);

  // NaN still doesn't equal NaN
  TEST(jl_eq(JLNAN(), JLNAN(), false)->boolean.val == 0);
}

void
test_strict_equality()
{
  // Mixed types are never strictly equal.
  TEST(jl_eq(JLNULL(), JLUNDEF(), true)->boolean.val == 0);
  TEST(jl_eq(JLSTR("3.14"), JLNUM(3.14), true)->boolean.val == 0);
  TEST(jl_eq(JLBOOL(1), JLNUM(1), true)->boolean.val == 0);

  TEST(jl_eq(JLBOOL(1), JLBOOL(1), true)->boolean.val == 1);
  TEST(jl_eq(JLNUM(42), JLNUM(42), true)->boolean.val == 1);
  TEST(jl_eq(JLNAN(), JLNAN(), true)->boolean.val == 0);

  JLVALUE *obj = JLOBJ();
  TEST(jl_eq(obj, obj, true)->boolean.val == 1);
}

void
test_inequality_returns_the_inverse_of_equality()
{
  TEST(jl_eq(JLSTR("Hello"), JLSTR("Hello"), false)->boolean.val == 1);
  TEST(jl_neq(JLSTR("Hello"), JLSTR("Hello"), false)->boolean.val == 0);

  TEST(jl_eq(JLNAN(), JLNAN(), true)->boolean.val == 0);
  TEST(jl_neq(JLNAN(), JLNAN(), true)->boolean.val == 1);
}

void
test_less_than_operator()
{
  TEST(jl_lt(JLNUM(2), JLNUM(4))->boolean.val == 1);
  TEST(jl_lt(JLNUM(1234132), JLNUM(3426423623))->boolean.val == 1);
}

void
test_greater_than_operator()
{
  TEST(jl_gt(JLNUM(4), JLNUM(1))->boolean.val == 1);
  TEST(jl_gt(JLNUM(123532234132), JLNUM(3426423623))->boolean.val == 1);
}

void
test_less_than_or_equal_to_operator()
{
}

void
test_greater_than_or_equal_to_operator()
{
}

int
main()
{
  TEST_START("Evaluation tests");

  test_add_handles_numbers();
  test_add_concatenates_strings();
  test_add_handles_mixed_types();
  test_sub_handles_numbers();
  test_sub_handles_non_numeric_types();
  test_mul_handles_numbers();
  test_mul_handles_non_numeric_types();
  test_div_handles_numbers();
  test_div_handles_non_numeric_types();
  test_mod_handles_numbers();
  test_mod_handles_non_numeric_types();
  test_equality();
  test_strict_equality();
  test_inequality_returns_the_inverse_of_equality();
  test_less_than_or_equal_to_operator();
  test_greater_than_or_equal_to_operator();

  TEST_SUMMARY();

  return 0;
}
