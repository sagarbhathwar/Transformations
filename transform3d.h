#ifndef TRANSFORM3D_H
#define TRANSFORM3D_H
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class Transform3D
{
public:
    Transform3D();

    Transform3D translate(const QVector3D& dt);
    Transform3D translate(float dx, float dy, float dz);
    Transform3D scale(const QVector3D& ds);
    Transform3D scale(float dx, float dy, float dz);
    Transform3D scale(float factor);
    Transform3D rotate(const QQuaternion& dr);
    Transform3D rotate(float angle, const QVector3D& axis);
    Transform3D rotate(float angle, float ax, float ay, float az);
    Transform3D grow(const QVector3D& ds);
    Transform3D grow(float dx, float dy, float dz);
    Transform3D grow(float factor);

    Transform3D setTranslation(const QVector3D& t);
    Transform3D setTranslation(float x, float y, float z);
    Transform3D setScale(const QVector3D& s);
    Transform3D setScale(float x, float y, float z);
    Transform3D setScale(float k);
    Transform3D setRotation(const QQuaternion& r);
    Transform3D setRotation(float angle, const QVector3D& axis);
    Transform3D setRotation(float angle, float ax, float ay, float az);

    const QVector3D& translation() const;
    const QVector3D& scale() const;
    const QQuaternion& rotation() const;
    const QMatrix4x4& toMatrix();

private:
    bool m_dirty;
    QVector3D m_translation;
    QVector3D m_scale;
    QQuaternion m_rotation;
    QMatrix4x4 m_world;

#ifndef QT_NO_DATASTREAM
  friend QDataStream& operator<<(QDataStream& out, const Transform3D& transform);
  friend QDataStream& operator>>(QDataStream& in, Transform3D& transform);
#endif
};

Q_DECLARE_TYPEINFO(Transform3D, Q_MOVABLE_TYPE);

inline Transform3D::Transform3D() : m_dirty(true), m_scale(1.0f, 1.0f, 1.0f) {}

inline Transform3D Transform3D::translate(float dx, float dy,float dz) { translate(QVector3D(dx, dy, dz)); return *this; }
inline Transform3D Transform3D::scale(float dx, float dy,float dz) { scale(QVector3D(dx, dy, dz)); return *this; }
inline Transform3D Transform3D::scale(float factor) { scale(QVector3D(factor, factor, factor)); return *this; }
inline Transform3D Transform3D::rotate(float angle, const QVector3D &axis) { rotate(QQuaternion::fromAxisAndAngle(axis, angle)); return *this; }
inline Transform3D Transform3D::rotate(float angle, float ax, float ay,float az) { rotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); return *this; }
inline Transform3D Transform3D::grow(float dx, float dy, float dz) { grow(QVector3D(dx, dy, dz)); return *this; }
inline Transform3D Transform3D::grow(float factor) { grow(QVector3D(factor, factor, factor)); return *this; }

inline Transform3D Transform3D::setTranslation(float x, float y, float z) { setTranslation(QVector3D(x, y, z)); return *this; }
inline Transform3D Transform3D::setScale(float x, float y, float z) { setScale(QVector3D(x, y, z)); return *this; }
inline Transform3D Transform3D::setScale(float k) { setScale(QVector3D(k, k, k)); return *this; }
inline Transform3D Transform3D::setRotation(float angle, const QVector3D &axis) { setRotation(QQuaternion::fromAxisAndAngle(axis, angle)); return *this; }
inline Transform3D Transform3D::setRotation(float angle, float ax, float ay, float az) { setRotation(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); return *this; }

inline const QVector3D& Transform3D::translation() const { return m_translation; }
inline const QVector3D& Transform3D::scale() const { return m_scale; }
inline const QQuaternion& Transform3D::rotation() const { return m_rotation; }

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const Transform3D &transform);
#endif

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const Transform3D &transform);
QDataStream &operator>>(QDataStream &in, Transform3D &transform);
#endif


#endif // TRANSFORM3D_H
