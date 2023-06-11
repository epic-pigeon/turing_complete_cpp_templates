#ifndef TEMPLATES_H
#define TEMPLATES_H
#include <type_traits>

template<typename value> struct Eval {};
template<typename value> struct EvalCond {};


template<typename cond, typename if_true, typename if_false> struct If {};

template<bool cond, typename if_true, typename if_false> struct TernaryHelper {};
template<typename if_true, typename if_false> struct TernaryHelper<true, if_true, if_false> {
    typedef if_true value;
};
template<typename if_true, typename if_false> struct TernaryHelper<false, if_true, if_false> {
    typedef if_false value;
};


template<typename cond, typename if_true, typename if_false>
struct Eval<If<cond, if_true, if_false>> {
    typedef typename Eval<typename TernaryHelper<EvalCond<cond>::value, if_true, if_false>::value>::value value;
};

template<typename val>
struct ConvertToRuntimeValue {};

#ifndef NO_DEFINES

#define __decltype_start typename std::remove_cv<decltype(
#define __decltype_end )>::type
#define __decltype(v) __decltype_start v __decltype_end

#define eval(v) Eval<__decltype(v)>::value()

#define if_(cond) If<__decltype(cond), __decltype_start
#define else_ __decltype_end, __decltype_start
#define end __decltype_end>()

#define get_runtime(v) ConvertToRuntimeValue<Eval<__decltype(v)>::value>::value

#define __CONCATENATE(arg1, arg2)   __CONCATENATE1(arg1, arg2)
#define __CONCATENATE1(arg1, arg2)  __CONCATENATE2(arg1, arg2)
#define __CONCATENATE2(arg1, arg2)  arg1##arg2

#define __NARG(...) __NARG1(__VA_ARGS__, __SEQN())
#define __NARG1(...) __ARGN(__VA_ARGS__)
#define __ARGN(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define __SEQN() 8, 7, 6, 5, 4, 3, 2, 1, 0

#define __MAP0(mapf, sep)
#define __MAP1(mapf, sep, arg) mapf(arg, 0)
#define __MAP2(mapf, sep, arg, ...) mapf(arg, 1) sep() __MAP1(mapf, sep, __VA_ARGS__)
#define __MAP3(mapf, sep, arg, ...) mapf(arg, 2) sep() __MAP2(mapf, sep, __VA_ARGS__)
#define __MAP4(mapf, sep, arg, ...) mapf(arg, 3) sep() __MAP3(mapf, sep, __VA_ARGS__)
#define __MAP5(mapf, sep, arg, ...) mapf(arg, 4) sep() __MAP4(mapf, sep, __VA_ARGS__)
#define __MAP6(mapf, sep, arg, ...) mapf(arg, 5) sep() __MAP5(mapf, sep, __VA_ARGS__)
#define __MAP7(mapf, sep, arg, ...) mapf(arg, 6) sep() __MAP6(mapf, sep, __VA_ARGS__)
#define __MAP8(mapf, sep, arg, ...) mapf(arg, 7) sep() __MAP7(mapf, sep, __VA_ARGS__)

#define __SEP_COMMA() ,
#define __MAP_HELPER(n, mapf, sep, ...) __CONCATENATE(__MAP, n)(mapf, sep, __VA_ARGS__)
#define __MAP(mapf, sep, ...) __MAP_HELPER(__NARG(__VA_ARGS__), mapf, sep, __VA_ARGS__)

#define __MAP_TYPENAME(v, i) typename __CONCATENATE(__T, i)
#define __MAP_IDX(v, i) __CONCATENATE(__T, i)
#define __MAP_CONSTEXPR(v, i) static constexpr auto v = __CONCATENATE(__T, i)();

#define forward_def_fun(name, ...) \
template<__MAP(__MAP_TYPENAME, __SEP_COMMA, __VA_ARGS__)> struct __ ## name ## _struct; \
template<__MAP(__MAP_TYPENAME, __SEP_COMMA, __VA_ARGS__)> \
constexpr __ ## name ## _struct<__MAP(__MAP_IDX, __SEP_COMMA, __VA_ARGS__)>\
name(__MAP(__MAP_IDX, __SEP_COMMA, __VA_ARGS__));

#define def_fun(name, ...) \
template<__MAP(__MAP_TYPENAME, __SEP_COMMA, __VA_ARGS__)> struct __ ## name ## _struct {}; \
template<__MAP(__MAP_TYPENAME, __SEP_COMMA, __VA_ARGS__)> \
constexpr __ ## name ## _struct<__MAP(__MAP_IDX, __SEP_COMMA, __VA_ARGS__)>\
name(__MAP(__MAP_IDX, __SEP_COMMA, __VA_ARGS__)) { return {}; } \
template<__MAP(__MAP_TYPENAME, __SEP_COMMA, __VA_ARGS__)>\
struct Eval<__ ## name ## _struct<__MAP(__MAP_IDX, __SEP_COMMA, __VA_ARGS__)>> { \
    __MAP(__MAP_CONSTEXPR, __SEP_COMMA, __VA_ARGS__) \
    typedef typename Eval<__decltype_start

#define end_fun __decltype_end>::value value; };

#define def_var(name, val) \
struct __ ## name ## _struct {}; \
constexpr auto name = __ ## name ## _struct(); \
template<> struct Eval<__ ## name ## _struct> { \
    typedef typename Eval<__decltype(val)>::value value; \
};

#endif


#endif //TEMPLATES_H
