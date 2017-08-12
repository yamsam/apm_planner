#ifndef TOPOGRAPHICSERIES_H
#define TOPOGRAPHICSERIES_H

#include <QtDataVisualization/QSurface3DSeries>

using namespace QtDataVisualization;

class TopographicSeries : public QSurface3DSeries
{
    Q_OBJECT
public:
    explicit TopographicSeries();
    ~TopographicSeries();

    void setTopographyFile(const QString file, float width, float height);
    void setTopographyImage(const QImage&, float width, float height);
    void setXrange(float xmin, float xmax){
        m_xmin = xmin;
        m_xmax = xmax;
    }
    void setZrange(float zmin, float zmax){
        m_zmin = zmin;
        m_zmax = zmax;
    }

    float sampleCountX() { return m_sampleCountX; }
    float sampleCountZ() { return m_sampleCountZ; }

public Q_SLOTS:

private:
    float m_sampleCountX;
    float m_sampleCountZ;
    float m_xmin;
    float m_xmax;
    float m_zmin;
    float m_zmax;
};

#endif // TOPOGRAPHICSERIES_H
