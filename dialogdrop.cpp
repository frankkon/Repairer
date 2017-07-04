#include "dialogdrop.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDebug>
#include <QKeyEvent>

DialogDrop::DialogDrop(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);

    //初始化为未按下鼠标左键
    m_bMousePress = false;
}

void DialogDrop::mousePressEvent(QMouseEvent *event)
{
    //只能是鼠标左键移动和改变大小
    if( event->button() == Qt::LeftButton )
    {
        m_bMousePress = true;
    }

    //窗口移动距离
    m_point = event->globalPos() - pos();
//    qDebug()<<"event->globalPos()"<<event->globalPos().x()<<" "<<event->globalPos().y();
//    qDebug()<<"pos()"<<pos().x()<<" "<<pos().y();
//    qDebug()<<"m_point"<<m_point.x()<<" "<<m_point.y();
}

void DialogDrop::mouseReleaseEvent(QMouseEvent *event)
{
    m_bMousePress = false;
}

void DialogDrop::mouseMoveEvent(QMouseEvent *event)
{ 
    //移动窗口
    if(m_bMousePress)
    {
        QPoint ptMove = event->globalPos();
        move(ptMove - m_point);

        //qDebug()<<"ptMove"<<ptMove.x()<<" "<<ptMove.y();
    }
}

void DialogDrop::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(10, 10, this->width()-20, this->height()-20);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 50);
    for(int i=0; i<10; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
        color.setAlpha(150 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}

void DialogDrop::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
       //进行界面退出，重写Esc键，否则重写reject()方法
      case Qt::Key_Escape:
        break;

     default:
        QDialog::keyPressEvent(event);
    }



}

//void DialogDrop::reject()
//{
//    int n = 0;
//}
