#pragma once
#include <SFML/Graphics.hpp>
#include "../framework/widget.h"
#include <vector>
#include <memory>
#include "InventorySlot.h"
#include "../application/inventory.h"

namespace ui
{
    struct BoxConstraints {
        float minWidth;
        float maxWidth;
        float minHeight;
        float maxHeight;
    };

    class InventoryContainer : public Widget
    {
    public:
        InventoryContainer(float posX, float posY, float width, float height, int slotAmount, int slotsPerCol, std::shared_ptr< Inventory > Inventory);
        ~InventoryContainer() override = default;

        void AddWidget(std::shared_ptr<Widget> widget);
        InputEventState ProcessInput(const InputEvent& event) override;
        void Draw(RenderContext& context) const override;

        BoxConstraints GetConstraints() const;

        sf::RectangleShape m_containerShape_Outer;
        sf::RectangleShape m_containerShape_Inner;
        void Scroll(float deltaY);
        std::vector<std::shared_ptr<InventorySlot>>& GetSlots();
        void AssignItems(const std::vector<Item>& items);
        void ClearItems();
        void SetInventoryPath(std::string path);
        std::string GetInventoryPath();
        std::shared_ptr<Inventory> GetInventoryPtr() { return m_Inventory; }
        int GetInventorySlotAmount() { return m_slotAmount; }
        int GetInventoryItemAmount() { return m_itemAmount; }
        void UpdateItemInJson(const std::string& itemName, int countChange, std::shared_ptr<InventoryContainer> buyerInventoryContainer);
        std::shared_ptr<TextWidget> m_moneyTextWidget;
        std::shared_ptr<TextWidget> GetMoneyText() { return m_moneyTextWidget; }
        void SetMoneyText(std::string currency) { m_moneyTextWidget->SetText(currency); }

    private:
        void CreateGrid(int slotAmount, int slotsPerCol, float slotSize, float padding);
        
        std::vector<std::shared_ptr<Widget>> m_widgets;
        sf::View m_view;
        std::vector<std::shared_ptr<InventorySlot>> m_slots;
        std::shared_ptr< Inventory > m_Inventory;
        std::string m_path;
        float m_currentScrollOffset;
        float m_containerShape_BorderThickness;
        int m_slotAmount;
        int m_slotsPerCol;
        float m_slotSize;
        float m_padding;
        int m_rows;
        int m_itemAmount = 0;
    };
}
