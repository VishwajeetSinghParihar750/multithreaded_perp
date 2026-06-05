#pragma once

// for using std::excecpted directly, without checks everywhere
#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

#define ASSIGN_OR_RETURN(name, expr)                                    \
    auto CONCAT(tmp2388_89df, __LINE__) = expr;                         \
    if (!CONCAT(tmp2388_89df, __LINE__).has_value())                    \
        return std::unexpected(CONCAT(tmp2388_89df, __LINE__).error()); \
    auto name = *CONCAT(tmp2388_89df, __LINE__);
