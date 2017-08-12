#ifndef QGC3DMAPWIDGET_H
#define QGC3DMAPWIDGET_H

#include <QWidget>
#include <QImage>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QCustom3DItem>
#include <QtDataVisualization/QCustom3DLabel>

using namespace QtDataVisualization;

class SurfaceGraph;
class UASInterface;
class UASWaypointManager;
class Waypoint;
class Waypoint3DIcon;
class Waypoint3DPin;
class Waypoint3DLine;
class Waypoint3DLabel;


class QGC3DMapWidget : public QWidget
{
    Q_OBJECT
public:
    QGC3DMapWidget(QWidget *parent);
    ~QGC3DMapWidget();

    void setImage(const QImage& img);
    void setTexture(const QImage& img);

    void setXAxis(float min, float max); // 経度 long
    void setYAxis(float min, float max);
    void setZAxis(float min, float max); // 緯度 lat


 public slots:
    void activeUASSet(UASInterface* uas);
    void updateWaypoint(int uas, Waypoint* wp);
    void updateWaypointList(int uas);
    void updateHome(int uas);

protected:
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);
    void addUAS(UASInterface* uas);
    void redrawWaypointLines(int uas);

    UASWaypointManager* currWPManager; ///< The current waypoint manager

    QMap<Waypoint* , Waypoint3DIcon*>  waypointsToIcons;
    QMap<Waypoint3DIcon*, Waypoint*>   iconsToWaypoints;

    QMap<Waypoint* , Waypoint3DLabel*> waypointsToLabels;
    QMap<Waypoint3DLabel*, Waypoint*>  labelsToWaypoints;

    QMap<Waypoint* , Waypoint3DPin*>   waypointsToPins;
    QMap<Waypoint3DPin*, Waypoint*>    pinsToWaypoints;

    QMap<int, Waypoint3DLine*> waypointLines;

    Waypoint* firingWaypointChange;

protected slots:
    void updateGlobalPosition(UASInterface *uas,  double lat, double lon, double alt,
                              quint64 usec);

private:
     Q3DSurface     *m_graph;
     SurfaceGraph   *m_surface;
     QCustom3DItem  *uav_item;
     QCustom3DLabel *uav_label;
     UASInterface   *uas;
     Waypoint       *m_home;

     float m_xlen;
     float m_ylen;
     float m_zlen;



};

#endif // QGC3DMAPWIDGET_H
