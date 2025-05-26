#include "pch.h"
#include "TextWidget.h"
#include "../uiframework/FontManager.h"

namespace ui
{
    TextWidget::TextWidget(const std::string& text, float posX, float posY, int fontSize, sf::Color color)
        : Widget(posX, posY, 0, 0)
    {
        m_text.setFont(FontManager::GetFont());
        m_text.setString(text);
        m_text.setCharacterSize(static_cast<unsigned int>(fontSize));
        m_text.setFillColor(color);
        m_text.setPosition(posX, posY);
    }

    InputEventState TextWidget::ProcessInput(const InputEvent& event)
    {
        return InputEventState::Unhandled;
    }

    void TextWidget::Draw(RenderContext& context) const
    {
        context.draw(m_text);
    }

    void TextWidget::SetText(const std::string& new_text)
    {
        m_text.setString(new_text);
    }

    void TextWidget::SetSize(int size)
    {
        m_text.setCharacterSize(static_cast<unsigned int>(size));
    }

    void TextWidget::SetColor(sf::Color color)
    {
        m_text.setFillColor(color);
    }

    void TextWidget::SetPosition(float posX, float posY)
    {
        m_text.setPosition(posX, posY);
    }
}
