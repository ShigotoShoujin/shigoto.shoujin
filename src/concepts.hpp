#pragma once
#include <type_traits>

namespace shoujin {

// clang does not yet support c++20 concepts
// clang-format off

template<typename T> concept fundamental = std::is_fundamental_v<T>;

}
