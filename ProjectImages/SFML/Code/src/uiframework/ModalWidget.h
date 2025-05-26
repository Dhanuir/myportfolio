#pragma once
#include "../framework/widget.h"
#include <SFML/Graphics.hpp>
#include <functional>

namespace ui
{
    class ModalWidget : public Widget
    {
    public:
        ModalWidget(float posX, float posY, float width, float height);

        void AddChildWidget(std::shared_ptr<Widget> widget);

        void Draw(RenderContext& context) const override;

        InputEventState ProcessInput(const sf::Event& event) override;

    private:
        sf::RectangleShape m_background;
        std::vector<std::shared_ptr<Widget>> m_childWidgets; // Store any child widgets
    };
}
