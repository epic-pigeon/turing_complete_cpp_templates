#ifndef TEMPLATES_H
#define TEMPLATES_H

typedef const int name_t;

struct End {};
template<name_t var_name, typename var_value> struct VarD {};
template<typename ...vars> struct VarList {};
template<name_t fun_name, typename fun_value> struct Fun {};
template<typename ...vars> struct FunList {};


template<typename var_list, typename fun_list> struct Context {};

template<typename context, name_t var_name> struct FindVar {};

template<typename var_value, typename ...var_list, typename fun_list, name_t var_name>
struct FindVar<Context<VarList<VarD<var_name, var_value>, var_list...>, fun_list>, var_name> {
    typedef var_value value;
};

template<typename var, typename ...var_list, typename fun_list, name_t var_name>
struct FindVar<Context<VarList<var, var_list...>, fun_list>, var_name> {
    typedef typename FindVar<Context<VarList<var_list...>, fun_list>, var_name>::value value;
};

template<typename context, name_t fun_name> struct FindFun {};

template<typename fun_value, typename var_list, typename ...fun_list, name_t fun_name>
struct FindFun<Context<var_list, FunList<Fun<fun_name, fun_value>, fun_list...>>, fun_name> {
    typedef fun_value value;
};

template<typename fun, typename var_list, typename ...fun_list, name_t var_name>
struct FindFun<Context<var_list, FunList<fun, fun_list...>>, var_name> {
    typedef typename FindVar<Context<var_list, FunList<fun_list...>>, var_name>::value value;
};

template<typename value, typename context> struct Eval {};

template<name_t var_name> struct Var {};

template<name_t var_name, typename ctx_var_value, typename ...ctx_var_rest, typename ctx_fun>
struct Eval<Var<var_name>, Context<VarList<VarD<var_name, ctx_var_value>, ctx_var_rest...>, ctx_fun>> {
    typedef ctx_var_value value;
};

template<name_t var_name, typename vard, typename ...ctx_var_rest, typename ctx_fun>
struct Eval<Var<var_name>, Context<VarList<vard, ctx_var_rest...>, ctx_fun>> {
    typedef typename Eval<Var<var_name>, Context<VarList<ctx_var_rest...>, ctx_fun>>::value value;
};

template<name_t var_name, typename var_value, typename value> struct LetIn {};

template<name_t var_name, typename var_value, typename _value, typename ...ctx_var, typename ctx_fun>
struct Eval<LetIn<var_name, var_value, _value>, Context<VarList<ctx_var...>, ctx_fun>> {
    typedef typename Eval<_value,
            Context<
                    VarList<
                            VarD<var_name, typename Eval<var_value, Context<VarList<ctx_var...>, ctx_fun>>::value>,
                            ctx_var...
                    >,
                    ctx_fun
            >
    >::value value;
};

template<name_t fun_name, typename ...args> struct Call {};
template<typename context, typename prev, int count, typename ...args> struct CallVarList {};
template<typename context, typename prev, int count> struct CallVarList<context, prev, count> {
    typedef prev list;
};
template<typename context, typename ...vars, int count, typename arg, typename ...args>
struct CallVarList<context, VarList<vars...>, count, arg, args...> {
    typedef typename CallVarList<context, VarList<vars..., VarD<count, typename Eval<arg, context>::value>>, count + 1, args...>::list list;
};

template<name_t fun_name, typename ...args, typename var_list, typename fun_list>
struct Eval<Call<fun_name, args...>, Context<var_list, fun_list>> {
    typedef typename Eval<
            typename FindFun<Context<var_list, fun_list>, fun_name>::value,
            Context<typename CallVarList<Context<var_list, fun_list>, VarList<>, 0, args...>::list, fun_list>
    >::value value;
};

template<typename value, typename context> struct EvalCond {};

template<typename cond, typename if_true, typename if_false> struct Ternary {};

template<bool cond, typename if_true, typename if_false> struct TernaryHelper {};
template<typename if_true, typename if_false> struct TernaryHelper<true, if_true, if_false> {
    typedef if_true value;
};
template<typename if_true, typename if_false> struct TernaryHelper<false, if_true, if_false> {
    typedef if_false value;
};

template<typename cond, typename if_true, typename if_false, typename context>
struct Eval<Ternary<cond, if_true, if_false>, context> {
    typedef typename Eval<typename TernaryHelper<EvalCond<cond, context>::value, if_true, if_false>::value, context>::value value;
};

template<typename val>
struct ConvertToRuntimeValue {};

#endif //TEMPLATES_H
