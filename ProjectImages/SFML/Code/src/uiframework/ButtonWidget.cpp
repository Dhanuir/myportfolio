#include "pch.h"
#include "ButtonWidget.h"
#include "FontManager.h"
#include <iostream>

namespace ui
{
    ButtonWidget::ButtonWidget(float posX, float posY, float width, float height, const std::string& label)
        : Widget(posX, posY, width, height)
    {

        // Background
        m_background.setPosition(static_cast<float>(posX), static_cast<float>(posY));
        m_background.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
        m_background.setFillColor(sf::Color(100, 100, 100, 255));

        // Text
        m_labelText.setFont(FontManager::GetFont());
        m_labelText.setString(label);
        m_labelText.setCharacterSize(20);
        m_labelText.setFillColor(sf::Color::White);
        m_labelText.setPosition(static_cast<float>(posX + 10), static_cast<float>(posY + 5));
    }

    void ButtonWidget::SetOnClick(std::function<void()> callback)
    {
        m_onClick = callback;
    }

    void ButtonWidget::Draw(RenderContext& context) const
    {
        context.draw(m_background);
        context.draw(m_labelText);
    }

    void ButtonWidget::HighlightButton(bool PointerHovering)
    {
        if (PointerHovering)
        {
            m_background.setFillColor(sf::Color(150, 150, 150, 255));
        }
        else
        {
            m_background.setFillColor(sf::Color(100, 100, 100, 255));
        }
    }

    InputEventState ButtonWidget::ProcessInput(const sf::Event& event)
    {
        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (m_background.getGlobalBounds().contains(mousePos))
            {
                if (m_onClick)
                {
                    m_onClick();
                }
                return InputEventState::Handled;
            }
        }

        return InputEventState::Unhandled;
    }
}
