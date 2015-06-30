
#include "FloatSize.h"
#include <limits>
#include <math.h>

using namespace std;

float FloatSize::diagonalLength() const
{
    return sqrtf(diagonalLengthSquared());
}

bool FloatSize::isZero() const
{
    return fabs(m_width) < numeric_limits<float>::epsilon() && fabs(m_height) < numeric_limits<float>::epsilon();
}

bool FloatSize::isExpressibleAsIntSize() const
{
    return isWithinIntRange(m_width) && isWithinIntRange(m_height);
}

FloatSize FloatSize::narrowPrecision(double width, double height)
{
    return FloatSize((float)(width), (float)(height));
}

