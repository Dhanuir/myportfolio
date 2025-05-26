#pragma once
#include <SFML/Graphics.hpp>


class BackgroundChanger
{
public:
    BackgroundChanger();
    void ChangeBackgroundColor(sf::RenderWindow& window);

private:
    sf::Color m_backgroundColor;
};

