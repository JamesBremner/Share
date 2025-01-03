#include <fstream>
#include <sstream>
#include <algorithm>

#include "cShare.h"

void cShare::setConsumerCapacity(int c, int v)
{
    switch (myProblemType)
    {
    case eProblem::consumer_capacity_resource_units:
        break;
    default:
        throw std::runtime_error(
            "setConsumerCapacity wrong problem type");
    }
    if (c >= myConsumers.size())
        myConsumers.resize(c + 1);
    myConsumers[c].myCapacityAllResources = v;
}
int cShare::getConsumerCapacity(int c) const
{
    if (0 > c || c >= myConsumers.size())
        return 0;
    return myConsumers[c].myCapacityAllResources;
}
int cShare::getConsumerCapacityInit(int c) const
{
    if (0 > c || c >= myConsumers.size())
        return 0;
    return myInitConsumers[c].myCapacityAllResources;
}
int cShare::value(int r, int c) const
{
    srcv target(r, c, 0);
    for (auto &rcv : myValueLinear)
        if (rcv == target)
            return rcv.value;
    return 0;
}
int cShare::assign(int r, int c) const
{
    srcv target(r, c, 0);
    for (auto &rcv : myAssign)
        if (rcv == target)
            return rcv.value;
    return 0;
}

int cShare::consumerCount() const
{
    return myConsumers.size();
}

// cShare::rcv_t cShare::bestValueLinear() const
// {
//     rcv_t best;
//     best.second = 0;
//     for (auto &rcv : myValueLinear)
//         if (rcv.second > best.second)
//         {
//             if (myResourceTotalQuantity[rcv.first.first])
//                 best = rcv;
//         }
//     return best;
// }

cShare::vrcv_t cShare::sortValue()
{
    vrcv_t ret = myValueLinear;

    std::sort(
        ret.begin(), ret.end(),
        [](const srcv &a, const srcv &b)
        {
            return a.value > b.value;
        });
    return ret;
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
    parseInput(S, ifs);
}
void parseInput(
    cShare &S,
    std::istream &input)
{
    std::string ltype;
    while (1)
    {
        input >> ltype;
        if (!input.good())
            break;
        if (ltype[0] == 'p')
        {
            input >> ltype;
            if (ltype[0] == 'l')
                S.setProblemType(cShare::eProblem::linear);
            else if (ltype[0] == 'c')
                S.setProblemType(cShare::eProblem::consumer_capacity_resource_units);
            else
                throw std::runtime_error(
                    "Unrecognized problem type " + ltype);
        }
        else if (ltype[0] == 'r')
        {
            int q;
            input >> q;
            S.addResourceTotalQuantity(q);
        }
        else if (ltype[0] == 'v')
        {
            int r, c, v;
            input >> r >> c >> v;
            S.addValueLinear(r, c, v);
        }
        else if (ltype[0] == 'c')
        {
            int c, v;
            input >> c >> v;
            S.setConsumerCapacity(c, v);
        }
    }

    S.initBackup();
}

void solve(cShare &S)
{
    switch (S.myProblemType)
    {
    case cShare::eProblem::linear:
        solve1(S);
        break;
    case cShare::eProblem::consumer_capacity_resource_units:
        solve2(S);
        break;
    }
}

 void solve1(cShare &S)
 {
//     while (1)
//     {
//         auto best = S.bestValueLinear();
//         if (!best.second)
//             break;
//         best.second = S.resourceQuantity(best.first.first);
//         S.addAssign(best);
//         S.subResourceTotalQuantity(best);
//     }
 }
void solve2(
    cShare &S)
{
    auto sv = S.sortValue();
    for( auto& rcv : sv )
    {
        int q = S.resourceQuantity(rcv.resource);
        if( q < rcv.value )
            rcv.value = q;
        rcv.value = S.getConsumerCapacity(rcv.consumer);
        if( rcv.value <= 0)
            continue;
        S.addAssign(rcv);
        S.subResourceTotalQuantity(rcv);
        S.subConsumerCapacity(rcv.consumer,rcv.value);
    }
}
std::string text(const cShare &S)
{
    std::stringstream ss;
    ss << S.resourceCount() << " resources, "
       << S.consumerCount() << " consumers\n";
    ss << "\nResource Initial Quantity\n";
    for (int k = 0; k < S.resourceCount(); k++)
        ss << k << "\t" << S.resourceQuantityInit(k) 
            <<"\t"<< S.resourceQuantity(k)
            << "\n";
    
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

    if (S.myProblemType == cShare::eProblem::consumer_capacity_resource_units)
    {
        ss << "\nConsumer\tInitial\t\tCapacity\n";
        for (int c = 0; c < S.consumerCount(); c++)
        {
            ss << c << "\t\t" << S.getConsumerCapacityInit(c) 
                << "\t\t" << S.getConsumerCapacity(c)
                << "\n";
        }
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