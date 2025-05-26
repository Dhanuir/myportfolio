#include "pch.h"
#include "ModalWidget.h"

namespace ui
{
    ModalWidget::ModalWidget(float posX, float posY, float width, float height)
        : Widget(posX, posY, width, height)
    {
        // Setup background
        m_background.setPosition(static_cast<float>(posX), static_cast<float>(posY));
        m_background.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
        m_background.setFillColor(sf::Color(0, 0, 0, 150));
    }

    void ModalWidget::AddChildWidget(std::shared_ptr<Widget> widget)
    {
        m_childWidgets.push_back(widget);
    }

    void ModalWidget::Draw(RenderContext& context) const
    {
        context.draw(m_background);

        // Draw child widgets
        for (const auto& widget : m_childWidgets)
        {
            widget->Draw(context);
        }
    }

    InputEventState ModalWidget::ProcessInput(const sf::Event& event)
    {
        for (auto& widget : m_childWidgets)
        {
            if (widget->ProcessInput(event) == InputEventState::Handled)
            {
                return InputEventState::Handled;
            }
        }
        return InputEventState::Unhandled;
    }
}
