#include "Widget.hpp"

GraphicEngine::GUI::Widget::Widget(const std::string& label) :
    label{ label }
{
}

void GraphicEngine::GUI::Widget::addParent(std::shared_ptr<Widget> parent)
{
    m_parent = parent;
    // TODO - should be implamented different way
    /*if (m_parent)
        parent->addChildren(shared_from_this());*/
}

std::shared_ptr<GraphicEngine::GUI::Widget> GraphicEngine::GUI::Widget::getParent()
{
    return m_parent;
}

void GraphicEngine::GUI::Widget::addChildren(std::shared_ptr<Widget> children)
{
    m_childrens.push_back(children);
}

std::list<std::shared_ptr<GraphicEngine::GUI::Widget>> GraphicEngine::GUI::Widget::getChildrens()
{
    return m_childrens;
}
