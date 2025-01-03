#pragma once
#include <string>
#include <vector>
#include <map>

/// @brief A consumer converts resources into value
struct sConsumer
{
    int myCapacityAllResources;
    std::vector<int> myCapacityEachResource;

    void subCapacity(int q)
    {
        myCapacityAllResources -= q;
    }
};
struct srcv
{
    int resource;
    int consumer;
    int value;

    srcv(int r, int c, int v)
        : resource(r),
          consumer(c),
          value(v)
    {
    }
    bool operator==(const srcv &other) const
    {
        return resource == other.resource &&
               consumer == other.consumer;
    }
};

class cShare
{
    typedef std::vector<srcv> vrcv_t;

    typedef std::map<std::pair<int, int>, int> rcmap_t;
    std::vector<int> myResourceTotalQuantity,myInitResourceQuantity;
    std::vector<sConsumer> myConsumers, myInitConsumers;
    vrcv_t myValueLinear;
    vrcv_t myAssign;
    int myV;

public:
    enum class eProblem
    {
        none,
        linear,
        consumer_capacity_resource_units,
    };
    eProblem myProblemType;

    cShare()
        : myProblemType(eProblem::none)
    {
    }

    void setProblemType(eProblem t)
    {
        myProblemType = t;
    }
    void setConsumerCapacity(int c, int v);

    void clear()
    {
        myResourceTotalQuantity.clear();
        myValueLinear.clear();
        myAssign.clear();
        myV = 0;
    }
    void addResourceTotalQuantity(int q)
    {
        myResourceTotalQuantity.push_back(q);
    }
    void subResourceTotalQuantity(const srcv &rcv)
    {
        myResourceTotalQuantity[rcv.resource] -= rcv.value;
    }
    void addValueLinear(int r, int c, int v)
    {
        myValueLinear.emplace_back(r, c, v);
    }
    void initBackup()
    {
        myInitConsumers = myConsumers;
        myInitResourceQuantity = myResourceTotalQuantity;
    }
    void addAssign(const srcv &rcv)
    {
        myAssign.push_back(rcv);
    }

    srcv bestValueLinear() const;

    vrcv_t sortValue();

    /// @brief value from assigning one resource unit
    /// @param r resource type
    /// @param c consumer type
    /// @return value / unit

    int value(int r, int c) const;
    int assign(int r, int c) const;

    int resourceCount() const
    {
        return myResourceTotalQuantity.size();
    }
    int consumerCount() const;

    int resourceQuantity(int r) const
    {
        return myResourceTotalQuantity[r];
    }
    int resourceQuantityInit(int r) const
    {
        return myInitResourceQuantity[r];
    }
    int getConsumerCapacity(int c) const;
    int getConsumerCapacityInit(int c) const;
    void subConsumerCapacity(int c, int q)
    {
        myConsumers[c].subCapacity(q);
    }
    vrcv_t getAssign()
    {
        return myAssign;
    }
};

void readFile(
    cShare &S,
    const std::string &fn);
void parseInput(
    cShare &S,
    std::istream &input);

void solve(cShare &S);

/// @brief  Straightforward greedy - assign best values first
/// @param S

void solve1(
    cShare &S);
void solve2(
    cShare &S);

std::string text(
    const cShare &S);

bool test();