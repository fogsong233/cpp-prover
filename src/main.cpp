#include "nat/nat.h"
#include "theorem/provePlus.h"

void exampleProve() {
    using namespace Prover;
}

int main(int argc, char** argv) {
    using namespace Prover;
    using namespace Prover::Nat;
    auto proof = PlusCommTh<Three, Six>()();
    return 0;
}
