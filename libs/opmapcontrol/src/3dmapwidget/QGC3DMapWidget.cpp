#include <QSize>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScreen>
#include <QVector3D>
#include <QFont>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QCustom3DItem>
#include <QtDataVisualization/QCustom3DLabel>
#include "UASInterface.h"
#include "UASManager.h"
#include "surfacegraph.h"
#include "QGC3DMapWidget.h"
#include "UASWaypointManager.h"
#include "WaypointNavigation.h"
#include "logging.h"
#include "waypoint3dicon.h"


const float UAV_SCALL  = 0.1;
const float LABEL_OFFSET = 2;


QGC3DMapWidget::QGC3DMapWidget(QWidget* parent):
    QWidget(parent)
{
    m_graph = new Q3DSurface();

    QWidget *container = QWidget::createWindowContainer(m_graph);
    m_graph->setOrthoProjection(false);
    QSize screenSize = m_graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    QVBoxLayout *vLayout = new QVBoxLayout();

    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    vLayout->setAlignment(Qt::AlignTop);

    m_surface = new SurfaceGraph(m_graph);

    // uav
    uav_item = new QCustom3DItem(m_graph);
    uav_item->setMeshFile(":/uavs/plane");
    uav_item->setTextureFile(":/uavs/images/airplanepip.png");
    // uav_item->setMeshFile(":/uavs/ball");
    uav_item->setScaling(QVector3D(UAV_SCALL, UAV_SCALL, UAV_SCALL));
    uav_item->setScalingAbsolute(true);
    uav_item->setShadowCasting(false);

    QFont uavfont = QFont("Century Gothic", 10);

    uav_label = new QCustom3DLabel(m_graph);
    uav_label->setFacingCamera(true);
    uav_label->setFont(uavfont);
    uav_label->setScalingAbsolute(true);
    uav_label->setScaling(QVector3D(1., 1., 1.));


    m_graph->addCustomItem(uav_item);
  //  m_graph->addCustomItem(uav_label);

    currWPManager = UASManager::instance()->getActiveUASWaypointManager();
    connect(currWPManager, SIGNAL(waypointEditableListChanged(int)), this, SLOT(updateWaypointList(int)));
    connect(currWPManager, SIGNAL(waypointEditableChanged(int, Waypoint*)), this, SLOT(updateWaypoint(int,Waypoint*)));
    //connect(this, SIGNAL(waypointCreated(Waypoint*)), currWPManager, SLOT(addWaypointEditable(Waypoint*)));
    //connect(this, SIGNAL(waypointChanged(Waypoint*)), currWPManager, SLOT(notifyOfChangeEditable(Waypoint*)));

}

QGC3DMapWidget::~QGC3DMapWidget()
{

}

void QGC3DMapWidget::setImage(const QImage &img)
{
    m_surface->setImage(img);
}

void QGC3DMapWidget::setTexture(const QImage &img)
{
    m_surface->setTexure(img);
}

void QGC3DMapWidget::setXAxis(float min, float max)
{
    m_surface->setXrange(min, max);
    m_xlen = max - min;
}

void QGC3DMapWidget::setYAxis(float min, float max)
{
    m_graph->axisY()->setRange(min, max);
    m_ylen = max - min;
}

void QGC3DMapWidget::setZAxis(float min, float max)
{
    m_surface->setZrange(min, max);
    m_zlen = max - min;
}

void QGC3DMapWidget::showEvent(QShowEvent *event)
{
    qDebug() << "showEvent";
    QWidget::showEvent(event);

    foreach (UASInterface* uas, UASManager::instance()->getUASList())
    {
        addUAS(uas);
    }
    activeUASSet(UASManager::instance()->getActiveUAS());
}

void QGC3DMapWidget::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
}

void QGC3DMapWidget::addUAS(UASInterface* uas)
{
    connect(uas, SIGNAL(globalPositionChanged(UASInterface*,double,double,double,quint64)), this, SLOT(updateGlobalPosition(UASInterface*,double,double,double,quint64)));
//    connect(uas, SIGNAL(systemSpecsChanged(int)), this, SLOT(updateSystemSpecs(int)));
}


void QGC3DMapWidget::updateGlobalPosition(UASInterface* uas, double lat, double lon, double alt, quint64 usec)
{
    Q_UNUSED(usec);
    QVector3D position(lon, alt / 1000, lat);
    qDebug() << alt / 1000;

    uav_item->setPosition(position);

    QVector3D label_position(position);

//    label_position.setY(position.y() + 500 / 1000);

 //   uav_label->setPosition(label_position);
//    uav_label->setText(QString::number(alt));

    double yaw   = - uas->getYaw()   / M_PI * 180;
    double roll  = - uas->getRoll()  / M_PI * 180;
    double pitch =   uas->getPitch() / M_PI * 180;

    uav_item->setRotation(QQuaternion::fromEulerAngles(pitch, yaw, roll));
}

