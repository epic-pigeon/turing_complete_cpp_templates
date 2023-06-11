#include <iostream>
#include "templates.h"
#include "int.h"
#include "list.h"

def_var(it_count, ic(10))

def_fun(factorial, x)
    if_(x <= ic(0))
        ic(1)
    else_
        x * factorial(x - ic(1))
    end
end_fun

constexpr auto res = factorial(it_count);

int main() {
    std::cout << get_runtime(res) << std::endl;
    return 0;
}
