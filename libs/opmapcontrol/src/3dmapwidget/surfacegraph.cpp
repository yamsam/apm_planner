/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Data Visualization module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "surfacegraph.h"
#include "topographicseries.h"

#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>

using namespace QtDataVisualization;

const float areaWidth = 8000.0f;
const float areaHeight = 8000.0f;
const float aspectRatio = 0.3f;
const float minRange = areaWidth * 0.49f;

SurfaceGraph::SurfaceGraph(Q3DSurface *surface)
    : m_graph(surface)
{
    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);
    m_graph->axisX()->setLabelFormat("%f");
    m_graph->axisZ()->setLabelFormat("%f");
    m_graph->axisX()->setRange(0.0f, areaWidth);
    m_graph->axisY()->setRange(0.0f, 5000.0f);
    m_graph->axisZ()->setRange(0.0f, areaHeight);
    m_graph->axisX()->setLabelAutoRotation(30);
    m_graph->axisY()->setLabelAutoRotation(90);
    m_graph->axisZ()->setLabelAutoRotation(30);
    m_graph->activeTheme()->setType(Q3DTheme::ThemePrimaryColors);

    QFont font = m_graph->activeTheme()->font();
    font.setPointSize(20);
    m_graph->activeTheme()->setFont(font);

    m_topography = new TopographicSeries();
    m_topography->setItemLabelFormat(QStringLiteral("@yLabel m"));
    m_graph->addSeries(m_topography);
}

SurfaceGraph::~SurfaceGraph()
{
    delete m_graph;
}

void SurfaceGraph::setImage(const QImage& img)
{
    m_topography->setTopographyImage(img, areaWidth, areaHeight);
}

void SurfaceGraph::setImagefile(QString filename)
{
    m_topography->setTopographyFile(filename, areaWidth, areaHeight);
}

//! [0]
void SurfaceGraph::setTexureFile(QString filename)
{
    m_topography->setTextureFile(filename);
//    m_topography->setTextureFile(":/maps/fuji_texture");
}

void SurfaceGraph::setTexure(const QImage &img)
{
    m_topography->setTexture(img);
}

//! [0]
