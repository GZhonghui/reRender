#ifndef WA_BUTTON_H
#define WA_BUTTON_H

#include <QWidgetAction>
#include <QPushButton>
#include <QHBoxLayout>

class waButton : public QWidgetAction
{
public:
    explicit waButton(const QString& Text, QObject *parent = nullptr,
        int MarginsA = 10, int MarginsB = 10);

protected:
    QWidget* createWidget(QWidget *parent) override;

protected:
    QString m_Text;
    int m_MarginsA;
    int m_MarginsB;

    QWidget* m_Widget;
    QHBoxLayout* m_Layout;

public:
    QPushButton* m_Button;
};

#endif // WA_BUTTON_H
