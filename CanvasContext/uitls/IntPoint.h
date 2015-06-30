#ifndef IntPoint_h
#define IntPoint_h


class IntPoint
{
public:
    IntPoint() : m_x(0), m_y(0) { }
    IntPoint(int x, int y) : m_x(x), m_y(y) { }
    explicit IntPoint(const IntSize& size) : m_x(size.width()), m_y(size.height()) { }

    static IntPoint zero() { return IntPoint(); }

    int x() const { return m_x; }
    int y() const { return m_y; }

    void setX(int x) { m_x = x; }
    void setY(int y) { m_y = y; }

    void move(const IntSize& s) { move(s.width(), s.height()); }
    void moveBy(const IntPoint& offset) { move(offset.x(), offset.y()); }
    void move(int dx, int dy) { m_x += dx; m_y += dy; }
    void scale(float sx, float sy)
    {
        m_x = lroundf(static_cast<float>(m_x * sx));
        m_y = lroundf(static_cast<float>(m_y * sy));
    }

    IntPoint expandedTo(const IntPoint& other) const
    {
        return IntPoint(m_x > other.m_x ? m_x : other.m_x,
            m_y > other.m_y ? m_y : other.m_y);
    }

    IntPoint shrunkTo(const IntPoint& other) const
    {
        return IntPoint(m_x < other.m_x ? m_x : other.m_x,
            m_y < other.m_y ? m_y : other.m_y);
    }

    int distanceSquaredToPoint(const IntPoint&) const;

    void clampNegativeToZero()
    {
        *this = expandedTo(zero());
    }

    IntPoint transposedPoint() const
    {
        return IntPoint(m_y, m_x);
    }

private:
    int m_x, m_y;
};

inline IntPoint& operator+=(IntPoint& a, const IntSize& b)
{
    a.move(b.width(), b.height());
    return a;
}

inline IntPoint& operator-=(IntPoint& a, const IntSize& b)
{
    a.move(-b.width(), -b.height());
    return a;
}

inline IntPoint operator+(const IntPoint& a, const IntSize& b)
{
    return IntPoint(a.x() + b.width(), a.y() + b.height());
}

inline IntPoint operator+(const IntPoint& a, const IntPoint& b)
{
    return IntPoint(a.x() + b.x(), a.y() + b.y());
}

inline IntSize operator-(const IntPoint& a, const IntPoint& b)
{
    return IntSize(a.x() - b.x(), a.y() - b.y());
}

inline IntPoint operator-(const IntPoint& a, const IntSize& b)
{
    return IntPoint(a.x() - b.width(), a.y() - b.height());
}

inline IntPoint operator-(const IntPoint& point)
{
    return IntPoint(-point.x(), -point.y());
}

inline bool operator==(const IntPoint& a, const IntPoint& b)
{
    return a.x() == b.x() && a.y() == b.y();
}

inline bool operator!=(const IntPoint& a, const IntPoint& b)
{
    return a.x() != b.x() || a.y() != b.y();
}

inline IntSize toIntSize(const IntPoint& a)
{
    return IntSize(a.x(), a.y());
}

inline int IntPoint::distanceSquaredToPoint(const IntPoint& point) const
{
    return ((*this) - point).diagonalLengthSquared();
}


#endif // IntPoint_h
