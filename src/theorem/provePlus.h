#ifndef provePlus_H
#define provePlus_H
#include "../logic.h"
#include "../nat/nat.h"
#include "../nat/plus.h"
#include <type_traits>

using namespace Prover;

template <Nat::isNat A, Nat::isNat B>
struct PlusCommTh : Prop {
    constexpr static Eq<Nat::Plus<A, B>, Nat::Plus<B, A>> operator() () {
        throw "not proved!";
    }
};

template <>
struct PlusCommTh<Nat::Zero, Nat::Zero> : Prop {
    constexpr Eq<Nat::Plus<Nat::Zero, Nat::Zero>, Nat::Plus<Nat::Zero, Nat::Zero>> operator() () {
        using namespace Nat;
        using namespace Prover;
        return rfl<Plus<Zero, Zero>>();
    }
};

template <Nat::isNat N>
struct PlusCommTh<Nat::Succ<N>, Nat::Zero> : Prop {
    constexpr Eq<Nat::Plus<Nat::Succ<N>, Nat::Zero>, Nat::Plus<Nat::Zero, Nat::Succ<N>>>
        operator() () {
        using namespace Nat;
        using namespace Prover;

        // 1) Plus<Succ<N>, Zero> = Succ<Plus<N, Zero>>
        Eq<Plus<Succ<N>, Zero>, Succ<Plus<N, Zero>>> e1 = SuccPlusAxiom<N, Zero>()();

        // 2) 归纳假设：Plus<N, Zero> = Plus<Zero, N>
        Eq<Plus<N, Zero>, Plus<Zero, N>> ih = PlusCommTh<N, Zero>()();

        // 3) Plus<Zero, N> = N
        Eq<Plus<Zero, N>, N> zpn = ZeroPlusAxiom<N>()();

        // 4) 合成得到：Plus<N,Zero> = N
        auto plusNzero_eq_N = transitivity<Plus<N, Zero>, Plus<Zero, N>, N>(ih, zpn);

        // 5) 在 Succ 构造器内部把 Plus<N,Zero> 替换为 N
        Eq<Plus<Succ<N>, Zero>, Succ<N>> succ_rewritten =
            rw<Eq<Plus<Succ<N>, Zero>, Succ<Plus<N, Zero>>>, Plus<N, Zero>, N, R<L<Here>>>(
                e1,
                plusNzero_eq_N,
                R<L<Here>>{});

        // 6) ZeroPlusAxiom<Succ<N>>() : Eq<Plus<Zero, Succ<N>>, Succ<N>>
        Eq<Plus<Zero, Succ<N>>, Succ<N>> zps_succ = ZeroPlusAxiom<Succ<N>>()();

        // 7) 对 zps_succ 做对称
        auto succ_to_plus0succ = symm(zps_succ);

        // 8) 合成所有步骤得到目标等式

        auto full =
            transitivity<Plus<Succ<N>, Zero>, Succ<N>, Plus<Zero, Succ<N>>>(succ_rewritten,
                                                                            succ_to_plus0succ);

        return full;
    }
};

template <Nat::isNat N>
struct PlusCommTh<Nat::Zero, Nat::Succ<N>> : Prop {
    constexpr Eq<Nat::Plus<Nat::Zero, Nat::Succ<N>>, Nat::Plus<Nat::Succ<N>, Nat::Zero>>
        operator() () {
        using namespace Nat;
        using namespace Prover;
        return symm(PlusCommTh<Succ<N>, Zero>()());  // 修复：添加了 ()()
    }
};

template <Nat::isNat A, Nat::isNat B>
struct PlusCommTh<Nat::Succ<A>, Nat::Succ<B>> : Prop {
    constexpr Eq<Nat::Plus<Nat::Succ<A>, Nat::Succ<B>>, Nat::Plus<Nat::Succ<B>, Nat::Succ<A>>>
        operator() () {
        using namespace Nat;
        using namespace Prover;

        // 左边展开：Plus<Succ<A>, Succ<B>> = Succ<Plus<A, Succ<B>>>
        Eq<Plus<Succ<A>, Succ<B>>, Succ<Plus<A, Succ<B>>>> e1 =
            SuccPlusAxiom<A, Succ<B>>()();  // 修复：添加了 ()()

        // 归纳假设：Plus<A, Succ<B>> = Plus<Succ<B>, A>
        Eq<Plus<A, Succ<B>>, Plus<Succ<B>, A>> ih = PlusCommTh<A, Succ<B>>()();

        // 在 Succ 构造器内部把 Plus<A, Succ<B>> 替换为 Plus<Succ<B>, A>
        Eq<Plus<Succ<A>, Succ<B>>, Succ<Plus<Succ<B>, A>>> succ_rewritten =
            rw<Eq<Plus<Succ<A>, Succ<B>>, Succ<Plus<A, Succ<B>>>>,
               Plus<A, Succ<B>>,
               Plus<Succ<B>, A>,
               R<L<Here>>>(e1, ih, R<L<Here>>{});

        // Eq<Plus<Succ<A>, Succ<B>>, Succ<Plus<Succ<B>, A>>>
        // -> Eq<Plus<Succ<A>, Succ<B>>, Succ<Succ<Plus<B, A>>>>
        auto h3 = rw(succ_rewritten, SuccPlusAxiom<B, A>()(), R<L<Here>>{});
        // -> Eq<Plus<Succ<A>, Succ<B>>, Succ<Succ<Plus<A, B>>>>
        auto h4 = rw(h3, PlusCommTh<B, A>()(), R<L<L<Here>>>{});
        // -> Eq<Plus<Succ<A>, Succ<B>>, Succ<Plus<Succ<A>, B>>>
        auto h5 = rw(h4, symm(SuccPlusAxiom<A, B>()()), R<L<Here>>{});
        // -> Eq<Plus<Succ<A>, Succ<B>>, Succ<Plus<B, Succ<A>>>>
        auto h6 = rw(h5, PlusCommTh<Succ<A>, B>()(), R<L<Here>>{});
        // -> Eq<Plus<Succ<A>, Succ<B>>, Plus<Succ<B>, Succ<A>>>
        auto h7 = rw(h6, symm(SuccPlusAxiom<B, Succ<A>>()()), R<Here>{});
        return h7;
    }
};

#endif  // provePlus_H
