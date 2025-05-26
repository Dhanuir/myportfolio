#include "pch.h"
#include "TradingWidget.h"
#include "../application/inventory.h"
#include "iostream"

namespace ui
{
    TradingWidget::TradingWidget(float posX, float posY, float width, float height, std::shared_ptr<ui::InventoryContainer> sellerInventoryContainer, std::shared_ptr<ui::InventoryContainer> buyerInventoryContainer, std::shared_ptr< Inventory > seller_inventory, std::shared_ptr< Inventory > buyer_inventory, std::shared_ptr<ui::Window> mainWindow)
        : ModalWidget(posX, posY, width, height),
        m_sellerInventoryContainer(sellerInventoryContainer),
        m_buyerInventoryContainer(buyerInventoryContainer),
        m_sellerInventory(seller_inventory),
        m_buyerInventory(buyer_inventory),
        m_mainWindow(mainWindow)
    {
        m_quantityToTrade = std::make_shared<int>(1);
        m_itemNameText = std::make_shared<TextWidget>("Test", posX + 20.0f, posY - 30.0f, 60, sf::Color::White);
        AddChildWidget(m_itemNameText);

        // Trade button
        m_tradeButton = std::make_shared<ui::ButtonWidget>(posX + 10.0f, posY + height - 50.0f, 280.0f, 40.0f, "Trade");
        m_tradeButton->SetOnClick([this]()
            {
                int sellersCurrency = m_sellerInventoryContainer->GetInventoryPtr()->GetCurrentMoney();
                int buyerCurrency = m_buyerInventoryContainer->GetInventoryPtr()->GetCurrentMoney();
                int itemCost = m_currentItem->m_cost;
                int totalPrice = *m_quantityToTrade * itemCost;

                bool buyerAlreadyHasItem = ContainsItem(m_itemNameText->GetText(), m_buyerInventoryContainer->GetInventoryPtr()->GetItems());

                // Calculate the total cost of the trade
                int totalCost = (*m_quantityToTrade * -1) * itemCost;

                std::cout << m_buyerInventoryContainer->GetInventorySlotAmount() << std::endl << m_buyerInventoryContainer->GetInventoryItemAmount() << std::endl;
                if (m_buyerInventoryContainer->GetInventorySlotAmount() > m_buyerInventoryContainer->GetInventoryItemAmount() || buyerAlreadyHasItem)
                {
                    if (buyerCurrency >= totalPrice)
                    {
                        m_sellerInventoryContainer->UpdateItemInJson(m_currentItem->m_name, (*m_quantityToTrade) * (-1), m_buyerInventoryContainer);

                        m_sellerInventoryContainer->ClearItems();
                        if (!m_sellerInventoryContainer->GetInventoryPtr()->GetItems().empty()) {
                            m_sellerInventoryContainer->AssignItems(m_sellerInventoryContainer->GetInventoryPtr()->GetItems());
                        }

                        m_buyerInventoryContainer->ClearItems();
                        if (!m_buyerInventoryContainer->GetInventoryPtr()->GetItems().empty()) {
                            m_buyerInventoryContainer->AssignItems(m_buyerInventoryContainer->GetInventoryPtr()->GetItems());
                        }

                        ResetQuantity();

                        // Update currency text widgets
                        m_sellerInventoryContainer->m_moneyTextWidget->SetText('$' + std::to_string(sellersCurrency + totalPrice));
                        m_buyerInventoryContainer->m_moneyTextWidget->SetText('$' + std::to_string(buyerCurrency + (totalPrice * (-1))));
                        std::cout << "Trade completed" << std::endl;
                    }
                    else
                    {
                        std::cout << "Not enough money" << std::endl;
                    }
                    
                }
                else
                {
                    std::cout << "Not enough inventory space" << std::endl;
                }
            });

        // Decrement button
        m_decrementButton = std::make_shared<ui::ButtonWidget>(posX + 10.0f, posY + 60.0f, 40.0f, 40.0f, "<");
        m_decrementButton->SetOnClick([this]()
            {
                if (*m_quantityToTrade > 1)
                {
                    (*m_quantityToTrade)--;
                    UpdateQuantityText();
                }
            });

        // Increment button
        m_incrementButton = std::make_shared<ui::ButtonWidget>(posX + 240.0f, posY + 60.0f, 40.0f, 40.0f, ">");
        m_incrementButton->SetOnClick([this]()
            {
                if (m_currentItem->m_count > *m_quantityToTrade)
                {
                    if (m_buyerInventory->GetCurrentMoney() >= m_currentItem->m_cost * (*m_quantityToTrade + 1))
                    {
                        (*m_quantityToTrade)++;
                        UpdateQuantityText();
                    }
                    else
                    {
                        std::cout << "Not enough money";
                    }
                }
            });

        // Quantity text
        m_quantityText = std::make_shared<ui::TextWidget>("1", posX + 80.0f, posY + 20.0f, 100, sf::Color::White);

        // Add the widgets
        AddChildWidget(m_tradeButton);
        AddChildWidget(m_decrementButton);
        AddChildWidget(m_incrementButton);
        AddChildWidget(m_quantityText);
    }

    void TradingWidget::UpdateQuantityText()
    {
        m_quantityText->SetText(std::to_string(*m_quantityToTrade));
    }

    void TradingWidget::ResetQuantity()
    {
        *m_quantityToTrade = 1;
        UpdateQuantityText();
    }


    void TradingWidget::SetItem(std::shared_ptr<Item> item)
    {
        m_currentItem = item;

        if (m_currentItem)
        {
            // Set the name of the item in trading widget
            m_itemNameText->SetText(m_currentItem->m_name);
            // Reset quantity each time a new item is selected
            ResetQuantity();
        }
    }

    bool TradingWidget::ContainsItem(const std::string& itemName, std::vector<Item> itemList) const {
        for (const auto& item : itemList) {
            if (item.m_name == itemName) {
                return true;
            }
        }
        return false;
    }

    bool TradingWidget::ContainsTextWidget(const std::string& widgetName, const std::shared_ptr<TextWidget>& itemNameText) const {
        return (itemNameText && itemNameText->GetText() == widgetName);
    }


}
