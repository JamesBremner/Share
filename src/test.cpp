#include <iostream>
#include <sstream>
#include "cShare.h"

bool test1()
{
    cShare S;
    // 'unlimited' resource quantities
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
    parseInput(S, input);
    solve(S);
    auto va = S.getAssign();

    // both consumers should be assigned thair maximum capacity
    if (va.size() != 2)
        return false;
    if (va[0].value != 5)
        return false;
    if (va[1].value != 10)
        return false;
    return true;
}

bool test2()
{
    cShare S;
    // limited resources
    std::istringstream input(
        "p c\n"
        "r 5\n"
        "r 2\n"
        "r 1\n"
        "v 0 0 2\n"
        "v 0 1 3\n"
        "v 0 2 1\n"
        "v 1 0 1\n"
        "v 1 1 2\n"
        "v 1 2 3\n"
        "c 0 15\n"
        "c 1 10\n");
    parseInput(S, input);
    solve(S);
    auto va = S.getAssign();

    // all available resources assigned
    if (va.size() != 2)
        return false;
    if (va[0].value != 5)
        return false;
    if (va[1].value != 2)
        return false;

    return true;
}
bool test()
{
    if (!test1())
        return false;
    if (!test2())
        return false;
    return true;
}