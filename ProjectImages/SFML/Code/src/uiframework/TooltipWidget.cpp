#include "pch.h"
#include "TooltipWidget.h"

ui::TooltipWidget::TooltipWidget(float posX, float posY, float width, float height, const Item& item)
    : Widget(posX, posY, width, height),
    m_nameText("Name: " + item.m_name, posX + 10, posY + 10, 20, sf::Color::White),
    m_costText("Cost: $" + std::to_string(item.m_cost), posX + 10, posY + 250, 30, sf::Color::White),
    m_sizeText("Size: " + std::to_string(item.m_size), posX + 10, posY + 110, 20, sf::Color::White),
    m_rarityText("Rarity: " + std::to_string(static_cast<int>(item.m_rarity)), posX + 10, posY + 130, 20, sf::Color::White),
    m_attributesText("Attributes: ", posX + 10, posY + 150, 20, sf::Color::White)
{
    // Set up the background rectangle
    m_background.setPosition(static_cast<float>(posX), static_cast<float>(posY));
    m_background.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
    m_background.setFillColor(sf::Color(100, 100, 100, 255));

    // Set the default attributes text
    std::string attributesStr = "Attributes: ";
    for (const auto& attribute : item.m_attributes) {
        if (item.m_attributes.back() != attribute)
        {
            attributesStr += attribute + ", ";
        }
        else
        {
            attributesStr += attribute;
        }
    }
    m_attributesText.SetText(attributesStr);
}

void ui::TooltipWidget::SetItemDetails(const std::string& name, int cost, int size, RarityLevel rarity, const std::vector<std::string>& attributes) 
{
    m_nameText.SetText(name);
    m_nameText.SetSize(50);
    m_nameText.SetColor(GetRarityColor(rarity));
    m_costText.SetText("Cost: $" + std::to_string(cost));
    m_sizeText.SetText("Size: " + std::to_string(size));
    m_rarityText.SetText("Rarity: " + RarityLevelToString(rarity));

    std::string attributesStr = "Attributes: ";
    for (const auto& attribute : attributes) {
        if (attributes.back() != attribute)
        {
            attributesStr += attribute + ", ";
        }
        else
        {
            attributesStr += attribute;
        }
    }
    m_attributesText.SetText(attributesStr);
}

void ui::TooltipWidget::Draw(sf::RenderWindow& window) const 
{
    window.draw(m_background);
    m_nameText.Draw(window);
    m_costText.Draw(window);
    m_sizeText.Draw(window);
    m_rarityText.Draw(window);
    m_attributesText.Draw(window);
}

std::string ui::TooltipWidget::RarityLevelToString(RarityLevel rarity) 
{
    switch (rarity) {
    case RarityLevel::Rare:
        return "Rare";
    case RarityLevel::Normal:
        return "Normal";
    case RarityLevel::Common:
        return "Common";
    default:
        return "Unknown";
    }
}

sf::Color ui::TooltipWidget::GetRarityColor(RarityLevel rarity)
{
    switch (rarity) {
    case RarityLevel::Rare:
        return sf::Color(102, 153, 255, 255);
    case RarityLevel::Normal:
        return sf::Color(0, 255, 0, 255);
    case RarityLevel::Common:
        return sf::Color(255, 255, 255, 255);;
    default:
        return sf::Color(191, 191, 191, 255);;
    }
}

void ui::TooltipWidget::SetPosition(float posX, float posY)
{
    m_background.setPosition(posX, posY);
    m_nameText.SetPosition(posX + 10, posY + 10);
    m_costText.SetPosition(posX + 10, posY + 250);
    m_sizeText.SetPosition(posX + 10, posY + 110);
    m_rarityText.SetPosition(posX + 10, posY + 130);
    m_attributesText.SetPosition(posX + 10, posY + 150);
}


// Makes the tooltip stay within window right and bottom boundaries
void ui::TooltipWidget::AdjustPosition(const sf::RenderWindow& window)
{
    sf::Vector2u windowSize = window.getSize();
    float maxX = static_cast<float>(windowSize.x);
    float maxY = static_cast<float>(windowSize.y);

    if ((this->GetPosX() + this->GetWidth()) > maxX) {
        float newPosX = maxX - static_cast<float>(this->GetWidth());
        SetPosition(newPosX, this->GetPosY());
    }

    if ((this->GetPosY() + this->GetHeight()) > maxY) {
        float newPosY = maxY - static_cast<float>(this->GetHeight());
        SetPosition(this->GetPosX(), newPosY);
    }
}