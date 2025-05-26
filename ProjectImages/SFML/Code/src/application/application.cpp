#include "pch.h"
#include "../framework/window.h"
#include "../uiframework/BackgroundChanger.h"
#include "../uiframework/pointerwidget.h"
#include "../uiframework/InventoryContainer.h"
#include "../uiframework/TextWidget.h"
#include "../uiframework/ButtonWidget.h"
#include "../uiframework/ModalWidget.h"
#include "../uiframework/SortingWidget.h"
#include "../uiframework/TradingWidget.h"
#include "inventory.h"
#include "application.h"
#include "iostream"

Application::Application() = default;
Application::~Application() = default;

void Application::Initialize()
{
	m_renderContext = std::make_unique<RenderContext>( sf::VideoMode( 1920, 1080 ), "Trading Window" );
	m_renderContext->setFramerateLimit(60);

	SetDataPath( "../../data/" );

	LoadInventories();

    if (m_mainWindow = std::make_shared<ui::Window>())
    {
        // Replace system cursor
        m_renderContext.get()->setMouseCursorVisible(false);
        sf::Cursor cursor;
        if (cursor.loadFromSystem(sf::Cursor::Hand))
        {
            m_renderContext.get()->setMouseCursor(cursor);
        }

        m_backgroundChanger = std::make_unique<BackgroundChanger>();

        // Add Player Inventory
        m_playerInventoryContainer = std::make_shared<ui::InventoryContainer>(20.0f, 100.0f, 800.0f, 600.0f, m_playerInventory->GetMaxSpace(), 5, m_playerInventory); // posX, posY, width, height, slotAmount, slotsPerCol
        m_playerInventoryContainer->SetInventoryPath("player_inventory.json");
        auto playerTitle = std::make_shared<ui::TextWidget>(m_playerInventory->GetOwnerName(), m_playerInventoryContainer->GetPosX(), m_playerInventoryContainer->GetPosY() - 50, 30, sf::Color(207, 169, 136));
        m_playerInventoryContainer->m_moneyTextWidget = std::make_shared<ui::TextWidget>('$' + std::to_string(m_playerInventory->GetCurrentMoney()), m_playerInventoryContainer->GetPosX(), m_playerInventoryContainer->GetPosY() + m_playerInventoryContainer->GetHeight() + 50, 30, sf::Color(207, 169, 136));

        m_mainWindow->AddWidget(playerTitle);
        m_mainWindow->AddWidget(m_playerInventoryContainer);
        m_mainWindow->AddWidget(m_playerInventoryContainer->m_moneyTextWidget);

        // Create and add Vendor Inventory Container
        m_vendorInventoryContainer = std::make_shared<ui::InventoryContainer>(1050.0f, 100.0f, 800.0f, 600.0f, m_vendorInventory->GetMaxSpace(), 5, m_vendorInventory); // posX, posY, width, height, slotAmount, slotsPerCol
        m_vendorInventoryContainer->SetInventoryPath("vendor_inventory.json");
        auto vendorTitle = std::make_shared<ui::TextWidget>(m_vendorInventory->GetOwnerName(), m_vendorInventoryContainer->GetPosX(), m_vendorInventoryContainer->GetPosY() - 50, 30, sf::Color(207, 169, 136));
        m_vendorInventoryContainer->m_moneyTextWidget = std::make_shared<ui::TextWidget>('$' + std::to_string(m_vendorInventory->GetCurrentMoney()), m_vendorInventoryContainer->GetPosX(), m_vendorInventoryContainer->GetPosY() + m_vendorInventoryContainer->GetHeight() + 50, 30, sf::Color(207, 169, 136));

        // Add Vendor title and container
        m_mainWindow->AddWidget(vendorTitle);
        m_mainWindow->AddWidget(m_vendorInventoryContainer);
        m_mainWindow->AddWidget(m_vendorInventoryContainer->m_moneyTextWidget);

        m_pointerWidget = std::make_unique<ui::PointerWidget>(50.0f, 50.0f, 20.0f, 20.0f, m_renderContext.get());

        // Assign items to player inventory slots
        if (m_playerInventory && m_playerInventoryContainer)
        {
            m_playerInventoryContainer->AssignItems(m_playerInventory->GetItems());
        }

        // Create player trading widget
        playerTradingWidget = std::make_shared<ui::TradingWidget>(
            m_renderContext->getSize().x / 2 - 150.0f, m_playerInventoryContainer->GetPosY() + (m_playerInventoryContainer->GetHeight() / 2) - 110.0f, 300.0f, 220.0f,
            m_playerInventoryContainer,
            m_vendorInventoryContainer,
            m_playerInventory,
            m_vendorInventory,
            m_mainWindow
        );
        
        playerTradingWidget->m_tradeButton->SetOnClick([this]()
            {
                InitiateTrade(playerTradingWidget);
                m_mainWindow->RemoveWidget(playerTradingWidget);
            });

        // Create Vendor trading widget
        vendorTradingWidget = std::make_shared<ui::TradingWidget>(
            static_cast<float>(m_renderContext->getSize().x) / 2.0f - 150.0f, m_vendorInventoryContainer->GetPosY() + (m_vendorInventoryContainer->GetHeight() / 2.0f) - 110.0f, 300.0f, 220.0f,
            m_vendorInventoryContainer,
            m_playerInventoryContainer,
            m_vendorInventory,
            m_playerInventory,
            m_mainWindow
        );

        vendorTradingWidget->m_tradeButton->SetOnClick([this]()
            {
                InitiateTrade(vendorTradingWidget);
                m_mainWindow->RemoveWidget(vendorTradingWidget);
            });

        // Create player inventory sorting widget
        playerSortingWidget = std::make_shared<ui::SortingWidget>(
            m_playerInventoryContainer->GetPosX() + m_playerInventoryContainer->GetWidth() -250.0f, 90.0f, 300.0f, 220.0f,
            m_playerInventoryContainer,
            m_playerInventory,
            m_mainWindow
        );
        // Assign items to player inventory slots
        if (m_vendorInventory && m_vendorInventoryContainer)
        {
            m_vendorInventoryContainer->AssignItems(m_vendorInventory->GetItems());
        }

        playerSortButton = std::make_shared<ui::ButtonWidget>(m_playerInventoryContainer->GetPosX() + m_playerInventoryContainer->GetWidth()-50.0f, 50.0f, 100.0f, 40.0f, "Sort");
        m_mainWindow->AddWidget(playerSortButton);

        playerSortButton->SetOnClick([this]()
            {
                if (playerSortingWidget && playerSortingWidget->IsActive==false)
                {
                    m_mainWindow->AddWidget(playerSortingWidget);
                    playerSortingWidget->IsActive = true;
                }
                else if (playerSortingWidget && playerSortingWidget->IsActive ==true)
                {
                    m_mainWindow->RemoveWidget(playerSortingWidget);
                    playerSortingWidget->IsActive = false;
                }
            });
        
        // Create vendor inventory sorting widget
        vendorSortingWidget = std::make_shared<ui::SortingWidget>(
            m_vendorInventoryContainer->GetPosX() + m_vendorInventoryContainer->GetWidth() - 250.0f, 90.0f, 300.0f, 220.0f,
            m_vendorInventoryContainer,
            std::move(m_vendorInventory),
            m_mainWindow
        );
        // Assign items to vendor inventory slots
        if (m_vendorInventory && m_vendorInventoryContainer)
        {
            m_vendorInventoryContainer->AssignItems(m_vendorInventory->GetItems());
        }

        vendorSortButton = std::make_shared<ui::ButtonWidget>(m_vendorInventoryContainer->GetPosX() + m_vendorInventoryContainer->GetWidth() - 50.0f, 50.0f, 100.0f, 40.0f, "Sort");
        m_mainWindow->AddWidget(vendorSortButton);

        vendorSortButton->SetOnClick([this]()
            {
                if (vendorSortingWidget && vendorSortingWidget->IsActive == false)
                {
                    m_mainWindow->AddWidget(vendorSortingWidget);
                    vendorSortingWidget->IsActive = true;
                }
                else if (vendorSortingWidget && vendorSortingWidget->IsActive == true)
                {
                    m_mainWindow->RemoveWidget(vendorSortingWidget);
                    vendorSortingWidget->IsActive = false;
                }
            });
	}
}

