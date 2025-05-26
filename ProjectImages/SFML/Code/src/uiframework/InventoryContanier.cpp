#include "pch.h"
#include "InventoryContainer.h"
#include "../uiframework/InventorySlot.h"
#include "../application/inventory.h"
#include <iostream>

namespace ui
{
    InventoryContainer::InventoryContainer(float posX, float posY, float width, float height, int slotAmount, int slotsPerCol, std::shared_ptr< Inventory > Inventory)
        : Widget(posX, posY, width, height), m_currentScrollOffset(0.0f), m_slotAmount(slotAmount), m_slotsPerCol(slotsPerCol), m_slotSize(140.0f), m_padding(5.0f), m_Inventory(Inventory)
    {
        // Container Outer shape
        m_containerShape_Outer.setPosition(static_cast<float>(posX), static_cast<float>(posY));
        m_containerShape_BorderThickness = static_cast<float>(width) * 0.06f;
        m_containerShape_Outer.setSize(sf::Vector2f(static_cast<float>(width)+m_containerShape_BorderThickness, static_cast<float>(height)+m_containerShape_BorderThickness));
        m_containerShape_Outer.setFillColor(sf::Color(45, 110, 155));
        // Container Inner shape
        m_containerShape_Inner.setPosition(static_cast<float>(posX)+m_containerShape_BorderThickness/2, static_cast<float>(posY)+m_containerShape_BorderThickness / 2);
        m_containerShape_Inner.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
        m_containerShape_Inner.setFillColor(sf::Color(25, 45, 80));

        // View for clipping container content
        m_view.setSize(m_containerShape_Inner.getSize().x, m_containerShape_Inner.getSize().y);

        float initialCenterY = m_containerShape_Inner.getPosition().y + m_containerShape_Inner.getSize().y / 2.0f;
        float initialCenterX = m_containerShape_Inner.getPosition().x + m_containerShape_BorderThickness/2.0f + m_containerShape_Inner.getSize().x / 2.0f;
        m_view.setCenter(initialCenterX, initialCenterY);

        // Normalize viewport
        float normalizedLeft = m_containerShape_Inner.getPosition().x / 1920.0f;
        float normalizedTop = m_containerShape_Inner.getPosition().y / 1080.0f;
        float normalizedWidth = static_cast<float>(width) / 1920.0f;
        float normalizedHeight = static_cast<float>(height) / 1080.0f;

        m_view.setViewport(sf::FloatRect(
            normalizedLeft,
            normalizedTop,
            normalizedWidth,
            normalizedHeight
        ));
        
        // Set up view for clipping
        CreateGrid(slotAmount, slotsPerCol, m_slotSize, m_padding);

    }

    void InventoryContainer::AddWidget(std::shared_ptr<Widget> widget)
    {
        m_widgets.push_back(widget);
    }

    InputEventState InventoryContainer::ProcessInput(const InputEvent& event)
    {
        for (auto& widget : m_widgets)
        {
            if (widget->ProcessInput(event) == InputEventState::Handled)
            {
                return InputEventState::Handled;
            }
        }
        return InputEventState::Unhandled;
    }

    void InventoryContainer::Draw(RenderContext& context) const
    {
        
        // Store the current view
        sf::View previousView = context.getView();

        // Draw the container
        context.draw(m_containerShape_Outer);
        context.draw(m_containerShape_Inner);

        // Set the view for clipping the inventory container content
        context.setView(m_view);

        // Draw child widgets
        for (const auto& widget : m_widgets)
        {
            widget->Draw(context);
        }
        // Restore view
        context.setView(previousView);

        // Draw tooltip if inventory slot is hovered and contains item
        for (const auto& slot : m_slots)
        {
            if (slot->IsHovered() && slot->HasItem())
            {
                Item slotItem = slot->GetItem();
                std::shared_ptr<ui::TooltipWidget> slotItemTooltip = slot->GetTooltip();
                
                slotItemTooltip->AdjustPosition(context);
                slotItemTooltip->SetItemDetails(slotItem.m_name, slotItem.m_cost, slotItem.m_size, slotItem.m_rarity, slotItem.m_attributes);
                slot->DrawTooltip(context);
            }
        }
    }

    void InventoryContainer::CreateGrid(int slotAmount, int slotsPerCol, float slotSize, float padding)
    {
        m_widgets.clear();
        m_slots.clear();

        // Calculate rows
        m_rows = (slotAmount + slotsPerCol - 1) / slotsPerCol;

        float containerWidth = static_cast<float>(m_containerShape_Inner.getSize().x);
        float totalSlotWidth = static_cast<float>((slotsPerCol * (slotSize + padding)) - padding);
        float paddingToCenterGrid_x = (m_containerShape_Inner.getSize().x - totalSlotWidth) / 2.0f;
        float startX = m_containerShape_Inner.getPosition().x + paddingToCenterGrid_x + m_containerShape_BorderThickness / 2;
        float startY = m_containerShape_Inner.getPosition().y + m_containerShape_BorderThickness / 2;

        // Grid layout
        for (int index = 0; index < slotAmount; ++index)
        {
            int row = index / slotsPerCol;
            int col = index % slotsPerCol;

            float x = startX + static_cast<float>(col * (slotSize + padding));
            float y = startY + static_cast<float>(row * (slotSize + padding));

            auto slot = std::make_shared<InventorySlot>(
                x, y, slotSize);

            m_widgets.push_back(slot);
            m_slots.push_back(slot);
        }
    }

