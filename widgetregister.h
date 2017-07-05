#ifndef WIDGETREGISTER_H
#define WIDGETREGISTER_H


#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class WidgetRegister : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetRegister(QWidget* parent = 0);

signals:

public slots:
    void closeDialog();
    void registerKey();
    void buy();


private:
    QPushButton* m_pBtnRegister;
    QPushButton* m_pBtnBuy;
    QPushButton* m_pBtnClose;
    QLineEdit*   m_pEdtKey;

};

#endif // WIDGETREGISTER_H
