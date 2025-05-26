#include "pch.h"
#include "InventorySlot.h"
#include "PointerWidget.h"
#include "../uiframework/FontManager.h"
#include "iostream"
#include "TooltipWidget.h"

namespace ui
{
    InventorySlot::InventorySlot(float posX, float posY, float size)
        : Widget(posX, posY, size, size), m_slotTextureFilePath("../../resources/images/ui/SlotBg.png"), m_highlightTextureFilePath("../../resources/images/ui/SlotHl.png")
    {
        // Load slot background texture
        if (!LoadTexture(m_backgroundSprite, m_backgroundTexture, m_slotTextureFilePath))
        {
            std::cerr << "Error loading texture for InventorySlot" << std::endl;
        }
        m_backgroundSprite.setPosition(static_cast<float>(posX), static_cast<float>(posY));

        if (!LoadTexture(m_highlightSprite, m_highlightTexture, m_highlightTextureFilePath))
        {
            std::cerr << "Error loading texture for HighlightSlot" << std::endl;
        }
        // Set quantity text
        try
        {
            m_quantityTextWidget = std::make_shared<TextWidget>("", posX + size - 50.0f, posY + size - 35.0f, 20, sf::Color::White);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception occurred during TextWidget creation: " << e.what() << std::endl;
            m_quantityTextWidget.reset();
        }
        m_highlightSprite.setPosition(static_cast<float>(posX), static_cast<float>(posY));
        m_tooltip = std::make_shared <TooltipWidget>(posX + size, posY + size, 500.0f, 300.0f, m_item);
    }

    bool InventorySlot::LoadTexture(sf::Sprite& sprite, sf::Texture& texture, const std::string& textureFilePath)
    {
        if (texture.loadFromFile(textureFilePath))
        {
            sprite.setTexture(texture);
            // Scaling item
            sprite.setScale(
                static_cast<float>(GetWidth()) / texture.getSize().x,
                static_cast<float>(GetHeight()) / texture.getSize().y
            );
            // Make empty slots transparent
            sprite.setColor(sf::Color(255, 255, 255, 200));
            return true;
        }
        else
        {
            std::cerr << "Failed to load texture from: " << textureFilePath << std::endl;
        }
        return false;
    }

    void InventorySlot::Draw(sf::RenderWindow& window) const
    {
        window.draw(m_backgroundSprite);
        if (m_item.m_count > 0)
        {
            window.draw(m_itemSprite);
        }

        if (m_quantityTextWidget)
        {
            m_quantityTextWidget->Draw(window);
        }
        
        if (m_isHovered)
        {
            window.draw(m_highlightSprite);
        }
    }
    
    InputEventState InventorySlot::ProcessInput(const sf::Event& event)
    {
        
        return InputEventState::Unhandled;
    }
    
    bool InventorySlot::IsPointerOver(const sf::Vector2i& pointerPosition)
    {
        sf::FloatRect bounds = m_backgroundSprite.getGlobalBounds();
        if(bounds.contains(static_cast<sf::Vector2f>(pointerPosition))){ m_isHovered = true; }
        else{ m_isHovered = false; }
        
        return bounds.contains(static_cast<sf::Vector2f>(pointerPosition));
    }

    void InventorySlot::SetItem(const Item& item)
    {
        m_item = item;

        // Load the texture based on item name
        std::string texturePath = "../../resources/images/items/" + item.m_name + ".png";

        if (m_itemTexture.loadFromFile(texturePath))
        {
            m_itemSprite.setTexture(m_itemTexture);
            m_itemSprite.setPosition(static_cast<float>(GetPosX()), static_cast<float>(GetPosY()));
            m_itemSprite.setScale(
                static_cast<float>(GetWidth()) / m_itemTexture.getSize().x,
                static_cast<float>(GetHeight()) / m_itemTexture.getSize().y
            );
            if (m_quantityTextWidget && m_quantityTextWidget > 0)
            {
                m_quantityTextWidget->SetText(std::to_string(item.m_count));
                m_hasItem = true;
            }
        }
        else
        {
            std::cerr << "Error: Could not load texture " << texturePath << std::endl;
        }
    }

    void InventorySlot::SetItemQuantity(int quantity)
    {
        if (m_quantityTextWidget)
        {
            if (quantity == 0)
            {
                m_quantityTextWidget->SetText(" ");
                m_hasItem = false;
                
            }
            else
            { 
                m_quantityTextWidget->SetText(std::to_string(quantity)); 
                m_hasItem = true;
            }
            
        }
    }

    void InventorySlot::RemoveTransparency()
    {
        m_backgroundSprite.setColor(sf::Color(255, 255, 255, 255));
    }

    bool InventorySlot::IsHovered()
    {
        return m_isHovered;
    }

    bool InventorySlot::HasItem()
    {
        return m_hasItem;
    }

    void InventorySlot::DrawTooltip(sf::RenderWindow& window)
    {
        if (m_isHovered && m_tooltip)
        {
            m_tooltip->Draw(window);
        }
    }

    std::shared_ptr<ui::TooltipWidget> InventorySlot::GetTooltip()
    {
        return m_tooltip;
    }
}
