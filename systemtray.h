#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>

class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit SystemTray(QObject *parent = 0);

    void translateLanguage();

private:
    void createAction();
    void addActions();

signals:
    void showWidget();

private:
    QMenu* m_pMenu;          //托盘菜单
    QAction* m_pActOpen;     //打开
    QAction* m_pActLayout;   //退出
};

#endif // SYSTEMTRAY_H
