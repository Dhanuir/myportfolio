#include "pch.h"
#include "SortingWidget.h"
#include "iostream"

namespace ui
{
    SortingWidget::SortingWidget(float posX, float posY, float width, float height, std::shared_ptr<ui::InventoryContainer> inventoryContainer, std::shared_ptr<Inventory> inventory, std::shared_ptr<ui::Window> mainWindow)
        : ModalWidget(posX, posY, width, height),
        m_inventoryContainer(inventoryContainer),
        m_inventory(std::move(inventory)),
        m_mainWindow(mainWindow)
    {
        sortByNameButton = std::make_shared<ui::ButtonWidget>(posX + 10.0f, posY + 10.0f, 280.0f, 40.0f, "Sort by Name");
        sortByCostButton = std::make_shared<ui::ButtonWidget>(posX + 10.0f, posY + 60.0f, 280.0f, 40.0f, "Sort by Cost");
        sortBySizeButton = std::make_shared<ui::ButtonWidget>(posX + 10.0f, posY + 110.0f, 280.0f, 40.0f, "Sort by Size");
        sortByRarityButton = std::make_shared<ui::ButtonWidget>(posX + 10.0f, posY + 160.0f, 280.0f, 40.0f, "Sort by Rarity");
        
        sortByNameButton->SetOnClick([this]()
            {
                SortingWidget::SortItemsByName(m_inventory->GetItems());
                m_inventoryContainer->AssignItems(m_inventory->GetItems());
                m_mainWindow->RemoveWidget(shared_from_this());  // Hide modal after sorting
                IsActive = false;
            });

        sortByCostButton->SetOnClick([this]()
            {
                SortingWidget::SortItemsByCost(m_inventory->GetItems());
                m_inventoryContainer->AssignItems(m_inventory->GetItems());
                m_mainWindow->RemoveWidget(shared_from_this());
                IsActive = false;
            });

        sortBySizeButton->SetOnClick([this]()
            {
                SortingWidget::SortItemsBySize(m_inventory->GetItems());
                m_inventoryContainer->AssignItems(m_inventory->GetItems());
                m_mainWindow->RemoveWidget(shared_from_this());
                IsActive = false;
            });

        sortByRarityButton->SetOnClick([this]()
            {
                SortingWidget::SortItemsByRarity(m_inventory->GetItems());
                m_inventoryContainer->AssignItems(m_inventory->GetItems());
                m_mainWindow->RemoveWidget(shared_from_this());
                IsActive = false;
            });

        // Add sorting buttons
        AddChildWidget(sortByNameButton);
        AddChildWidget(sortByCostButton);
        AddChildWidget(sortBySizeButton);
        AddChildWidget(sortByRarityButton);
    }

    void SortingWidget::SortItemsByName(std::vector<Item>& m_items)
    {
        if (!sortedByName)
        {
            std::sort(m_items.begin(), m_items.end(), [](const Item& a, const Item& b) {
                return a.m_name < b.m_name;
                });
            sortedByName = true;
        }
        else
        {
            std::sort(m_items.begin(), m_items.end(), [](const Item& a, const Item& b) {
                return a.m_name > b.m_name;
                });
            sortedByName = false;
            sortedByCost = false;
            sortedBySize = false;
            sortedByRarity = false;
        }
    }

    void SortingWidget::SortItemsByCost(std::vector<Item>& m_items)
    {
        if (!sortedByCost)
        {
            std::sort(m_items.begin(), m_items.end(), [](const Item& a, const Item& b) {
                return a.m_cost < b.m_cost;
                });
            sortedByCost = true;
        }
        else
        {
            std::sort(m_items.begin(), m_items.end(), [](const Item& a, const Item& b) {
                return a.m_cost > b.m_cost;
                });
            sortedByName = false;
            sortedByCost = false;
            sortedBySize = false;
            sortedByRarity = false;
        }
    }

    void SortingWidget::SortItemsBySize(std::vector<Item>& m_items)
    {
        if (!sortedBySize)
        {
            std::sort(m_items.begin(), m_items.end(), [](const Item& a, const Item& b) {
                return a.m_size < b.m_size;
                });
            sortedBySize = true;
        }
        else
        {
            std::sort(m_items.begin(), m_items.end(), [](const Item& a, const Item& b) {
                return a.m_size > b.m_size;
                });
            sortedByName = false;
            sortedByCost = false;
            sortedBySize = false;
            sortedByRarity = false;
        }
    }

    void SortingWidget::SortItemsByRarity(std::vector<Item>& m_items)
    {
        if (!sortedByRarity)
        {
            std::sort(m_items.begin(), m_items.end(), [](const Item& a, const Item& b) {
                return a.m_rarity > b.m_rarity;
                });
            sortedByRarity = true;
        }
        else
        {
            std::sort(m_items.begin(), m_items.end(), [](const Item& a, const Item& b) {
                return a.m_rarity < b.m_rarity;
                });
            sortedByName = false;
            sortedByCost = false;
            sortedBySize = false;
            sortedByRarity = false;
        }
    }
}
