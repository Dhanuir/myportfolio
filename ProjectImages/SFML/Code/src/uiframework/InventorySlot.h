#pragma once
#include <SFML/Graphics.hpp>
#include "../framework/widget.h"
#include "../application/inventory.h"
#include "TextWidget.h"
#include "TooltipWidget.h"

namespace ui
{
    class InventorySlot : public Widget
    {
    public:
        InventorySlot(float posX, float posY, float size);
        ~InventorySlot() override = default;

        bool LoadTexture(sf::Sprite& sprite, sf::Texture& texture, const std::string& textureFilePath);

        void Draw(sf::RenderWindow& window) const override;
        InputEventState ProcessInput(const sf::Event& event) override;
        bool IsPointerOver(const sf::Vector2i& pointerPosition);

        void SetItem(const Item& item);
        Item GetItem() { return m_item; };
        sf::Sprite GetItemSprite() { return m_itemSprite; };
        void SetItemQuantity(int quantity);
        void RemoveTransparency();
        bool IsHovered();
        bool HasItem();
        void DrawTooltip(sf::RenderWindow& window);
        std::shared_ptr<ui::TooltipWidget> GetTooltip();


    private:
        // Sprites
        sf::Sprite m_backgroundSprite;
        sf::Sprite m_highlightSprite;
        sf::Sprite m_itemSprite;
        // Textures
        sf::Texture m_backgroundTexture;
        sf::Texture m_highlightTexture;
        sf::Texture m_itemTexture;
        // Files Paths
        const std::string m_slotTextureFilePath;
        const std::string m_highlightTextureFilePath;
        // InventorySlot info bools
        bool m_isHovered = false;
        bool m_hasItem = false;
        // Item in the InventorySlot
        Item m_item;
        // Item display infomation
        std::shared_ptr<ui::TextWidget> m_quantityTextWidget;
        std::shared_ptr<ui::TooltipWidget> m_tooltip;
    };
}
