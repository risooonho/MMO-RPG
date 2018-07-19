#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include <json.hpp>

#include <unordered_map>

#include "Graphics.h"
#include "InventoryItem.h"
#include "HudWindow.h"
#include <optional>
#include "Player.h"
#include "TimeoutTracker.h"

class Inventory : public HudWindow
{
public:
	explicit Inventory(Player& player);

public:
	void OnDraw(const Graphics& gfx) override;

	void OnMouseClicked(const sf::Vector2f position) override;

private:
	class InventorySlot
	{
	public:
		InventorySlot() = default;
		explicit InventorySlot(const sf::FloatRect dimensions);
		InventorySlot(InventoryItem item, const sf::FloatRect dimensions);

	public:
		virtual ~InventorySlot() = default;

	public:
		static void Swap(InventorySlot& first, InventorySlot& second);

	public:
		void Equip(const InventoryItem& item);
		std::optional<InventoryItem> Dequip();

		void UpdateWorldPosition(const sf::Vector2f worldPosition);

		virtual void Draw(const Graphics& gfx);

		bool IsAt(const sf::Vector2f point) const;
		bool HasItem() const;

		InventoryItem::EquipmentType GetEquipmentType() const;

		bool RequiresSwap(sf::Vector2f& position);

	protected:
		sf::Vector2f GetWorldPosition() const;

	protected:
		sf::FloatRect GetWorldDimensions() const;

	private:
		std::optional<InventoryItem> item;
		sf::FloatRect dimensions;
		sf::Vector2f worldPosition;
	};

public:
	class InventorySlotWithPlaceholder : public InventorySlot
	{
	public:
		InventorySlotWithPlaceholder() = default;
		InventorySlotWithPlaceholder(const sf::FloatRect dimensions, const sf::IntRect viewport);
		InventorySlotWithPlaceholder(const InventoryItem& item, const sf::FloatRect dimensions, const sf::IntRect viewport);

	public:
		void Draw(const Graphics& gfx) override;

	private:
		void Setup();

	private:
		std::shared_ptr<sf::Shader> grayscale;

		Sprite placeholderSprite;
	};

private:
	void Equip(const int itemIndex);
	void Dequip(const InventoryItem::EquipmentType type);

	int CreateAndStore(const InventoryItem::Equipment type);

	InventorySlot& FindEmptySlot();
	int FindEmptySlotIndex() const;

	sf::IntRect ComputeTextureRectangle(const int equipmentPosition) const;

	InventorySlot* FindSlot(const sf::Vector2f position);

private:
	Sprite background;

	nlohmann::json itemsInfo;

	std::unordered_map<InventoryItem::EquipmentType, InventorySlotWithPlaceholder> equipSlots;

	std::vector<InventorySlot> inventorySlots;

	Player& player;
};