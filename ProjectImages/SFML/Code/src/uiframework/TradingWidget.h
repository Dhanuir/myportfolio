#pragma once
#include "../uiframework/ButtonWidget.h"
#include "../uiframework/ModalWidget.h"
#include "../uiframework/TextWidget.h"
#include "../uiframework/InventoryContainer.h"
#include "../framework/window.h"
#include "../application/inventory.h"

namespace ui
{
    class TradingWidget : public ModalWidget, public std::enable_shared_from_this<TradingWidget>
    {
    public:
        TradingWidget(float posX, float posY, float width, float height, std::shared_ptr<ui::InventoryContainer> inventoryContainer, std::shared_ptr<ui::InventoryContainer> buyerInventoryContainer, std::shared_ptr< Inventory > seller_inventory, std::shared_ptr< Inventory > buyer_inventory, std::shared_ptr<ui::Window> mainWindow);

        void SetItem(std::shared_ptr<Item> item);
        void ResetQuantity();

        bool ContainsItem(const std::string& itemName, std::vector<Item> itemList) const;
        bool ContainsTextWidget(const std::string& widgetName, const std::shared_ptr<TextWidget>& itemNameText) const;
        std::shared_ptr<ui::InventoryContainer> GetSellerInventoryContainer() { return m_sellerInventoryContainer; }
        std::shared_ptr<ui::InventoryContainer> GetBuyerInventoryContainer() { return m_buyerInventoryContainer; }
        std::shared_ptr<Item> GetCurrentItem() { return m_currentItem; }
        std::shared_ptr<TextWidget> GetItemNameText() { return m_itemNameText; }
        std::shared_ptr<int> GetQuantityToTrade() { return m_quantityToTrade; }
        std::shared_ptr<ui::ButtonWidget> m_tradeButton;

    private:
        std::shared_ptr<ui::ButtonWidget> m_incrementButton;
        std::shared_ptr<ui::ButtonWidget> m_decrementButton;
        std::shared_ptr<ui::TextWidget> m_quantityText;

        std::shared_ptr<int> m_quantityToTrade;

        std::shared_ptr<ui::InventoryContainer> m_sellerInventoryContainer;
        std::shared_ptr<ui::InventoryContainer> m_buyerInventoryContainer;
        std::shared_ptr< Inventory > m_sellerInventory;
        std::shared_ptr< Inventory > m_buyerInventory;
        std::shared_ptr<ui::Window> m_mainWindow;

        void UpdateQuantityText();

        std::shared_ptr<Item> m_currentItem;
        std::shared_ptr<TextWidget> m_itemNameText;
    };
}