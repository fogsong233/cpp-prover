#ifndef logic_H
#define logic_H

namespace Prover {

struct True {};

struct False {};

template <typename A, typename B>
struct And {};

template <typename A, typename B>
struct Or {};

template <typename A>
struct Not {};

}  // namespace Prover
#endif  // logic_H
