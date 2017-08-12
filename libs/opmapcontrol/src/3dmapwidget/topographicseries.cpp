#include "topographicseries.h"
#include <qmath.h>
using namespace QtDataVisualization;

//! [0]
// Value used to encode height data as RGB value on PNG file
const float packingFactor = 11983.0f;
//! [0]

TopographicSeries::TopographicSeries()
{
    setDrawMode(QSurface3DSeries::DrawSurface);
    setFlatShadingEnabled(true);
    setBaseColor(Qt::gray);
}

TopographicSeries::~TopographicSeries()
{
}

void TopographicSeries::setTopographyFile(const QString file, float width, float height)
{
//! [1]
    QImage heightMapImage(file);
    setTopographyImage(heightMapImage, width, height);
}

void TopographicSeries::setTopographyImage(const QImage& img, float width, float height)
{
//! [1]
//!
    float xlen = m_xmax - m_xmin;
    float zlen = m_zmax - m_zmin;

    const uchar *bits = img.bits();
    int imageHeight = img.height();
    int imageWidth = img.width();
    int widthBits = imageWidth * 4;
    //float stepX = width / float(imageWidth);
    //float stepZ = height / float(imageHeight);

    float stepX = xlen / float(imageWidth);
    float stepZ = zlen / float(imageHeight);

    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(imageHeight);
    for (int i = 0; i < imageHeight; i++) {
        int p = i * widthBits;
//        float z = height - float(i) * stepZ;
        float z = m_zmax - float(i) * stepZ;
        QSurfaceDataRow *newRow = new QSurfaceDataRow(imageWidth);
        for (int j = 0; j < imageWidth; j++) {
            uchar bb = bits[p + 0];
            uchar gg = bits[p + 1];
            uchar rr = bits[p + 2];
            uchar aa = bits[p + 3];
            uint color = uint((rr << 16) + (gg << 8) + bb);
            float y;
            int threshold = qPow(2,23);
            if( color < threshold){
                y = color * 0.01;
            }else if(color == threshold){
                y = 0;
            }else{
                y = (color - qPow(2,24)) * 0.01;
            }
            if ( y > 4000) y = 0;
            float x = m_xmin + float(j) * stepX;

//            (*newRow)[j].setPosition(QVector3D(float(j) * stepX, y, z));
            (*newRow)[j].setPosition(QVector3D(x, y / 1000, z));
            p = p + 4;
        }
        *dataArray << newRow;
    }

    dataProxy()->resetArray(dataArray);
//! [1]

    m_sampleCountX = float(imageWidth);
    m_sampleCountZ = float(imageHeight);
}
