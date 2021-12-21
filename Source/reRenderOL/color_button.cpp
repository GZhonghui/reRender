#include "color_button.h"

Color_Button::Color_Button(QWidget* parent) : QPushButton(parent)
{
    connect(this, &clicked, this, &changeColor);
    updateColor();
}

void Color_Button::updateColor()
{
    setStyleSheet("background-color: " + color.name());
    setText(color.name());
}

void Color_Button::changeColor()
{
    QColor newColor = QColorDialog::getColor(color,parentWidget());
    if (newColor.isValid() && newColor != color)
    {
        setColor(newColor);
    }
}

void Color_Button::setColor(const QColor& color)
{
    this->color = color;
    updateColor();
}

const QColor& Color_Button::getColor()
{
    return color;
}
