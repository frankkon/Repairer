#ifndef WIDGETTOP_H
#define WIDGETTOP_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class WidgetTop : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetTop(QWidget *parent = 0);

signals:

public slots:
    void closeWidget();
    void minimizeWidget();
    void registerDlg();
    void about();

private:
    QLabel* m_pQLabLogo;
    QLabel* m_pQLabVersion;
    QPushButton* m_pBtnRegister;
    QPushButton* m_pBtnAbout;
    QPushButton* m_pBtnMinimize;
    QPushButton* m_pBtnClose;
};

#endif // WIDGETTOP_H
