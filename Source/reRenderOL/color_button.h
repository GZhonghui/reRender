#ifndef COLOR_BUTTON_H
#define COLOR_BUTTON_H

#include <QPushButton>
#include <QColorDialog>
#include <QColor>

class Color_Button : public QPushButton
{
    Q_OBJECT
public:
    Color_Button(QWidget* parent);

    void setColor(const QColor& color);
    const QColor& getColor();

public:
    void updateColor();
    void changeColor();

private:
    QColor color;
};

#endif // COLOR_BUTTON_H
