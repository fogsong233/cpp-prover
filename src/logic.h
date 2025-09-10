#ifndef logic_H
#define logic_H

#include <type_traits>

// all in Prop
namespace Prover {

struct Prop {};

template <typename T>
concept isProp = std::is_base_of_v<Prop, T>;

struct True : Prop {};

struct False : Prop {};

template <typename A, typename B>
struct And : Prop {};

template <typename A, typename B>
struct Or : Prop {};

template <typename A>
struct Not : Prop {};

template <typename A, typename B>
constexpr Or<A, B> makeOr(A /*unused*/) {
    return Or<A, B>{};
}

template <isProp A, isProp B>
constexpr auto orCases(Or<A, B> /*unused*/, auto fA, auto fB) {
    auto retA = fA(A{});
    auto retB = fB(B{});
    static_assert(std::is_same_v<decltype(retA), decltype(retB)>);
    return retA;
}

template <isProp A, isProp B>
constexpr And<A, B> makeAnd(A a, B b) {

    return And<A, B>{};
}

template <isProp A, isProp B>
constexpr A deAndL(And<A, B> /*unused*/) {
    return A{};
}

template <isProp A, isProp B>
constexpr B deAndR(And<A, B> /*unused*/) {
    return B{};
}

template <isProp A>
struct dNot {
    constexpr static auto operator() (A /*unused*/) {
        return A{};
    }
};

template <isProp A>
struct dNot<Not<Not<A>>> {
    constexpr static auto operator() (Not<Not<A>> /*unused*/) {
        return A{};
    }
};

template <isProp A, isProp B, template <typename, typename> typename T>
    requires isProp<T<A, B>>
struct dNot<T<A, B>> {
    constexpr static auto operator() (T<A, B> /*unused*/) {
        return T<decltype(dNot<A>(A{})), decltype(dNot<B>(B{}))>{};
    }
};

template <class A, class B>
struct Eq : Prop {};

template <class T>
constexpr Eq<T, T> rfl() {

    return {};
}

template <class T, class P, class Q>
constexpr Eq<T, Q> transitivity(Eq<T, P> /*unused*/, Eq<P, Q> /*unused*/) {
    return {};
}

template <class A, class B>
constexpr Eq<B, A> symm(Eq<A, B> /*unused*/) {
    return {};
}

struct Path {};

template <class Inner>
struct L : Path {};

template <class Inner>
struct R : Path {};

struct Here : Path {};

template <class P>
concept isPath = std::is_base_of_v<Path, P>;

template <isPath P, class Bef, class Nxt>
struct followPath {
    using ty = Bef;
    using after = Nxt;
};

template <isPath P, template <typename, typename> class T, class A, class B, class Nxt>
struct followPath<L<P>, T<A, B>, Nxt> {
    using ty = followPath<P, A, Nxt>::ty;
    using after = T<typename followPath<P, A, Nxt>::after, B>;
};

template <isPath P, template <typename, typename> class T, class A, class B, class Nxt>
struct followPath<R<P>, T<A, B>, Nxt> {
    using ty = followPath<P, B, Nxt>::ty;
    using after = T<A, typename followPath<P, B, Nxt>::after>;
};

template <isPath P, template <typename> class T, class A, class Nxt>
struct followPath<L<P>, T<A>, Nxt> {
    using ty = followPath<P, A, Nxt>::ty;
    using after = T<typename followPath<P, A, Nxt>::after>;
};

template <class A, class Nxt>
struct followPath<Here, A, Nxt> {
    using ty = A;
    using after = Nxt;
};

template <class Raw, class L, class R, isPath P>
    requires std::is_same_v<typename followPath<P, Raw, R>::ty, L>
struct RwImpl {
    using Matched = followPath<P, Raw, R>::ty;
    using Ret = followPath<P, Raw, R>::after;
};

template <class Raw, class L, class R, isPath P>
    requires std::is_same_v<typename followPath<P, Raw, R>::ty, L>
constexpr RwImpl<Raw, L, R, P>::Ret rw(Raw raw, Eq<L, R> eq, P path) {
    return {};
}

}  // namespace Prover

#endif  // logic_H
