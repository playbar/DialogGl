
#include "FloatPoint.h"
#include "SkPoint.h"
#include <limits>
#include <math.h>


// Skia has problems when passed infinite, etc floats, filter them to 0.
static inline SkScalar WebCoreFloatToSkScalar(float f)
{
    return SkFloatToScalar(std::isfinite(f) ? f : 0);
}

FloatPoint::FloatPoint(const IntPoint& p) : m_x(p.x()), m_y(p.y())
{
}

void FloatPoint::normalize()
{
    float tempLength = length();

    if (tempLength) {
        m_x /= tempLength;
        m_y /= tempLength;
    }
}

float FloatPoint::slopeAngleRadians() const
{
    return atan2f(m_y, m_x);
}

float FloatPoint::length() const
{
    return sqrtf(lengthSquared());
}
FloatPoint::operator SkPoint() const
{
    SkPoint p = { WebCoreFloatToSkScalar(m_x), WebCoreFloatToSkScalar(m_y) };
    return p;
}

FloatPoint FloatPoint::narrowPrecision(double x, double y)
{
    return FloatPoint((float)(x), (float)(y));
}

float findSlope(const FloatPoint& p1, const FloatPoint& p2, float& c)
{
    if (p2.x() == p1.x())
        return std::numeric_limits<float>::infinity();

    // y = mx + c
    float slope = (p2.y() - p1.y()) / (p2.x() - p1.x());
    c = p1.y() - slope * p1.x();
    return slope;
}

bool findIntersection(const FloatPoint& p1, const FloatPoint& p2, const FloatPoint& d1, const FloatPoint& d2, FloatPoint& intersection)
{
    float pxLength = p2.x() - p1.x();
    float pyLength = p2.y() - p1.y();

    float dxLength = d2.x() - d1.x();
    float dyLength = d2.y() - d1.y();

    float denom = pxLength * dyLength - pyLength * dxLength;
    if (!denom)
        return false;

    float param = ((d1.x() - p1.x()) * dyLength - (d1.y() - p1.y()) * dxLength) / denom;

    intersection.setX(p1.x() + param * pxLength);
    intersection.setY(p1.y() + param * pyLength);
    return true;
}


