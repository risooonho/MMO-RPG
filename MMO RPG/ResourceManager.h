#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

#include <map>

#include "Texture.h"
#include "Font.h"

template<typename T, typename TResourceType>
class ResourceManager
{
public:
	static const T& Get(const std::string& name);

private:
	static std::map<std::string, T> LoadResources();

private:
	static std::map<std::string, T> resourceMap;
};

typedef ResourceManager<sf::Texture, ResourceManagement::Texture> TextureManager;
typedef ResourceManager<sf::Font, ResourceManagement::Font> FontManager;