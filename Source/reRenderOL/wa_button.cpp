#include "wa_button.h"

WA_Button::WA_Button(const QString& Text, QObject *parent,
    int MarginsA, int MarginsB)
    : QWidgetAction{parent},
      m_Text(Text),m_MarginsA(MarginsA),m_MarginsB(MarginsB) {}

QWidget* WA_Button::createWidget(QWidget *parent)
{
    if(parent->inherits("QMenu"))
    {
        m_Widget = new QWidget(parent);

        m_Button = new QPushButton(m_Text,parent);
        m_Layout = new QHBoxLayout(parent);
        
        m_Layout->addWidget(m_Button);
        m_Layout->setContentsMargins(10,m_MarginsA,10,m_MarginsB);

        m_Widget->setLayout(m_Layout);

        return m_Widget;
    }
    return nullptr;
}

