#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "../framework/widget.h"
#include "../application/inventory.h"
#include "TextWidget.h"

namespace ui {
    class TooltipWidget : public Widget {
    public:
        TooltipWidget(float posX, float posY, float width, float height, const Item& item);
        void SetItemDetails(const std::string& name, int cost, int size, RarityLevel rarity, const std::vector<std::string>& attributes);
        void Draw(sf::RenderWindow& window) const override;
        std::string RarityLevelToString(RarityLevel rarity);
        sf::Color GetRarityColor(RarityLevel rarity);
        void SetPosition(float posX, float posY);
        void AdjustPosition(const sf::RenderWindow& window);

    private:
        sf::RectangleShape m_background;
        std::shared_ptr<sf::Font> m_font;

        TextWidget m_nameText;
        TextWidget m_costText;
        TextWidget m_sizeText;
        TextWidget m_rarityText;
        TextWidget m_attributesText;
    };
}
