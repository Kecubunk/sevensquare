/*
 * cubescene.h
 *
 * Copyright 2012-2012 Yang Hong
 *
 */

#ifndef CUBESCENE_H
#define CUBESCENE_H

#include <QSize>
#include <QString>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMutex>
#include <QKeyEvent>
#include <QWidget>
#include <QThread>
#include <QTimer>
#include <QMap>

#include <QGraphicsView>

#include "cubecellitem.h"
#include "fbcellitem.h"
#include "adbfb.h"
#include "debug.h"

#define WINDOW_BORDER 2
#define KEY_BTN_SIZE  32
#define POINTER_ANCHOR_SIZE 24

class CubeView : public QGraphicsView
{
    Q_OBJECT

public:
    CubeView(QWidget * parent = 0);

protected:
    void resizeEvent ( QResizeEvent * event );

public slots:
    void cubeSizeChanged(QSize);

signals:
    void viewSizeChanged(QSize);

private:
    QTimer timer;
    QSize delayedSize;
};

class CubeScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CubeScene(QObject * parent = 0);
    ~CubeScene();

    void initialize (void);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyReleaseEvent(QKeyEvent * event);
    CubeCellItem *createCellItem(const char* name, int size, int key = 0);

    QStringList newEventCmd (int type, int code, int value);
    void sendTap(QPoint pos, bool, bool);
    void sendEvent(QPoint pos, bool, bool);
    void sendVirtualClick(QPointF, bool, bool);
    void sendVirtualKey(int key);
    void setMenuIconsPos(void);
    void setPointerPos(QPointF, bool);
    bool poinInFB(QPointF);

public slots:
    void newFBFound(int, int, int, int);
    void updateFBCell(QByteArray *);
    void deviceConnected(void);
    void deviceDisconnected(void);
    void deviceScreenTurnedOff(void);
    void deviceScreenTurnedOn(void);
    void cubeResize(QSize);

signals:
    void sceneSizeChanged(QSize);
    void execAdbCmd(const QStringList);
    void waitForDevice(void);
    void wakeUpDevice(void);
    void updateDeviceBrightness(void);
    void readFrame(void);

private:
    FBCellItem fb;
    QGraphicsRectItem grayMask;
    QGraphicsSimpleTextItem promptItem;
    CubeCellItem *home;
    CubeCellItem *back;
    CubeCellItem *menu;
    CubeCellItem *pointer;

    int os_type;
    int fb_width;
    int fb_height;
    int pixel_format;
    int cube_width;
    int cube_height;
    unsigned long waitCount;

    QMutex update_mutex;
    QPixmap pixmap;

    FBEx reader;
    QThread fbThread;

    AdbExecObject adbex;
    QThread adbThread;
    QStringList cmds;
    QPoint posPress;

    // Previous mouse event pos, used to filter
    // out too mouch event.
    QPoint posPrevious;

    // Qt key, Android key map
    QMap<int, int> keys;
};

#endif // CUBESCENE_H
