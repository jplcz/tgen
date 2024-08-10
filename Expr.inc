// clang-format off

#ifndef TGEN_EXPR
#define TGEN_EXPR(Id, Name, ArgCount)
#endif

TGEN_EXPR(0, Value, 0)
TGEN_EXPR(1, Add, 2)
TGEN_EXPR(2, Sub, 2)
TGEN_EXPR(3, Mul, 2)
TGEN_EXPR(4, Div, 2)
TGEN_EXPR(5, Mod, 2)
TGEN_EXPR(6, UnaryMinus, 1)
TGEN_EXPR(7, Identifier, 0)

#undef TGEN_EXPR
