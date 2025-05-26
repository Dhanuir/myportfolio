#include "pch.h"
#include "BackgroundChanger.h"


BackgroundChanger::BackgroundChanger()
    : m_backgroundColor(sf::Color(25, 45, 80))
{
}

void BackgroundChanger::ChangeBackgroundColor(sf::RenderWindow& window)
{
    window.clear(m_backgroundColor);
}
