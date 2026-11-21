#pragma once

#include <functional>

namespace prgmr {

template <typename Signature>
using delegate = std::function<Signature>;

} // namespace prgmr
