#pragma once
#include <SFML/Graphics.hpp>
#include "../framework/widget.h"
#include <iostream>

namespace ui
{
    class TextWidget : public Widget
    {
    public:
        TextWidget(const std::string& text, float posX, float posY, int fontSize, sf::Color color);
        ~TextWidget() override = default;

        InputEventState ProcessInput(const InputEvent& event) override;
        void Draw(RenderContext& context) const override;
        void SetText(const std::string& new_text);
        void SetSize(int size);
        void SetColor(sf::Color color);
        void SetPosition(float posX, float posY);
        std::string GetText() { return m_text.getString(); };



    private:
        sf::Text m_text;
        sf::Font m_font;
    };
}
