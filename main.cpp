#include <iostream>
#include "templates.h"
#include "int.h"
#include "list.h"

#define factorial_fun 0
#define n var(0)

typedef fun(0)
    if_(n <= int_const(0))
        int_const(1)
    else_
        n * call<factorial_fun>(n - int_const(1))
    end
endfun factorial_fun_t;

#undef n

typedef Context<VarList<>, FunList<factorial_fun_t>> ctx; // define context

constexpr auto res = eval(call<factorial_fun>(int_const(10)), ctx);

int main() {
    std::cout << get_runtime(res) << std::endl;
    return 0;
}
