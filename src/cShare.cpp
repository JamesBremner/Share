#include <fstream>
#include <sstream>
#include <algorithm>

#include "cShare.h"

int cShare::value(int r, int c) const
{
    auto it = myValueLinear.find(std::make_pair(r, c));
    if (it == myValueLinear.end())
        return 0;
    return it->second;
}
int cShare::assign(int r, int c) const
{
    auto it = myAssign.find(std::make_pair(r, c));
    if (it == myAssign.end())
        return 0;
    return it->second;
}
int cShare::consumerCount() const
{
    int count = 0;
    for (auto m : myValueLinear)
    {
        if (m.first.second > count)
            count = m.first.second;
    }
    return count + 1;
}

cShare::rcv_t cShare::bestValueLinear() const
{
    rcv_t best;
    best.second = 0;
    for (auto &rcv : myValueLinear)
        if (rcv.second > best.second)
        {
            if (myResourceTotalQuantity[rcv.first.first])
                best = rcv;
        }
    return best;
}

void readFile(
    cShare &S,
    const std::string &fn)
{
    S.clear();
    std::ifstream ifs(fn);
    if (!ifs.is_open())
        throw std::runtime_error(
            "Cannot open " + fn);
    std::string ltype;
    while (1)
    {
        ifs >> ltype;
        if (!ifs.good())
            break;
        if (ltype[0] == 'p')
        {
            ifs >> ltype;
            if( ltype[0] == 'l' )
                S.setProblemType(cShare::eProblem::linear);
            else
                throw std::runtime_error(
                    "Unrecognized problem type " + ltype                );
        }
        else if (ltype[0] == 'r')
        {
            int q;
            ifs >> q;
            S.addResourceTotalQuantity(q);
        }
        else if (ltype[0] == 'v')
        {
            int r, c, v;
            ifs >> r >> c >> v;
            S.addValueLinear(r, c, v);
        }
    }
}

void solve(cShare& S )
{
    switch( S.myProblemType )
    {
        case cShare::eProblem::linear:
        solve1(S);
        break;
    }
}

void solve1(cShare &S)
{
    while (1)
    {
        auto best = S.bestValueLinear();
        if (!best.second)
            break;
        best.second = S.resourceQuantity(best.first.first);
        S.addAssign(best);
        S.subResourceTotalQuantity(best);
    }
}
std::string text(const cShare &S)
{
    std::stringstream ss;
    ss << S.resourceCount() << " resources, "
       << S.consumerCount() << " consumers\n";
    ss << "\nResource Quantity\n";
    for (int k = 0; k < S.resourceCount(); k++)
        ss << k << "\t" << S.resourceQuantity(k) << "\n";
    ss << "\nValues\nResource\tConsumer\tValue / Unit\n";
    for (int r = 0; r < S.resourceCount(); r++)
        for (int c = 0; c < S.consumerCount(); c++)
        {
            int v = S.value(r, c);
            if (v > 0)
                ss << "    " << r
                   << "\t\t" << c
                   << "\t\t" << v << "\n";
        }
    ss << "\nAssignments\nResource\tConsumer\tUnits Assigned\n";
    int totalValue = 0;
    for (int r = 0; r < S.resourceCount(); r++)
        for (int c = 0; c < S.consumerCount(); c++)
        {
            int a = S.assign(r, c);
            if (!a)
                continue;
            int v = a * S.value(r, c);
            totalValue += v;
            ss << "    " << r
               << "\t\t" << c
               << "\t\t" << a
               << "\t\t" << v
               << "\n";
        }
    ss << "Total Value " << totalValue << "\n";
    return ss.str();
}