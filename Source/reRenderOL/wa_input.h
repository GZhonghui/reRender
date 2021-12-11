#ifndef WA_INPUT_H
#define WA_INPUT_H

#include <QHBoxLayout>
#include <QWidgetAction>
#include <QLineEdit>
#include <QLabel>

class WA_Input : public QWidgetAction
{
    Q_OBJECT
public:
    explicit WA_Input(QObject *parent = nullptr);

protected:
    QWidget* createWidget(QWidget *parent) override;

protected:
    QWidget* m_Widget;
    QHBoxLayout* m_Layout;
    QLabel* m_Text;

public:
    QLineEdit* m_Line;
};

#endif // WA_INPUT_H
