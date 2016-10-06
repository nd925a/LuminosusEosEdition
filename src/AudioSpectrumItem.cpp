#include "AudioSpectrumItem.h"

#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>
#include <QQuickWindow>
#include <QGuiApplication>
#include <QScreen>
#include <cmath>


AudioSpectrumItem::AudioSpectrumItem(QQuickItem *parent)
    : QQuickItem(parent)
    , m_color("blue")
    , m_lineWidth(3)
    , m_agcEnabled(true)
    , m_device_pixel_ratio(QGuiApplication::primaryScreen()->devicePixelRatio())
{
    setFlag(ItemHasContents, true);
}

AudioSpectrumItem::~AudioSpectrumItem()
{
}

void AudioSpectrumItem::setColor(const QColor &color) {
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged(color);
    update();
}

void AudioSpectrumItem::setLineWidth(float width) {
    if (m_lineWidth == width)
        return;

    m_lineWidth = width;
    emit lineWidthChanged(width);
    update();
}

void AudioSpectrumItem::setAnalyzer(AudioInputAnalyzer* value) {
    if (m_analyzer) {
        disconnect(m_analyzer, SIGNAL(spectrumChanged()), this, SLOT(update()));
    }
    m_analyzer = value;
    if (m_analyzer) {
        connect(m_analyzer, SIGNAL(spectrumChanged()), this, SLOT(update()));
    }
    emit analyzerChanged();
    update();
}

QSGNode* AudioSpectrumItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) {
    if (!m_analyzer) return nullptr;

    std::vector<double> points = m_analyzer->getSimplifiedSpectrum();
    double gain = m_agcEnabled ? m_analyzer->getAgcValue() : 1.0;
    const int pointCount = points.size();
    if (pointCount < 2) return nullptr;

    // -------------------- Prepare QSG Nodes:
    QSGNode* parentNode = nullptr;
    if (oldNode) {
        parentNode = static_cast<QSGNode*>(oldNode);
    } else {
        parentNode = new QSGNode;
    }
    // adapt child count:
    int childCount = parentNode->childCount();
    if (childCount != 2) {
        parentNode->removeAllChildNodes();
        QSGGeometryNode* node = new QSGGeometryNode;
        QSGGeometry* geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 3);
        geometry->setDrawingMode(GL_TRIANGLE_STRIP);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        QSGFlatColorMaterial* material = new QSGFlatColorMaterial;
        material->setColor(m_color);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
        parentNode->appendChildNode(node);

        node = new QSGGeometryNode;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 3);
        geometry->setDrawingMode(GL_TRIANGLE_STRIP);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        material = new QSGFlatColorMaterial;
        material->setColor("#777");
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
        parentNode->appendChildNode(node);
    }

    //double widthOffset = m_lineWidth / 2;

    int verticesCount = pointCount * 2;

    QSGGeometryNode* qsgNode = static_cast<QSGGeometryNode*>(parentNode->childAtIndex(0));
    QSGGeometryNode* qsgNodeOutline = static_cast<QSGGeometryNode*>(parentNode->childAtIndex(1));
    if (!qsgNode || !qsgNodeOutline) {
        qCritical() << "[SpectrumItem] Could not get QSG Node.";
        return nullptr;
    }
    QSGGeometry* geometry = qsgNode->geometry();
    QSGGeometry* geometryOutline = qsgNodeOutline->geometry();
    if (!geometry || !geometryOutline) {
        qCritical() << "[SpectrumItem] Could not get QSG Geometry.";
        return nullptr;
    }
    geometry->allocate(verticesCount);
    geometryOutline->allocate(verticesCount);
    QSGGeometry::Point2D* vertices = geometry->vertexDataAsPoint2D();
    QSGGeometry::Point2D* verticesOutline = geometryOutline->vertexDataAsPoint2D();

    if (! vertices || !verticesOutline) {
        qCritical() << "[SpectrumItem] Could not get QSG vertices.";
        return nullptr;
    }

    const double itemWidth = width();
    const double itemHeight = height();
    const double outlineWidth = m_lineWidth;

    // draw spectrum:
    for (int i = 0; i < pointCount; ++i) {
        float x = itemWidth * (i / float(pointCount - 1));
        float y = itemHeight * (1 - points[i] * gain);

        vertices[i*2].set(x, itemHeight);
        vertices[i*2+1].set(x, y);

        verticesOutline[i*2].set(x, y - outlineWidth);
        verticesOutline[i*2+1].set(x, y);
    }

    // tell Scene Graph that this items needs to be drawn:
    qsgNode->markDirty(QSGNode::DirtyGeometry);
    qsgNodeOutline->markDirty(QSGNode::DirtyGeometry);

    return parentNode;
}
