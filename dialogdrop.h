#ifndef DIALOGDROP_H
#define DIALOGDROP_H

#include <QDialog>
#include <QPoint>
#include <QtMath>

class DialogDrop : public QDialog
{
    Q_OBJECT
public:
    explicit DialogDrop(QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *event);
    //void reject();

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);

signals:

public slots:


private:
    QPoint m_point;       //移动的距离
    bool m_bMousePress;   //按下鼠标左键

};

#endif // DIALOGDROP_H