void Application::Run()
{
    while (m_renderContext->isOpen())
    {
        InputEvent event;
        while (m_renderContext->pollEvent(event))
        {
            if (event.type == InputEvent::Closed)
            {
                m_renderContext->close();
            }
            else if (event.type == InputEvent::MouseWheelScrolled)
            {
                float scrollDelta = event.mouseWheelScroll.delta * 20.0f; // Scroll speed

                sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_renderContext);

                if (m_playerInventoryContainer->IsPointerOver(mousePosition))
                {
                    m_playerInventoryContainer->Scroll(-scrollDelta); // Scroll player inventory
                }
                else if (m_vendorInventoryContainer->IsPointerOver(mousePosition))
                {
                    m_vendorInventoryContainer->Scroll(-scrollDelta); // Scroll vendor inventory
                }
            }
            else if (event.type == InputEvent::MouseMoved)
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_renderContext);

                // Check if the mouse is over any inventory slot in the player inventory
                for (auto& slot : m_playerInventoryContainer->GetSlots())
                {
                    slot->IsPointerOver(mousePosition);
                }

                // Check if the mouse is over any inventory slot in the vendor inventory
                for (auto& slot : m_vendorInventoryContainer->GetSlots())
                {
                    slot->IsPointerOver(mousePosition);
                }
                if (playerSortButton != nullptr)
                {
                    bool playerSortButtonHovered = playerSortButton->IsPointerOver(mousePosition);
                    playerSortButton->HighlightButton(playerSortButtonHovered);
                }
                else
                {
                std::cout << "playerSortButton not loaded";
                }
                if (vendorSortButton != nullptr)
                {
                    bool playerSortButtonHovered = vendorSortButton->IsPointerOver(mousePosition);
                    vendorSortButton->HighlightButton(playerSortButtonHovered);
                }
                else
                {
                    std::cout << "vendorSortButton not loaded";
                }
                // Highlight sorting drop down buttons
                if (playerSortingWidget)
                {
                    if (playerSortingWidget->IsPointerOver(mousePosition))
                    {
                        playerSortingWidget->GetSortByNameButton()->HighlightButton(playerSortingWidget->GetSortByNameButton()->IsPointerOver(mousePosition));
                        playerSortingWidget->GetSortByCostButton()->HighlightButton(playerSortingWidget->GetSortByCostButton()->IsPointerOver(mousePosition));
                        playerSortingWidget->GetSortBySizeButton()->HighlightButton(playerSortingWidget->GetSortBySizeButton()->IsPointerOver(mousePosition));
                        playerSortingWidget->GetSortByRarityButton()->HighlightButton(playerSortingWidget->GetSortByRarityButton()->IsPointerOver(mousePosition));
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_renderContext);

                // Close sorting widget when clicking outside of it
                if (playerSortingWidget)
                {
                    if (playerSortButton->IsPointerOver(mousePosition) && playerSortingWidget->IsActive)
                    {
                        m_mainWindow->RemoveWidget(playerSortingWidget);
                    }
                    else if (!playerSortingWidget->IsPointerOver(mousePosition))
                    {
                        m_mainWindow->RemoveWidget(playerSortingWidget);
                        playerSortingWidget->IsActive = false;
                    }
                }
                if (vendorSortingWidget)
                {
                    if (vendorSortButton->IsPointerOver(mousePosition) && vendorSortingWidget->IsActive)
                    {
                        m_mainWindow->RemoveWidget(vendorSortingWidget);
                    }
                    else if (!vendorSortingWidget->IsPointerOver(mousePosition))
                    {
                        m_mainWindow->RemoveWidget(vendorSortingWidget);
                        vendorSortingWidget->IsActive = false;
                    }
                }
                // Close trading widget when clicking outside of it
                if (playerTradingWidget)
                {
                    if (!playerTradingWidget->IsPointerOver(mousePosition))
                    {
                        m_mainWindow->RemoveWidget(playerTradingWidget);
                        playerTradingWidget->ResetQuantity();
                    }
                }
                if (vendorTradingWidget)
                {
                    if (!vendorTradingWidget->IsPointerOver(mousePosition))
                    {
                        m_mainWindow->RemoveWidget(vendorTradingWidget);
                        vendorTradingWidget->ResetQuantity();
                    }
                }

                // Player inventory click and drag
                if (m_playerInventoryContainer->IsPointerOver(mousePosition))
                {
                    for (std::shared_ptr<ui::InventorySlot> slot : m_playerInventoryContainer->GetSlots())
                    {
                        if (slot)
                        {
                            if (slot->IsPointerOver(mousePosition))
                            {
                                // Create a dragging copy of the item
                                m_draggedItemSprite = std::make_shared<sf::Sprite>(slot->GetItemSprite());
                                m_draggedItem = std::make_shared<Item>(slot->GetItem());
                                if (m_draggedItemSprite->getTexture())
                                { 
                                    m_isDraggingItem = true;
                                    m_isDraggingPlayerItem = true;
                                }
                                break;
                            }
                        }
                        else
                        {
                            std::cout << "Widget is not a player InventorySlot.\n";
                        }
                    }
                }

                // Vendor inventory click and drag
                else if (m_vendorInventoryContainer->IsPointerOver(mousePosition))
                {
                    for (std::shared_ptr<ui::InventorySlot> slot : m_vendorInventoryContainer->GetSlots())
                    {
                        if (slot)
                        {
                            if (slot->IsPointerOver(mousePosition))
                            {
                                // Create a dragging copy of the item
                                m_draggedItemSprite = std::make_shared<sf::Sprite>(slot->GetItemSprite());
                                m_draggedItem = std::make_shared<Item>(slot->GetItem());
                                if (m_draggedItemSprite->getTexture()) 
                                { 
                                    m_isDraggingItem = true;
                                    m_isDraggingVendorItem = true;
                                }
                                break;
                            }

                        }
                        else
                        {
                            std::cout << "Widget is not a vendor InventorySlot.\n";
                        }
                    }
                }
            }
            // Release item on top of buyers container
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
                {
                    // Release on top of Vendor Container
                    if (m_isDraggingItem && m_isDraggingPlayerItem)
                    {
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_renderContext);
                        if (m_vendorInventoryContainer->IsPointerOver(mousePosition))
                        {
                            m_mainWindow->AddWidget(playerTradingWidget);
                            playerTradingWidget->SetItem(m_draggedItem);
                            
                            if (m_vendorInventoryContainer->GetInventoryPtr()->GetCurrentMoney() >= m_draggedItem->m_cost)
                            {
                                m_mainWindow->AddWidget(playerTradingWidget);
                                playerTradingWidget->SetItem(m_draggedItem);
                            }
                            else
                            {
                                std::cout << "Vendor do not have enough money";
                            }
                        }

                        m_isDraggingItem = false;
                        m_isDraggingPlayerItem = false;

                        // Remove the dragged item
                        if (m_draggedItemSprite)
                        {
                            m_draggedItemSprite = nullptr;
                        }
                    }
                    // Release on top of Player Container
                    else if (m_isDraggingItem && m_isDraggingVendorItem)
                    {
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_renderContext);
                        if (m_playerInventoryContainer->IsPointerOver(mousePosition))
                        {
                            if (m_playerInventoryContainer->GetInventoryPtr()->GetCurrentMoney() >= m_draggedItem->m_cost)
                            {
                                m_mainWindow->AddWidget(vendorTradingWidget);
                                vendorTradingWidget->SetItem(m_draggedItem);
                            }
                            else
                            {
                                std::cout << "Player do not have enough money";
                            }
                        }

                        m_isDraggingItem = false;
                        m_isDraggingPlayerItem = false;

                        // Remove the dragged item
                        if (m_draggedItemSprite)
                        {
                            m_draggedItemSprite = nullptr;
                        }
                    }
                }


            m_mainWindow->ProcessInput(event);

            if (m_pointerWidget)
            {
                m_pointerWidget->ProcessInput(event);

            }
        }

        // Update item being dragged with the mouse position
        if (m_isDraggingItem && m_draggedItemSprite)
        {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_renderContext);
            m_draggedItemSprite->setPosition(static_cast<float>(mousePosition.x - 60), static_cast<float>(mousePosition.y - 60));
        }

        if (m_backgroundChanger)
        {
            m_backgroundChanger->ChangeBackgroundColor(*m_renderContext);
        }
        else
        {
            m_renderContext->clear();
        }

        // Add UI elements
        m_mainWindow->Draw(*m_renderContext);

        // Dragged items drawn towards the end to ensure they render on top
        if (m_isDraggingItem && m_draggedItemSprite)
        {
            m_renderContext->draw(*m_draggedItemSprite);
        }

        // Draw the pointer widget
        if (m_pointerWidget)
        {
            m_pointerWidget->Draw();
        }


        m_renderContext->display();
    }
}

