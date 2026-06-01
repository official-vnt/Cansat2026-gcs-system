#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHash>
#include <QPixmap>
#include <QTimer>

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = nullptr);

public slots:
    void setCoordinate(double lat, double lon);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void handleNetworkData(QNetworkReply *reply);
    void updatePulse();

private:
    void requestTile(int x, int y, int z);
    
    double m_lat = 0.0;
    double m_lon = 0.0;
    int m_zoom = 16;
    
    QNetworkAccessManager m_netManager;
    QHash<QString, QPixmap> m_tileCache; // key: "z_x_y"
    
    QTimer m_pulseTimer;
    int m_pulseSize = 0;
    bool m_pulseGrowing = true;
};

#endif // MAPWIDGET_H
