#ifndef SURFACEGRAPH_H
#define SURFACEGRAPH_H

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>
#include "topographicseries.h"

using namespace QtDataVisualization;

class SurfaceGraph : public QObject
{
    Q_OBJECT
public:
    explicit SurfaceGraph(Q3DSurface *surface);
    ~SurfaceGraph();

    void toggleSurfaceTexture(bool enable);
    void setImage(const QImage& img);
    void setImagefile(QString filename);

    void setTexure(const QImage& img);
    void setTexureFile(QString filename);

    void setXrange(float xmin, float xmax){
        qDebug() << "xrange" << xmin << xmax ;
        m_graph->axisX()->setRange(xmin, xmax);
        m_topography->setXrange(xmin, xmax);
    }

    void setZrange(float zmin, float zmax){
        qDebug() << "zrange" << zmin << zmax ;
        m_graph->axisZ()->setRange(zmin, zmax);
        m_topography->setZrange(zmin, zmax);
    }

private:
    Q3DSurface *m_graph;

    TopographicSeries *m_topography;
    int m_highlightWidth;
    int m_highlightHeight;
};

#endif // SURFACEGRAPH_H
