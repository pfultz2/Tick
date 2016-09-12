#ifndef GUARD_TEST_H
#define GUARD_TEST_H

#include <type_traits>
#include <tick/detail/pp.h>
#include <iostream>
#include <functional>
#include <vector>

namespace tick { namespace test {
typedef std::function<void()> test_case;
static std::vector<test_case> test_cases;

struct auto_register
{
    auto_register(test_case tc)
    {
        test_cases.push_back(tc);
    }
};

#define TICK_DETAIL_TEST_CASE(name) \
struct name \
{ void operator()() const; }; \
static tick::test::auto_register TICK_PP_CAT(name, _register) = tick::test::auto_register(name()); \
void name::operator()() const
}}

#define STATIC_ASSERT_SAME(...) static_assert(std::is_same<__VA_ARGS__>::value, "Types are not the same")
#define TICK_TEST_CASE() TICK_DETAIL_TEST_CASE(TICK_PP_CAT(test_, __LINE__))
#define TICK_TEST_CHECK(...) if (!(__VA_ARGS__)) std::cout << "*****FAILED: " << #__VA_ARGS__ << "@" << __FILE__ << ": " << __LINE__ << std::endl
#define TICK_STATIC_TEST_CASE() struct TICK_PP_CAT(test_, __LINE__)

#define TICK_TEST_TEMPLATE(...) typedef std::integral_constant<int, sizeof(__VA_ARGS__)> TICK_PP_CAT(test_template_, __LINE__)


int main()
{
    for(const auto& tc: tick::test::test_cases) tc();
    return 0;
}
 
#endif
