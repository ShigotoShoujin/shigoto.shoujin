#pragma once
#include <concepts>

namespace shoujin::concepts {
template<typename T>
concept pointer = std::is_pointer_v<T>;
}
