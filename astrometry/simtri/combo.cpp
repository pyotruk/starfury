#include "combo.h"
/////////////////////////////////////////////////////////////////////////////////////
Combo::Combo(int i,
             int j,
             int k)
{
    _raw[0] = i;
    _raw[1] = j;
    _raw[2] = k;
}
/////////////////////////////////////////////////////////////////////////////////////
bool Combo::operator ==(const Combo &other) const
{
    int n = 0;
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3 ; ++j)
        {
            if(other._raw[j] == this->_raw[i])
            {
                ++n;
            }
        }
    }
    if(n == 3)    return true;
    else          return false;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
bool comb::isContain(const ComboVector &v,
                     const Combo &c)
{
    if(v.empty())    return false;
    for(ComboVector::const_iterator
        it = v.constBegin(); it < v.constEnd(); ++it)
    {
        if(*it == c)
        {
            return true;
        }
    }
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