void QGC3DMapWidget::activeUASSet(UASInterface *uas)
{
    // Only execute if proper UAS is set
    if (!uas)
    {
        this->uas = 0;
        return;
    }
    if (this->uas == uas) return;

    QLOG_DEBUG() << "activeUASSet" << uas->getUASName();

    // Disconnect old MAV manager
    if (currWPManager)
    {
        // Disconnect the waypoint manager / data storage from the UI
        disconnect(currWPManager, SIGNAL(waypointEditableListChanged(int)), this, SLOT(updateWaypointList(int)));
        disconnect(currWPManager, SIGNAL(waypointEditableChanged(int, Waypoint*)), this, SLOT(updateWaypoint(int,Waypoint*)));
//        disconnect(this, SIGNAL(waypointCreated(Waypoint*)), currWPManager, SLOT(addWaypointEditable(Waypoint*)));
//        disconnect(this, SIGNAL(waypointChanged(Waypoint*)), currWPManager, SLOT(notifyOfChangeEditable(Waypoint*)));

        /*
        QGraphicsItemGroup* group = waypointLine(this->uas ? this->uas->getUASID() : 0);
        if (group)
        {
            // Delete existing waypoint lines
            foreach (QGraphicsItem* item, group->childItems())
            {
                delete item;
            }
        }
        */
    }

    this->uas = uas;
    this->currWPManager = uas->getWaypointManager();

 //   updateSelectedSystem(uas->getUASID());
 //   followUAVID = uas->getUASID();

    updateHome(uas->getUASID());
    updateWaypointList(uas->getUASID());

    redrawWaypointLines(uas->getUASID());

    // Connect the waypoint manager / data storage to the UI
    connect(currWPManager, SIGNAL(waypointEditableListChanged(int)), this, SLOT(updateWaypointList(int)));
    connect(currWPManager, SIGNAL(waypointEditableChanged(int, Waypoint*)), this, SLOT(updateWaypoint(int,Waypoint*)));
 //   connect(this, SIGNAL(waypointCreated(Waypoint*)), currWPManager, SLOT(addWaypointEditable(Waypoint*)));
 //   connect(this, SIGNAL(waypointChanged(Waypoint*)), currWPManager, SLOT(notifyOfChangeEditable(Waypoint*)));
}

/**
 * This function is called if a a single waypoint is updated and
 * also if the whole list changes.
 */
void QGC3DMapWidget::updateWaypoint(int uas, Waypoint* wp)
{
    qDebug() << "updateWaypoint 3D";
    // Source of the event was in this widget, do nothing
    if (firingWaypointChange == wp) {
        return;
    }
    // Currently only accept waypoint updates from the UAS in focus
    // this has to be changed to accept read-only updates from other systems as well.

    UASInterface* uasInstance = UASManager::instance()->getUASForId(uas);
    if (currWPManager)
    {
        // Only accept waypoints in global coordinate frame
        if (((wp->getFrame() == MAV_FRAME_GLOBAL) || (wp->getFrame() == MAV_FRAME_GLOBAL_RELATIVE_ALT)) && (wp->isNavigationType() || wp->visibleOnMapWidget()))
        {
            qDebug() << " frame ";
            // We're good, this is a global waypoint

            // Get the index of this waypoint
            int wpindex = currWPManager->getIndexOf(wp);
            // If not found, return (this should never happen, but helps safety)
            if (wpindex < 0) return;
            // Mark this wp as currently edited
            firingWaypointChange = wp;

            // Check if wp exists yet in map
            if (!waypointsToIcons.contains(wp))
            {
                // Create icon for new WP
                QColor wpColor(Qt::red);
                if (uasInstance) wpColor = uasInstance->getColor();

                Waypoint3DIcon* icon = new Waypoint3DIcon(m_graph);
                icon->setColor(wpColor);
                icon->setWayPoint(wp, m_home);
                icon->update();
                m_graph->addCustomItem(icon);

                Waypoint3DLabel* label = new Waypoint3DLabel(m_graph);
                label->setWayPoint(wp, m_home);
                label->update();
                m_graph->addCustomItem(label);

                Waypoint3DPin* pin = new Waypoint3DPin(m_graph);
                pin->setRatio(m_xlen, m_ylen, m_zlen);
                pin->setColor(wpColor);
                pin->setWayPoint(wp, m_home);
                pin->update();
                m_graph->addCustomItem(pin);

//                ConnectWP(icon);
//                icon->setParentItem(map);
                // Update maps to allow inverse data association
                waypointsToIcons.insert(wp, icon);
                iconsToWaypoints.insert(icon, wp);

                waypointsToLabels.insert(wp, label);
                labelsToWaypoints.insert(label, wp);

                waypointsToPins.insert(wp, pin);
                pinsToWaypoints.insert(pin, wp);

            }
            else
            {
                // Waypoint exists, block it's signals and update it
//
                Waypoint3DIcon* icon   = waypointsToIcons.value(wp);
                Waypoint3DLabel* label = waypointsToLabels.value(wp);
                Waypoint3DPin* pin     = waypointsToPins.value(wp);

                // Make sure we don't die on a null pointer
                // this should never happen, just a precaution
                if (!icon) return;
                // Block outgoing signals to prevent an infinite signal loop
                // should not happen, just a precaution
//               this->blockSignals(true);
                // Update the WP
//                Waypoint2DIcon* wpicon = dynamic_cast<Waypoint2DIcon*>(icon);
                if (icon)
                {
                    // Use safe standard interfaces for non Waypoint-class based wps
                    icon->setWayPoint(wp, m_home);
                    icon->update();

                    label->setWayPoint(wp, m_home);
                    label->update();

                    pin->setWayPoint(wp, m_home);
                    pin->update();


                }
                // Re-enable signals again
//                this->blockSignals(false);
            }
          //  redrawWaypointLines(uas);
            firingWaypointChange = NULL;
        }
        else
        {
            // Check if the index of this waypoint is larger than the global
            // waypoint list. This implies that the coordinate frame of this
            // waypoint was changed and the list containing only global
            // waypoints was shortened. Thus update the whole list
//            if (waypointsToIcons.count() > currWPManager->getGlobalFrameAndNavTypeCount())
//            {
//                updateWaypointList(uas);
//            }
        }
    }
}

