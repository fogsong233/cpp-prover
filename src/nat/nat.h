#ifndef nat_H
#define nat_H

namespace Prover::Nat {
struct Zero {};

template <class T>
struct Succ {};

template <typename T>
struct _IsNat {
    constexpr static bool value = false;
};

template <>
struct _IsNat<Zero> {
    constexpr static bool value = true;
};

template <typename T>
struct _IsNat<Succ<T>> {
    constexpr static bool value = true;
};

template <typename T>
concept isNat = _IsNat<T>::value;

using One = Succ<Zero>;
using Two = Succ<One>;
using Three = Succ<Two>;
using Four = Succ<Three>;
using Five = Succ<Four>;
using Six = Succ<Five>;
using Seven = Succ<Six>;
using Eight = Succ<Seven>;
using Nine = Succ<Eight>;
using Ten = Succ<Nine>;
}  // namespace Prover::Nat

#endif  // nat_H
