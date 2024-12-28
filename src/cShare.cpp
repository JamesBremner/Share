#include <fstream>
#include <sstream>

#include "cShare.h"

int cShare::value(int r, int c) const
{
    auto it = myValueLinear.find(std::make_pair(r, c));
    if (it == myValueLinear.end())
        return 0;
    return it->second;
}
int cShare::consumerCount() const
{
    int count = 0;
    for( auto m : myValueLinear )
    {
        if( m.first.second > count )
            count = m.first.second;
    }
    return count+1;
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
        if (ltype[0] == 'r')
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
std::string text( const cShare& S )
{
    std::stringstream ss;
    ss << S.resourceCount() << " resources, "
        << S.consumerCount() << " consumers\n";
    ss << "\nResource Quantity\n";
    for( int k = 0; k < S.resourceCount(); k++ )
        ss << k <<"\t"<< S.resourceQuantity(k) << "\n";
    ss << "\nValues\nResource\tConsumer\tValue / Unit\n";
    for( int r = 0; r < S.resourceCount(); r++ )
    for( int c = 0; c < S.consumerCount(); c++ )
    {
        int v = S.value(r,c);
        if( v > 0 )
            ss <<"    "<< r 
            <<"\t\t"<< c 
            <<"\t\t"<< v << "\n";
    }
    return ss.str();
}