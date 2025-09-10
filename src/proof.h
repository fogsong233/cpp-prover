#ifndef proof_H
#define proof_H

#include "logic.h"
#include <type_traits>
#include <utility>
#include <variant>

namespace Prover {
template <typename... Fs>
struct overloaded : Fs... {
    using Fs::operator()...;

    constexpr overloaded(Fs... fs) : Fs(fs)... {}
};

template <typename... Fs>
overloaded(Fs...) -> overloaded<Fs...>;

using Bool = std::variant<True, False>;

template <typename T, typename... Fs>
constexpr auto cases(T t, Fs... fs) {
    return std::visit(overloaded(fs...), t);
}

template <class L, class R>
constexpr True rfl() {
    return True{};
}

constexpr True admit() {
    return True{};
}

template <class I, class Fn>
constexpr auto apply(Fn fn, I i) {
    return fn(i);
}
}  // namespace Prover

#endif  // proof_H
