#include "MapWidget.h"
#include <QPainter>
#include <QNetworkRequest>
#include <QUrl>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

MapWidget::MapWidget(QWidget *parent)
    : QWidget(parent)
{
    // Start with a default view roughly over India/Global center just in case
    m_lat = 18.5204;
    m_lon = 73.8567;
    
    connect(&m_netManager, &QNetworkAccessManager::finished,
            this, &MapWidget::handleNetworkData);
            
    connect(&m_pulseTimer, &QTimer::timeout, this, &MapWidget::updatePulse);
    m_pulseTimer.start(50);
}

void MapWidget::setCoordinate(double lat, double lon)
{
    if (qAbs(m_lat - lat) < 0.000001 && qAbs(m_lon - lon) < 0.000001) return;
    
    m_lat = lat;
    m_lon = lon;
    update();
}

void MapWidget::updatePulse()
{
    if (m_pulseGrowing) {
        m_pulseSize++;
        if (m_pulseSize > 15) m_pulseGrowing = false;
    } else {
        m_pulseSize--;
        if (m_pulseSize < 5) m_pulseGrowing = true;
    }
    update();
}

void MapWidget::requestTile(int x, int y, int z)
{
    QString key = QString("%1_%2_%3").arg(z).arg(x).arg(y);
    if (m_tileCache.contains(key)) return;
    
    // Insert empty pixmap to avoid repeated requests
    m_tileCache.insert(key, QPixmap());
    
    QString urlStr = QString("https://tile.openstreetmap.org/%1/%2/%3.png").arg(z).arg(x).arg(y);
    QNetworkRequest request((QUrl(urlStr)));
    request.setRawHeader("User-Agent", "NAKSHATRA_GCS/1.0 (Qt6)");
    m_netManager.get(request);
}

void MapWidget::handleNetworkData(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }
    
    QUrl url = reply->request().url();
    QString path = url.path(); // /16/47512/29283.png
    QStringList parts = path.split('/');
    if (parts.size() >= 4) {
        QString z = parts[1];
        QString x = parts[2];
        QString y = parts[3].replace(".png", "");
        QString key = QString("%1_%2_%3").arg(z, x, y);
        
        QPixmap pix;
        pix.loadFromData(reply->readAll());
        if (!pix.isNull()) {
            m_tileCache.insert(key, pix);
            update();
        }
    }
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw background
    painter.fillRect(rect(), QColor("#ffffff"));
    
    // Convert lat/lon to OSM tile coordinates
    double n = pow(2.0, m_zoom);
    double tx = (m_lon + 180.0) / 360.0 * n;
    double lat_rad = m_lat * M_PI / 180.0;
    double ty = (1.0 - asinh(tan(lat_rad)) / M_PI) / 2.0 * n;
    
    int tileX = floor(tx);
    int tileY = floor(ty);
    
    // Offset within the center tile (in pixels, 0-255)
    int offsetX = (tx - tileX) * 256;
    int offsetY = (ty - tileY) * 256;
    
    // Center point of the widget
    int cx = width() / 2;
    int cy = height() / 2;
    
    // We want the point (offsetX, offsetY) of tile(tileX, tileY) to be exactly at (cx, cy).
    // So the top-left of tile(tileX, tileY) should be at:
    int startPx = cx - offsetX;
    int startPy = cy - offsetY;
    
    // Draw a 3x3 grid around the center tile
    for (int dx = -2; dx <= 2; ++dx) {
        for (int dy = -2; dy <= 2; ++dy) {
            int cxTile = tileX + dx;
            int cyTile = tileY + dy;
            
            QString key = QString("%1_%2_%3").arg(m_zoom).arg(cxTile).arg(cyTile);
            if (m_tileCache.contains(key) && !m_tileCache[key].isNull()) {
                painter.drawPixmap(startPx + dx * 256, startPy + dy * 256, m_tileCache[key]);
            } else {
                requestTile(cxTile, cyTile, m_zoom);
                // Draw a placeholder grid
                painter.setPen(QColor("#afb8c1"));
                painter.drawRect(startPx + dx * 256, startPy + dy * 256, 256, 256);
            }
        }
    }
    
    // Draw the pulsing beacon at the center
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(248, 81, 73, 100)); // #f85149 with alpha
    painter.drawEllipse(QPoint(cx, cy), m_pulseSize, m_pulseSize);
    
    painter.setBrush(QColor("#f85149"));
    painter.drawEllipse(QPoint(cx, cy), 5, 5);
}
