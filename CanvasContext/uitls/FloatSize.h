
#ifndef FloatSize_h
#define FloatSize_h

#include "IntSize.h"
#include "MathExtras.h"
#include "IntPoint.h"

class IntSize;

class FloatSize {
public:
    FloatSize() : m_width(0), m_height(0) { }
    FloatSize(float width, float height) : m_width(width), m_height(height) { }
    FloatSize(const IntSize& size) : m_width(size.width()), m_height(size.height()) { }

    static FloatSize narrowPrecision(double width, double height);

    float width() const { return m_width; }
    float height() const { return m_height; }

    void setWidth(float width) { m_width = width; }
    void setHeight(float height) { m_height = height; }

    bool isEmpty() const { return m_width <= 0 || m_height <= 0; }
    bool isZero() const;
    bool isExpressibleAsIntSize() const;

    float aspectRatio() const { return m_width / m_height; }

    void expand(float width, float height)
    {
        m_width += width;
        m_height += height;
    }

    void scale(float s) { scale(s, s); }

    void scale(float scaleX, float scaleY)
    {
        m_width *= scaleX;
        m_height *= scaleY;
    }

    FloatSize expandedTo(const FloatSize& other) const
    {
        return FloatSize(m_width > other.m_width ? m_width : other.m_width,
            m_height > other.m_height ? m_height : other.m_height);
    }

    FloatSize shrunkTo(const FloatSize& other) const
    {
        return FloatSize(m_width < other.m_width ? m_width : other.m_width,
            m_height < other.m_height ? m_height : other.m_height);
    }

    float diagonalLength() const;
    float diagonalLengthSquared() const
    {
        return m_width * m_width + m_height * m_height;
    }

    FloatSize transposedSize() const
    {
        return FloatSize(m_height, m_width);
    }

private:
    float m_width, m_height;
};

inline FloatSize& operator+=(FloatSize& a, const FloatSize& b)
{
    a.setWidth(a.width() + b.width());
    a.setHeight(a.height() + b.height());
    return a;
}

inline FloatSize& operator-=(FloatSize& a, const FloatSize& b)
{
    a.setWidth(a.width() - b.width());
    a.setHeight(a.height() - b.height());
    return a;
}

inline FloatSize operator+(const FloatSize& a, const FloatSize& b)
{
    return FloatSize(a.width() + b.width(), a.height() + b.height());
}

inline FloatSize operator-(const FloatSize& a, const FloatSize& b)
{
    return FloatSize(a.width() - b.width(), a.height() - b.height());
}

inline FloatSize operator-(const FloatSize& size)
{
    return FloatSize(-size.width(), -size.height());
}

inline FloatSize operator*(const FloatSize& a, const float b)
{
    return FloatSize(a.width() * b, a.height() * b);
}

inline FloatSize operator*(const float a, const FloatSize& b)
{
    return FloatSize(a * b.width(), a * b.height());
}

inline bool operator==(const FloatSize& a, const FloatSize& b)
{
    return a.width() == b.width() && a.height() == b.height();
}

inline bool operator!=(const FloatSize& a, const FloatSize& b)
{
    return a.width() != b.width() || a.height() != b.height();
}

inline IntSize roundedIntSize(const FloatSize& p)
{
    return IntSize(clampToInteger(roundf(p.width())), clampToInteger(roundf(p.height())));
}

inline IntSize flooredIntSize(const FloatSize& p)
{
    return IntSize(clampToInteger(floorf(p.width())), clampToInteger(floorf(p.height())));
}

inline IntSize expandedIntSize(const FloatSize& p)
{
    return IntSize(clampToInteger(ceilf(p.width())), clampToInteger(ceilf(p.height())));
}

inline IntPoint flooredIntPoint(const FloatSize& p)
{
    return IntPoint(clampToInteger(floorf(p.width())), clampToInteger(floorf(p.height())));
}


#endif // FloatSize_h
