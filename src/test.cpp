#include <iostream>
#include <sstream>
#include "cShare.h"

bool test()
{
    cShare S;
    std::istringstream input(
        "p c\n"
        "r 100\n"
        "r 100\n"
        "v 0 0 1\n"
        "v 1 0 2\n"
        "v 0 1 5\n"
        "v 1 1 6\n"
        "c 0 10\n"
        "c 1 5\n");
    parseInput( S, input );
    solve(S);
    auto va = S.getAssign();

    // both consumers should be assigned thair maximum capacity
    if( va.size() != 2 )
        return false;
    if( va[0].value != 5 )
        return false;
    if( va[1].value != 10 )
        return false;
    std::cout << text(S);
    return true;
}