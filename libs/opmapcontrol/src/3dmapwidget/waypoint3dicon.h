#ifndef WAYPOINT3DICON_H
#define WAYPOINT3DICON_H

#include <QObject>
#include <QColor>
#include <QtDataVisualization/QCustom3DItem>
#include <QtDataVisualization/QCustom3DLabel>
#include <Waypoint.h>

using namespace QtDataVisualization;

class Waypoint3DIcon : public QCustom3DItem
{
     Q_OBJECT
public:
    Waypoint3DIcon(QObject *parent = Q_NULLPTR);
    virtual ~Waypoint3DIcon();
    void setWayPoint(Waypoint* waypoint, Waypoint* home);
    void setColor(const QColor& color);
    void update();

private:
    Waypoint *m_waypoint;
    Waypoint *m_home;
};



class Waypoint3DLine: public Waypoint3DIcon
{
    Q_OBJECT
public:

    Waypoint3DLine(QObject *parent = Q_NULLPTR);
    void setRatio(float xlen, float ylen, float zlen);
    void setWayPoints(Waypoint* p1, Waypoint* p2, Waypoint* home);
    void setPoints(const QVector3D& p1, const QVector3D& p2);
    void update();
protected:
    Waypoint *m_wp1;
    Waypoint *m_wp2;
    Waypoint *m_home;

    float m_xlen;
    float m_ylen;
    float m_zlen;
    float m_scale;
};

class Waypoint3DPin: public Waypoint3DLine
{
    Q_OBJECT
public:

    Waypoint3DPin(QObject *parent = Q_NULLPTR);
    void setWayPoint(Waypoint* wp1, Waypoint* home);
    void update();

};

class Waypoint3DLabel : public QCustom3DLabel
{
     Q_OBJECT
public:
    Waypoint3DLabel(QObject *parent = Q_NULLPTR);
    virtual ~Waypoint3DLabel();
    void setWayPoint(Waypoint* waypoint, Waypoint* home);
    void update();


private:
    Waypoint *m_waypoint;
    Waypoint *m_home;
};

#endif // WAYPOINT3DICON_H
