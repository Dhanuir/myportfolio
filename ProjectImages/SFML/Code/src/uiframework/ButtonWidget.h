#pragma once
#include "../framework/widget.h"
#include <SFML/Graphics.hpp>
#include <functional>

namespace ui
{
    class ButtonWidget : public Widget
    {
    public:
        ButtonWidget(float posX, float posY, float width, float height, const std::string& label);

        void SetOnClick(std::function<void()> callback);
        void Draw(RenderContext& context) const override;
        InputEventState ProcessInput(const sf::Event& event) override;
        void HighlightButton(bool PointerHovering);


    private:
        sf::RectangleShape m_background;
        sf::Text m_labelText;

        std::function<void()> m_onClick;
    };
}
