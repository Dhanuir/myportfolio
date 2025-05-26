#pragma once
#include <SFML/Graphics.hpp> // Include SFML elements

namespace ui
{
    class PointerWidget
    {
    public:
        PointerWidget(float posX, float posY, float width, float height, sf::RenderWindow* renderWindow);
        ~PointerWidget() = default;

        void ProcessInput(const sf::Event& event);
        void Draw();
        sf::Vector2f GetPosition() const { return m_position; }

    private:
        sf::RenderWindow* m_renderWindow;
        sf::CircleShape m_pointerShape;
        float m_speed = 200.0f;
        sf::Vector2f m_position;
    };
}
