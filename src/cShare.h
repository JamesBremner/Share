#include <string>
#include <vector>
#include <map>

class cShare
{
    typedef std::pair<std::pair<int,int>,int> rcv_t;
    typedef std::map<std::pair<int, int>, int> rcmap_t;
    std::vector<int> myResourceTotalQuantity;
    rcmap_t myValueLinear;
    rcmap_t myAssign;
    int myV;

public:
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
     void subResourceTotalQuantity(int r, int q)
     {
        myResourceTotalQuantity[r] -= q;
     }
    void addValueLinear(int r, int c, int v)
    {
        myValueLinear.insert(
            std::make_pair(std::make_pair(r, c), v));
    }
    void addAssign( const rcv_t& rcv )
    {
        myAssign.insert( rcv );
    }

    rcv_t bestValueLinear() const;

    /// @brief value from assigning one resource unit
    /// @param r resource type
    /// @param c consumer type
    /// @return value / unit

    int value(int r, int c) const;
    int assign( int r, int c ) const;

    int resourceCount() const
    {
        return myResourceTotalQuantity.size();
    }
    int consumerCount() const;

    int resourceQuantity(int r) const
    {
        return myResourceTotalQuantity[r];
    }

};

void readFile(
    cShare &S,
    const std::string &fn);

/// @brief  Strightforward greedy - assign best values first
/// @param S 

void solve1(
    cShare &S);


std::string text(
    const cShare &S);