    std::vector<std::shared_ptr<InventorySlot>>& InventoryContainer::GetSlots()
    {
        return m_slots;
    }

    void InventoryContainer::Scroll(float deltaY)
    {
        // Scroll offset
        float newScrollOffset = m_currentScrollOffset + deltaY;

        // Maximum possible scroll offset
        float totalContentHeight = m_rows * (m_slotSize + m_padding) - m_padding;
        float visibleHeight = m_containerShape_Inner.getSize().y;
        float maxScrollOffset = std::max(0.0f, totalContentHeight - visibleHeight);

        // Clamp the scroll
        if (newScrollOffset < 0.0f)
        {
            newScrollOffset = 0.0f;
        }
        else if (newScrollOffset > maxScrollOffset)
        {
            newScrollOffset = maxScrollOffset;
        }

        if (newScrollOffset != m_currentScrollOffset)
        {
            m_currentScrollOffset = newScrollOffset;

            float newCenterY = m_containerShape_Inner.getPosition().y + m_currentScrollOffset + visibleHeight / 2.0f;
            m_view.setCenter(m_view.getCenter().x, newCenterY);
        }
    }

    BoxConstraints InventoryContainer::GetConstraints() const
    {
        return {
            0.0f,
            m_containerShape_Inner.getSize().x,
            0.0f,
            m_containerShape_Inner.getSize().y
        };
    }

    void InventoryContainer::AssignItems(const std::vector<Item>& items)
    {
        if (!items.empty())
        {
            for (size_t i = 0; i < items.size() && i < m_slots.size(); ++i)
            {
                m_slots[i]->SetItem(items[i]);
                m_itemAmount++;
                m_slots[i]->RemoveTransparency();
            }
        }
        else 
        {
            std::cout << "Items slot is empty. No items to assign." << std::endl;
        }
    }

    void InventoryContainer::ClearItems() {
        // Clear the vector of widgets and slots
        m_slots.clear();
        m_widgets.clear();

        // Recreate the grid layout for empty slots
        CreateGrid(m_slotAmount, m_slotsPerCol, m_slotSize, m_padding);
        m_itemAmount = 0;
    }

    void InventoryContainer::SetInventoryPath(std::string path)
    {
        std::ostringstream pathBuilderSeller;
        pathBuilderSeller << "../../data/" << path;
        m_path = pathBuilderSeller.str();
    }

    std::string InventoryContainer::GetInventoryPath()
    {
        return m_path;
    }


    void InventoryContainer::UpdateItemInJson(const std::string& itemName, int countChange, std::shared_ptr<InventoryContainer> buyerInventoryContainer) {
        // Update seller's inventory
        bool itemFound = false;

        auto& sellerItems = m_Inventory->GetItems();
        int sellerCurrency = m_Inventory->GetCurrentMoney();

        int itemCost = 0;

        for (auto& item : sellerItems) {
            if (item.m_name == itemName) {
                itemCost = item.m_cost;
                item.m_count += countChange;

                // If count becomes zero or less, remove the item from the seller's inventory
                if (item.m_count <= 0) {
                    sellerItems.erase(std::remove_if(sellerItems.begin(), sellerItems.end(),
                        [](const Item& item) { return item.m_count <= 0; }), sellerItems.end());
                }
                itemFound = true;
                break;
            }
        }

        if (!itemFound) {
            std::cerr << "Error: Item " << itemName << " not found in seller's inventory" << std::endl;
            return; // Exit the function if the seller does not have the item
        }

        // Calculate the total cost of the trade
        int totalCost = (countChange * -1) * itemCost;

        // Update buyer's inventory if the buyer can afford it
        auto& buyerItems = buyerInventoryContainer->GetInventoryPtr()->GetItems();
        int buyerCurrency = buyerInventoryContainer->GetInventoryPtr()->GetCurrentMoney();

        if (buyerCurrency < totalCost) {
            std::cerr << "Error: Buyer does not have enough currency to complete the purchase." << std::endl;
            return;
        }

        // Deduct buyer's currency and add to seller's currency
        buyerCurrency -= totalCost;
        sellerCurrency += totalCost;

        // Set the updated currencies back to their respective inventories
        buyerInventoryContainer->GetInventoryPtr()->SetCurrentMoney(buyerCurrency);
        m_Inventory->SetCurrentMoney(sellerCurrency);

        // Proceed with updating buyer's inventory
        int absoluteCountChange = -countChange;
        bool buyerItemFound = false;

        // Search for the item in the buyer's inventory and update it
        for (auto& item : buyerItems) {
            if (item.m_name == itemName) {
                item.m_count += absoluteCountChange;
                buyerItemFound = true;
                break;
            }
        }

        // If item not found in buyer's inventory, add it
        if (!buyerItemFound) {
            // Find the item in the seller's inventory to copy the details (note: must use item information before it is removed)
            for (const auto& item : sellerItems) {
                if (item.m_name == itemName) {
                    Item newItem = item; // Copy item properties
                    newItem.m_count = absoluteCountChange; // Set count for buyer's inventory
                    buyerItems.push_back(newItem);
                    break;
                }
            }
        }

        // Save the buyer's updated inventory
        buyerInventoryContainer->GetInventoryPtr()->Save(buyerInventoryContainer->GetInventoryPath());

        // Save the seller's updated inventory
        m_Inventory->Save(m_path);
    }
}

