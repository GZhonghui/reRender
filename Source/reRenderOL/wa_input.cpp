#include "wa_input.h"

WA_Input::WA_Input(QObject *parent)
    : QWidgetAction{parent}{}

QWidget* WA_Input::createWidget(QWidget *parent)
{
    if(parent->inherits("QMenu"))
    {
        m_Widget = new QWidget(parent);

        m_Layout = new QHBoxLayout(m_Widget);
        m_Text = new QLabel("ID: ",m_Widget);
        m_Line = new QLineEdit(m_Widget);
        
        m_Layout->addWidget(m_Text);
        m_Layout->addWidget(m_Line);
        
        m_Widget->setLayout(m_Layout);

        return m_Widget;
    }
    return nullptr;
}
