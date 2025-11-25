#pragma once
#include <vector>
#include <algorithm>
#include <ranges>

namespace SCION_UTL {

	enum class AssetType
	{
		TEXTURE = 0,
		FONT,
		SOUNDFX,
		MUSIC,
		SCENE,
		NO_TYPE
	};

	template<typename Map>
	std::vector<typename Map::key_type> GetKeys(const Map& map)
	{
		auto keyView = std::views::keys(map);
		std::vector<typename Map::key_type> keys{ keyView.begin(), keyView.end() };
		return keys;
	}

	template<typename Map, typename Func>
	std::vector<typename Map::key_type> GetKeys(const Map& map, Func func)
	{
		auto keyView = map | std::views::filter(func) | std::views::keys;
		std::vector<typename Map::key_type> keys{ keyView.begin(), keyView.end() };
		return keys;
	}

	template<typename Map>
	bool KeyChanged(Map& map, const typename Map::key_type& key, const typename Map::key_type& change)
	{
		if (!map.contains(key) || map.contains(change))
		{
			return false;
		}

		auto node = map.at(key);
		map.erase(key);
		map[change] = node;
		return true;
	}

	std::string GetSubstring(std::string_view str, std::string_view find)
	{
		if (find.empty())
			return {};

		if (find.size() > 1)
		{
			const auto found = str.find(find);
			if (found == std::string_view::npos)
				return {};

			return std::string{ str.substr(found) };
		}

		auto const found = str.find_last_of(find[0]);
		if (found == std::string_view::npos || found + 1 >= str.size())
			return{};

		return std::string{ str.substr(found + 1) };
	}
}