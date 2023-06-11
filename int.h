#ifndef TEMPLATES_INT_H
#define TEMPLATES_INT_H
#include "templates.h"

typedef int int_t;

template<int_t value> struct Int {};

template<int v>
struct ConvertToRuntimeValue<Int<v>> {
    static constexpr int_t value = v;
};

template<int_t _value>
struct Eval<Int<_value>> {
    typedef Int<_value> value;
};

#define DEFINE_OP(NAME, OP) \
template<typename left, typename right> struct NAME {}; \
template<typename left, typename right> \
struct Eval<NAME<left, right>> { \
private: \
    typedef typename Eval<left>::value left_val; \
    typedef typename Eval<right>::value right_val; \
public: \
    typedef Int<(ConvertToRuntimeValue<left_val>::value OP ConvertToRuntimeValue<right_val>::value)> value; \
};                          \
template<typename left, typename right> NAME<left, right> operator OP(left l, right r) { return {}; }

#define DEFINE_COND_OP(NAME, OP) \
template<typename left, typename right> struct NAME {}; \
template<typename left, typename right> \
struct EvalCond<NAME<left, right>> { \
private: \
    typedef typename Eval<left>::value left_val; \
    typedef typename Eval<right>::value right_val; \
public: \
    static constexpr bool value = ConvertToRuntimeValue<left_val>::value OP ConvertToRuntimeValue<right_val>::value; \
}; \
template<typename left, typename right> NAME<left, right> operator OP(left l, right r) { return {}; }

DEFINE_OP(IntAdd, +)
DEFINE_OP(IntSub, -)
DEFINE_OP(IntMul, *)
DEFINE_OP(IntDiv, /)
DEFINE_COND_OP(IntCmpL, <)
DEFINE_COND_OP(IntCmpLe, <=)
DEFINE_COND_OP(IntCmpE, ==)
DEFINE_COND_OP(IntCmpG, >)
DEFINE_COND_OP(IntCmpGe, >=)

#ifndef NO_DEFINES
#define ic(v) Int<v>()
#endif

#endif //TEMPLATES_INT_H
