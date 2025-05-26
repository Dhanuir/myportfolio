#pragma once
#include "../uiframework/ButtonWidget.h"
#include "../uiframework/ModalWidget.h"
#include "../uiframework/InventoryContainer.h"
#include "../framework/window.h"
#include "../application/inventory.h"
#include <memory>

namespace ui
{
    class SortingWidget : public ModalWidget, public std::enable_shared_from_this<SortingWidget>
    {
    public:
        SortingWidget(float posX, float posY, float width, float height, std::shared_ptr<ui::InventoryContainer> inventoryContainer, std::shared_ptr<Inventory> inventory, std::shared_ptr<ui::Window> mainWindow);
        void SortItemsByName(std::vector<Item>& m_items);
        void SortItemsByCost(std::vector<Item>& m_items);
        void SortItemsBySize(std::vector<Item>& m_items);
        void SortItemsByRarity(std::vector<Item>& m_items);

        std::shared_ptr<ui::ButtonWidget> GetSortByNameButton() { return sortByNameButton; }
        std::shared_ptr<ui::ButtonWidget> GetSortByCostButton() { return sortByCostButton; }
        std::shared_ptr<ui::ButtonWidget> GetSortBySizeButton() { return sortBySizeButton; }
        std::shared_ptr<ui::ButtonWidget> GetSortByRarityButton() { return sortByRarityButton; }

        bool IsActive = false;

    private:
        std::shared_ptr<ui::ButtonWidget> sortByNameButton;
        std::shared_ptr<ui::ButtonWidget> sortByCostButton;
        std::shared_ptr<ui::ButtonWidget> sortBySizeButton;
        std::shared_ptr<ui::ButtonWidget> sortByRarityButton;

        std::shared_ptr<ui::InventoryContainer> m_inventoryContainer;
        std::shared_ptr<Inventory> m_inventory;
        std::shared_ptr<ui::Window> m_mainWindow;

        bool sortedByName = true;
        bool sortedByCost = false;
        bool sortedBySize = false;
        bool sortedByRarity = false;

    };
}
