#include <iostream>
#include "templates.h"
#include "int.h"

typedef Fun<0, Ternary<
        IntCmpLe<Var<0>, Int<1>>,
        Int<1>,
        IntMul<
                Call<0, IntSub<Var<0>, Int<1>>>,
                Var<0>
        >
>> factorial_fun; // define function

typedef Context<End, FunList<factorial_fun, End>> ctx; // define context

typedef Eval<Call<0, Int<10>>, ctx>::value res; // calculate result

int main() {
    std::cout << ConvertToRuntimeValue<res>::value << std::endl;
    return 0;
}