ui::Window* Application::GetMainWindow() const
{
	return m_mainWindow.get();
}

const std::string& Application::GetDataPath() const
{
	return m_dataPath;
}

void Application::SetDataPath( const char* dataPath )
{
	m_dataPath = dataPath;
}

void Application::LoadInventories()
{
	if( m_playerInventory = std::make_shared<Inventory>() )
	{
		std::ostringstream pathBuilder;
		pathBuilder << GetDataPath() << "player_inventory.json";
		m_playerInventory->Load( pathBuilder.str() );
        m_playerInventory->SetPath(pathBuilder.str());
	}

    if (m_vendorInventory = std::make_shared<Inventory>())
    {
        std::ostringstream pathBuilder;
        pathBuilder << GetDataPath() << "vendor_inventory.json";
        m_vendorInventory->Load(pathBuilder.str());
        m_vendorInventory->SetPath(pathBuilder.str());
    }
}

void Application::InitiateTrade(std::shared_ptr<ui::TradingWidget> TradingWidget)
{
    std::shared_ptr<ui::InventoryContainer> SellerInventoryContainer = TradingWidget->GetSellerInventoryContainer();
    std::shared_ptr<ui::InventoryContainer> BuyerInventoryContainer = TradingWidget->GetBuyerInventoryContainer();
    int sellersCurrency = SellerInventoryContainer->GetInventoryPtr()->GetCurrentMoney();
    int buyerCurrency = BuyerInventoryContainer->GetInventoryPtr()->GetCurrentMoney();
    int itemCost = TradingWidget->GetCurrentItem()->m_cost;
    int totalPrice = *TradingWidget->GetQuantityToTrade() * itemCost;

    bool buyerAlreadyHasItem = TradingWidget->ContainsItem(TradingWidget->GetItemNameText()->GetText(), BuyerInventoryContainer->GetInventoryPtr()->GetItems());

    // Calculate the total cost of the trade
    int totalCost = (*TradingWidget->GetQuantityToTrade() * -1) * itemCost;

    if (BuyerInventoryContainer->GetInventorySlotAmount() > BuyerInventoryContainer->GetInventoryItemAmount() || buyerAlreadyHasItem)
    {
        if (buyerCurrency >= totalPrice)
        {
            SellerInventoryContainer->UpdateItemInJson(TradingWidget->GetCurrentItem()->m_name, (*TradingWidget->GetQuantityToTrade()) * (-1), BuyerInventoryContainer);

            SellerInventoryContainer->ClearItems();
            if (!SellerInventoryContainer->GetInventoryPtr()->GetItems().empty()) {
                SellerInventoryContainer->AssignItems(SellerInventoryContainer->GetInventoryPtr()->GetItems());
            }

            BuyerInventoryContainer->ClearItems();
            if (!BuyerInventoryContainer->GetInventoryPtr()->GetItems().empty()) {
                BuyerInventoryContainer->AssignItems(BuyerInventoryContainer->GetInventoryPtr()->GetItems());
            }

            TradingWidget->ResetQuantity();

            // Update currency text widgets
            SellerInventoryContainer->m_moneyTextWidget->SetText('$' + std::to_string(sellersCurrency + totalPrice));
            BuyerInventoryContainer->m_moneyTextWidget->SetText('$' + std::to_string(buyerCurrency + (totalPrice * (-1))));
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
}
