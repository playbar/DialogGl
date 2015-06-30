#ifndef IntSize_h
#define IntSize_h

class IntSize
{
public:
    IntSize() : m_width(0), m_height(0) { }
    IntSize(int width, int height) : m_width(width), m_height(height) { }

    int width() const { return m_width; }
    int height() const { return m_height; }

    void setWidth(int width) { m_width = width; }
    void setHeight(int height) { m_height = height; }

    bool isEmpty() const { return m_width <= 0 || m_height <= 0; }
    bool isZero() const { return !m_width && !m_height; }

    float aspectRatio() const { return static_cast<float>(m_width) / static_cast<float>(m_height); }

    void expand(int width, int height)
    {
        m_width += width;
        m_height += height;
    }

    void scale(float widthScale, float heightScale)
    {
        m_width = static_cast<int>(static_cast<float>(m_width) * widthScale);
        m_height = static_cast<int>(static_cast<float>(m_height) * heightScale);
    }

    void scale(float scale)
    {
        this->scale(scale, scale);
    }

    IntSize expandedTo(const IntSize& other) const
    {
        return IntSize(m_width > other.m_width ? m_width : other.m_width,
            m_height > other.m_height ? m_height : other.m_height);
    }

    IntSize shrunkTo(const IntSize& other) const
    {
        return IntSize(m_width < other.m_width ? m_width : other.m_width,
            m_height < other.m_height ? m_height : other.m_height);
    }

    void clampNegativeToZero()
    {
        *this = expandedTo(IntSize());
    }

    void clampToMinimumSize(const IntSize& minimumSize)
    {
        if (m_width < minimumSize.width())
            m_width = minimumSize.width();
        if (m_height < minimumSize.height())
            m_height = minimumSize.height();
    }

    int area() const
    {
        return m_width * m_height;
    }

    int diagonalLengthSquared() const
    {
        return m_width * m_width + m_height * m_height;
    }

    IntSize transposedSize() const
    {
        return IntSize(m_height, m_width);
    }

private:
    int m_width, m_height;
};

inline IntSize& operator+=(IntSize& a, const IntSize& b)
{
    a.setWidth(a.width() + b.width());
    a.setHeight(a.height() + b.height());
    return a;
}

inline IntSize& operator-=(IntSize& a, const IntSize& b)
{
    a.setWidth(a.width() - b.width());
    a.setHeight(a.height() - b.height());
    return a;
}

inline IntSize operator+(const IntSize& a, const IntSize& b)
{
    return IntSize(a.width() + b.width(), a.height() + b.height());
}

inline IntSize operator-(const IntSize& a, const IntSize& b)
{
    return IntSize(a.width() - b.width(), a.height() - b.height());
}

inline IntSize operator-(const IntSize& size)
{
    return IntSize(-size.width(), -size.height());
}

inline bool operator==(const IntSize& a, const IntSize& b)
{
    return a.width() == b.width() && a.height() == b.height();
}

inline bool operator!=(const IntSize& a, const IntSize& b)
{
    return a.width() != b.width() || a.height() != b.height();
}

#endif // IntSize_h
