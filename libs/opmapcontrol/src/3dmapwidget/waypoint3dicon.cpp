#include "waypoint3dicon.h"

const float ICON_SCALL = 0.002;
const float TEXT_SCALL = 2;
const float LINE_SCALL = 0.0003;
const float PIN_SCALL  = 0.0001;


Waypoint3DIcon::Waypoint3DIcon(QObject *parent)
    : QCustom3DItem(parent)
{
    setMeshFile(":/uavs/ball");
    setScaling(QVector3D(ICON_SCALL, ICON_SCALL, ICON_SCALL));
    setScalingAbsolute(true);
    setShadowCasting(false);
}

Waypoint3DIcon::~Waypoint3DIcon()
{
}

void Waypoint3DIcon::setWayPoint(Waypoint *waypoint, Waypoint* home)
{
    m_waypoint = waypoint;
    m_home     = home;
}

void Waypoint3DIcon::update()
{
    QVector3D q(m_waypoint->getLongitude(),
               m_waypoint->getAltitude() / 1000, m_waypoint->getLatitude());
    if (m_waypoint->getId() != 0 && m_waypoint->getFrame() == MAV_FRAME_GLOBAL_RELATIVE_ALT){
        q.setY(q.y() + m_home->getAltitude() / 1000);
    }

    setPosition(q);
}

void Waypoint3DIcon::setColor(const QColor &color)
{
    QImage img(QSize(1,1), QImage::Format_ARGB32);
    img.setPixelColor(0, 0, color);
    this->setTextureImage(img);
}

//

Waypoint3DLine::Waypoint3DLine(QObject *parent)
{
    setMeshFile(":/uavs/pin");
    setScalingAbsolute(true);
    setShadowCasting(false);
    m_scale = LINE_SCALL;
}

void Waypoint3DLine::setRatio(float xlen, float ylen, float zlen)
{
    m_xlen = xlen;
    m_ylen = ylen;
    m_zlen = zlen;
}

void Waypoint3DLine::setWayPoints(Waypoint* wp1, Waypoint* wp2, Waypoint* home)
{
    m_wp1 = wp1;
    m_wp2 = wp2;
    m_home = home;
}

void Waypoint3DLine::update(){
    QVector3D p1(m_wp1->getLongitude(), m_wp1->getAltitude() / 1000, m_wp1->getLatitude());
    QVector3D p2(m_wp2->getLongitude(), m_wp2->getAltitude() / 1000, m_wp2->getLatitude());

    if (m_wp1->getId() != 0 && m_wp1->getFrame() == MAV_FRAME_GLOBAL_RELATIVE_ALT){
            p1.setY(p1.y() + m_home->getAltitude() / 1000);
    }

    if (m_wp2->getId() != 0 && m_wp2->getFrame() == MAV_FRAME_GLOBAL_RELATIVE_ALT){
            p2.setY(p2.y() + m_home->getAltitude() / 1000);
    }

    setPoints(p1, p2);
}

void Waypoint3DLine::setPoints(const QVector3D &p1, const QVector3D &p2)
{
    float yr, zr;
    if (m_zlen > m_xlen) {
        yr = m_ylen / m_xlen / 0.5;
        zr = m_zlen / m_xlen;
    }else{
        yr = m_ylen / m_zlen / 0.5;
        zr = m_xlen / m_zlen;
    }

    QVector3D q1 = p1;
    QVector3D q2 = p2;

    q1.setY( q1.y() / yr);
    q2.setY( q2.y() / yr);

    q1.setZ( q1.z() / zr);
    q2.setZ( q2.z() / zr);

    q1.setX( q1.x() / zr);
    q2.setX( q2.x() / zr);

    QVector3D v1 = q2 - q1;
    v1.normalize();

    QVector3D v2(0, 1, 0);

    QVector3D cross = QVector3D::crossProduct(v1, v2);
    cross.normalize();
    cross.setZ(cross.z() * -1);

    float dot = QVector3D::dotProduct(v1, v2);
    float theta = acos(dot);

    float deg = theta / M_PI * 180;

    QQuaternion q = QQuaternion::fromAxisAndAngle(cross, deg);

    //v1.setY()
    double distance = q1.distanceToPoint(q2);

    QVector3D pscale(m_scale, distance * yr, m_scale);

    setPosition(p1);
    setRotation(q);
    setScaling(pscale);
    setShadowCasting(false);
    setScalingAbsolute(false);

    QColor color(Qt::red);
    QImage img(QSize(1,1), QImage::Format_ARGB32);
    img.setPixelColor(0, 0, color);

    setTextureImage(img);
}


//
Waypoint3DPin::Waypoint3DPin(QObject *parent)
{
    setMeshFile(":/uavs/pin");
    setScalingAbsolute(true);
    setShadowCasting(false);
    m_scale = PIN_SCALL;
}

void Waypoint3DPin::setWayPoint(Waypoint *wp, Waypoint* home)
{
    m_wp1  = wp;
    m_home = home;
}

void Waypoint3DPin::update(){
    QVector3D p0(m_wp1->getLongitude(), 0, m_wp1->getLatitude());
    QVector3D p1(m_wp1->getLongitude(), m_wp1->getAltitude() / 1000, m_wp1->getLatitude());

    if (m_wp1->getId() != 0 && m_wp1->getFrame() == MAV_FRAME_GLOBAL_RELATIVE_ALT){
            p1.setY(p1.y() + m_home->getAltitude() / 1000);
    }

    setPoints(p0, p1);
}



Waypoint3DLabel::Waypoint3DLabel(QObject *parent)
    : QCustom3DLabel(parent)
{
    setScaling(QVector3D(TEXT_SCALL, TEXT_SCALL, TEXT_SCALL));
//    setScalingAbsolute(true);
    setShadowCasting(false);
    setFacingCamera(true);
}

Waypoint3DLabel::~Waypoint3DLabel()
{
}

void Waypoint3DLabel::setWayPoint(Waypoint *waypoint, Waypoint* home)
{
    m_waypoint = waypoint;
    m_home     = home;
}

void Waypoint3DLabel::update()
{
    QVector3D q(m_waypoint->getLongitude(),
               m_waypoint->getAltitude() / 1000, m_waypoint->getLatitude());

    if (m_waypoint->getId() != 0 && m_waypoint->getFrame() == MAV_FRAME_GLOBAL_RELATIVE_ALT){
        q.setY(q.y() + m_home->getAltitude() / 1000);
    }

//    q += QVector3D(0, 0.1, 0);

    QString text = QString::number(m_waypoint->getId());
    //test = "test";
    setText(text);

    setPosition(q);
}
