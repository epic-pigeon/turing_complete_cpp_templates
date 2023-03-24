#ifndef TEMPLATES_INT_H
#define TEMPLATES_INT_H
#include "templates.h"

typedef int int_t;

template<int_t value> struct Int {};

template<int v>
struct ConvertToRuntimeValue<Int<v>> {
    static constexpr int_t value = v;
};

template<int_t _value, typename context>
struct Eval<Int<_value>, context> {
    typedef Int<_value> value;
};

#define DEFINE_OP(NAME, OP) \
template<typename left, typename right> struct NAME {}; \
template<typename left, typename right, typename context> \
struct Eval<NAME<left, right>, context> { \
private: \
    typedef typename Eval<left, context>::value left_val; \
    typedef typename Eval<right, context>::value right_val; \
public: \
    typedef Int<(ConvertToRuntimeValue<left_val>::value OP ConvertToRuntimeValue<right_val>::value)> value; \
};

#define DEFINE_COND_OP(NAME, OP) \
template<typename left, typename right> struct NAME {}; \
template<typename left, typename right, typename context> \
struct EvalCond<NAME<left, right>, context> { \
private: \
    typedef typename Eval<left, context>::value left_val; \
    typedef typename Eval<right, context>::value right_val; \
public: \
    static constexpr bool value = ConvertToRuntimeValue<left_val>::value OP ConvertToRuntimeValue<right_val>::value; \
};

DEFINE_OP(IntAdd, +)
DEFINE_OP(IntSub, -)
DEFINE_OP(IntMul, *)
DEFINE_OP(IntDiv, /)
DEFINE_COND_OP(IntCmpL, <)
DEFINE_COND_OP(IntCmpLe, <=)
DEFINE_COND_OP(IntCmpE, ==)
DEFINE_COND_OP(IntCmpG, >)
DEFINE_COND_OP(IntCmpGe, >=)

#endif //TEMPLATES_INT_H
