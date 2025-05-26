#include "pch.h" // Include precompiled header if needed
#include "PointerWidget.h"
#include <iostream>

namespace ui
{
    PointerWidget::PointerWidget(float posX, float posY, float width, float height, sf::RenderWindow* renderWindow)
        : m_renderWindow(renderWindow), m_position(static_cast<float>(posX), static_cast<float>(posY))
    {
        // Pointer shape
        m_pointerShape.setRadius(static_cast<float>(width) / 2);
        m_pointerShape.setFillColor(sf::Color::Red);
        m_pointerShape.setPosition(m_position);
    }

    void PointerWidget::ProcessInput(const sf::Event& event)
    {
        // Mouse movement events
        if (event.type == sf::Event::MouseMoved)
        {
            m_position.x = static_cast<float>(event.mouseMove.x);
            m_position.y = static_cast<float>(event.mouseMove.y);

            // Update the pointer position when the mouse moves
            m_pointerShape.setPosition(m_position);
        }
    }

    void PointerWidget::Draw()
    {
        m_renderWindow->draw(m_pointerShape);
    }

}