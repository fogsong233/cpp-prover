#ifndef plus_H
#define plus_H
#include "../logic.h"
#include "../util.h"
#include "nat.h"

namespace Prover::Nat {
// not impl
template <isNat A, isNat B>
struct Plus {
    using ty = Zero;
};

template <isNat A>
struct Plus<Zero, A> {
    using ty = A;
};

template <isNat A, isNat B>
struct Plus<Succ<B>, A> {
    using ty = Succ<typename Plus<B, A>::ty>;
};

template <isNat N>
struct ZeroPlusAxiom : Prop {
    constexpr Eq<Plus<Zero, N>, N> operator() () {
        return {};
    }
};

template <isNat N, isNat M>
struct SuccPlusAxiom : Prop {
    constexpr Eq<Plus<Succ<N>, M>, Succ<Plus<N, M>>> operator() () {
        return {};
    }
};

}  // namespace Prover::Nat
#endif
