#ifndef COLOR_BUTTON_H
#define COLOR_BUTTON_H

#include <QPushButton>
#include <QColorDialog>
#include <QColor>

class colorButton : public QPushButton
{
    Q_OBJECT
public:
    colorButton(QWidget* parent);

public:
    void setColor(const QColor& color);
    const QColor& getColor();

public:
    void updateColor();
    void changeColor();

private:
    QColor m_color;
};

#endif // COLOR_BUTTON_H
