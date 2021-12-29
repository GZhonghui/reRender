#include "colorButton.h"

colorButton::colorButton(QWidget* parent) : QPushButton(parent)
{
    connect(this, &QPushButton::clicked, this, &colorButton::changeColor);
    updateColor();
}

void colorButton::updateColor()
{
    setStyleSheet("background-color: " + m_color.name());
    setText(m_color.name());
}

void colorButton::changeColor()
{
    QColor newColor = QColorDialog::getColor(m_color,parentWidget());
    if (newColor.isValid() && newColor != m_color)
    {
        setColor(newColor);
    }
}

void colorButton::setColor(const QColor& color)
{
    m_color = color;
    updateColor();
}

const QColor& colorButton::getColor()
{
    return m_color;
}
