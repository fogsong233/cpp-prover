#ifndef ty_H
#define ty_H

#include "proof.h"
#include <memory>
#include <variant>

namespace Prover {

struct Nat;

struct Zero {};

struct Succ {
    std::shared_ptr<Nat> pred;
};

struct Nat {
    std::variant<Zero, Succ> self;

    constexpr Nat(Zero zero) : self(zero) {}

    constexpr Nat(Succ succ) : self(succ) {}
};

constexpr Nat succ(Nat nat) {
    if(std::holds_alternative<Succ>(nat.self)) {
        return Succ{std::make_shared<Nat>(std::get<Succ>(nat.self))};
    } else {
        return Succ{std::make_shared<Nat>(std::get<Zero>(nat.self))};
    }
}

constexpr Nat zero() {
    return Nat(Zero{});
}

constexpr int toInt(const Nat& nat) {
    return cases(
        nat.self,
        [](Zero) { return 0; },
        [](const Succ& succ) { return 1 + toInt(*succ.pred); });
}
}  // namespace Prover
#endif  // ty_H