/**
 * Update the whole list of waypoints. This is e.g. necessary if the list order changed.
 * The UAS manager will emit the appropriate signal whenever updating the list
 * is necessary.
 */
void QGC3DMapWidget::updateWaypointList(int uas)
{
    qDebug() << "update waypoint lilst 3D" << uas;
    // Currently only accept waypoint updates from the UAS in focus
    // this has to be changed to accept read-only updates from other systems as well.
    if (currWPManager)
    {
        // Delete first all old waypoints
        // this is suboptimal (quadratic, but wps should stay in the sub-100 range anyway)
        QList<Waypoint* > wps = currWPManager->getGlobalFrameAndNavTypeWaypointList(false);
        foreach (Waypoint* wp, waypointsToIcons.keys())
        {
            if (!wps.contains(wp))
            {
                QLOG_TRACE() << "DELETE EXISTING WP" << wp->getId();
                // Get icon to work on
//                mapcontrol::WayPointItem* icon = waypointsToIcons.value(wp);
//                waypointsToIcons.remove(wp);
//                iconsToWaypoints.remove(icon);
//                WPDelete(icon);
            }
        }

        // Update all existing waypoints
        foreach (Waypoint* wp, waypointsToIcons.keys())
        {
            qDebug() << "UPDATING EXISTING WP 3D" << wp->getId();
            updateWaypoint(uas, wp);
        }

        // Update all potentially new waypoints
        foreach (Waypoint* wp, wps)
        {
            // Update / add only if new
            if (!waypointsToIcons.contains(wp))
            {
                qDebug() << "UPDATING NEW WP 3D" << wp->getId();
                updateWaypoint(uas, wp);
            }
        }

//        redrawWaypointLines(uas);
    }
}


void QGC3DMapWidget::redrawWaypointLines(int uas)
{
    if (!currWPManager)
        return;

    /*
    QGraphicsItemGroup* group = waypointLine(uas);
    if (!group)
        return;
    Q_ASSERT(group->parentItem() == map);

    // Delete existing waypoint lines
    foreach (QGraphicsItem* item, group->childItems())
    {
        QLOG_TRACE() << "DELETE EXISTING WAYPOINT LINES" << item;
        delete item;
    }
*/

    QList<Waypoint*> wps = currWPManager->getGlobalFrameAndNavTypeWaypointList(true);
    if (wps.size() > 1)
    {
        for (int i = 0; i < wps.size()-1; i++){
            wps[i]->getName();
            Waypoint3DLine* line = new Waypoint3DLine(m_graph);
            line->setRatio(m_xlen, m_ylen, m_zlen);
            line->setWayPoints(wps[i], wps[i+1], m_home);
            line->update();
            m_graph->addCustomItem(line);
        }

        /*
        QPainterPath path = WaypointNavigation::path(wps, *map);
        if (path.elementCount() > 1)
        {
            QGraphicsPathItem* gpi = new QGraphicsPathItem(map);
            gpi->setPath(path);

            QColor color(Qt::red);
            UASInterface* uasInstance = UASManager::instance()->getUASForId(uas);
            if (uasInstance) color = uasInstance->getColor();
            QPen pen(color);
            pen.setWidth(2);
            gpi->setPen(pen);

            QLOG_TRACE() << "ADDING WAYPOINT LINES" << gpi;
            group->addToGroup(gpi);
        }
        */
    }
}

void QGC3DMapWidget::updateHome(int uas){
    if (!currWPManager)
        return;

    QList<Waypoint* > wps = currWPManager->getGlobalFrameAndNavTypeWaypointList(true);

    for (int i = 0; i < wps.size(); i++){
        if(wps[i]->getId() == 0){
            m_home = wps[i];
            break;
        }
    }
}
