#include "Inventory.h"

#include "ResourceManager.h"
#include "Json.h"

using nlohmann::json;

Inventory::Inventory()
	:
	HudWindow{ sf::Keyboard::Key::I },
	background{ TextureManager::Get("Inventory\\Items Inventory") }
{
	itemsInfo = read_json("Config\\Items.json");

	for (const InventoryItem::Equipment equipment : std::array<InventoryItem::Equipment, 3>
	{
		InventoryItem::Equipment::MagicStaff,
			InventoryItem::Equipment::StrongCape,
			InventoryItem::Equipment::HeavyArmor
	})
	{
		Equip(CreateAndStore(equipment));
	}
}

void Inventory::OnDraw(const Graphics& gfx)
{
	background.setPosition(gfx.MapPixelToCoords(sf::Vector2i{ 0, 0 }));
	gfx.Draw(background);

	{
		const sf::Vector2f equipSlotDimensions{ itemsInfo["equipSlotWidth"], itemsInfo["equipSlotHeight"] };

		for (const auto& pair : equippedItems)
		{
			const int positionIndex = static_cast<int>(pair.first);

			pair.second->Render
			(
				gfx,
				sf::FloatRect
				{
					gfx.MapPixelToCoords(sf::Vector2i{ itemsInfo["positions"][positionIndex]["x"], itemsInfo["positions"][positionIndex]["y"] }),
					equipSlotDimensions
				}
			);
		}
	}

	const int itemsPerRow = itemsInfo["itemsPerRow"];

	const sf::Vector2f regularSlotDimensions{ itemsInfo["regularSlotWidth"], itemsInfo["regularSlotHeight"] };

	const sf::Vector2i equipAreaOffset{ itemsInfo["equipAreaHorizontalStart"], itemsInfo["equipAreaVerticalStart"] };

	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < itemsPerRow; ++x)
		{
			const int index = x + y * itemsPerRow;

			if (index >= storedItems.size())
			{
				return;
			}

			storedItems[index]->Render
			(
				gfx,
				sf::FloatRect
				{
					gfx.MapPixelToCoords(sf::Vector2i{ static_cast<int>(equipAreaOffset.x + x * regularSlotDimensions.x), static_cast<int>(equipAreaOffset.y + y * regularSlotDimensions.y) }),
					regularSlotDimensions
				}
			);
		}
	}
}

void Inventory::OnMouseClicked(const sf::Vector2f position)
{
	for (const auto& pair : equippedItems)
	{
		if (pair.second->IsAt(position))
		{
			Dequip(pair.second->GetEquipmentType());
			return;
		}
	}

	for (auto iterator = storedItems.begin(); iterator != storedItems.end(); ++iterator)
	{
		if ((*iterator)->IsAt(position))
		{
			Equip(iterator - storedItems.begin());
			return;
		}
	}
}

void Inventory::Equip(const int itemIndex)
{
	std::unique_ptr<InventoryItem> item = std::move(storedItems[itemIndex]);

	storedItems.erase(storedItems.begin() + itemIndex);

	const InventoryItem::EquipmentType type = item->GetEquipmentType();

	{
		const auto iterator = equippedItems.find(type);

		if (iterator != equippedItems.end())
		{
			Dequip(iterator->second->GetEquipmentType());
		}
	}

	equippedItems.emplace(type, std::move(item));
}

void Inventory::Dequip(const InventoryItem::EquipmentType type)
{
	const auto iterator = equippedItems.find(type);

	storedItems.emplace_back(std::move(iterator->second));

	equippedItems.erase(iterator);
}

int Inventory::CreateAndStore(const InventoryItem::Equipment type)
{
	const int position = static_cast<int>(type);

	const int itemsPerRow = itemsInfo["itemsPerRow"];

	const int row = position / itemsPerRow;

	const int column = position % itemsPerRow;

	const int itemWidth = itemsInfo["itemWidth"];
	const int itemHeight = itemsInfo["itemHeight"];

	storedItems.emplace_back(std::make_unique<InventoryItem>(type, sf::IntRect{ column * itemWidth, row * itemHeight, itemWidth, itemHeight }));

	return storedItems.size() - 1;
}