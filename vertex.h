#ifndef VERTEX_H
#define VERTEX_H
#include <QVector3D>

//Helped class for Vertex Shader Rendering
//QVector3D Represents a vector or vertex in 3D space
class Vertex
{
public:
    Q_DECL_CONSTEXPR Vertex();
    Q_DECL_CONSTEXPR Vertex(const QVector3D& position, const QVector3D& color);
    Q_DECL_CONSTEXPR explicit Vertex(const QVector3D& position);

    Q_DECL_CONSTEXPR const QVector3D& position() const;
    Q_DECL_CONSTEXPR const QVector3D& color() const;
    void setPosition(const QVector3D& position);
    void setColor(const QVector3D& color);

    static const int PositionTupleSize = 3;
    static const int ColorTupleSize = 3;
    static Q_DECL_CONSTEXPR int positionOffset();
    static Q_DECL_CONSTEXPR int colorOffset();
    static Q_DECL_CONSTEXPR int stride();

private:
    QVector3D m_position;
    QVector3D m_color;
};

Q_DECLARE_TYPEINFO(Vertex, Q_MOVABLE_TYPE);

Q_DECL_CONSTEXPR inline Vertex::Vertex() {}
Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D& position) : m_position(position) {}
Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D& position, const QVector3D& color) : m_position(position), m_color(color) {}

Q_DECL_CONSTEXPR inline const QVector3D& Vertex::position() const { return m_position; }
Q_DECL_CONSTEXPR inline const QVector3D& Vertex::color() const { return m_color; }
void inline Vertex::setPosition(const QVector3D& position) { m_position = position; }
void inline Vertex::setColor(const QVector3D& color) { m_color = color; }

Q_DECL_CONSTEXPR inline int Vertex::positionOffset() { return offsetof(Vertex, m_position);}
Q_DECL_CONSTEXPR inline int Vertex::colorOffset() { return offsetof(Vertex, m_color); }
Q_DECL_CONSTEXPR inline int Vertex::stride() { return sizeof(Vertex); }
#endif // VERTEX_H