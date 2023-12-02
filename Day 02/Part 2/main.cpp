#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <charconv>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>


std::vector<std::string> readAllLines() {
	std::vector<std::string> lines;
	std::string line;

	while(std::getline(std::cin, line)) {
		lines.emplace_back(line);
	}

	return lines;
}

using Tint = int;

struct Game {
	int id{};
	std::vector<std::unordered_map<std::string, int>> reveals{};
};

Tint strToInt(std::string_view str) {
	Tint ret{};

	if(std::from_chars(str.data(), str.data() + str.length(), ret).ec == std::errc{}) {
		return ret;
	}

	throw;
}

Game parseGame(std::string_view raw) {
	Game game{};

	static constexpr std::string_view nums{"0123456789"};
	static constexpr std::string_view alpha{"qwertzuiopasdfghjklyxcvbnm"};
	
	auto gameIdBegin = raw.find_first_of(nums);
	auto gameIdEnd = raw.find_first_not_of(nums, gameIdBegin);

	game.id = strToInt(raw.substr(gameIdBegin, gameIdEnd - gameIdBegin));

	auto groupBegin = gameIdEnd;

	while(true) {
		auto groupEnd = raw.find(';', groupBegin);
		game.reveals.emplace_back();

		auto subGroupBegin = groupBegin;

		while(true) {
			auto countBegin = raw.find_first_of(nums, subGroupBegin);
			if(countBegin >= groupEnd) {
				break;
			}
			auto countEnd = raw.find_first_not_of(nums, countBegin);

			auto colorBegin = raw.find_first_of(alpha, countEnd);
			auto colorEnd = raw.find_first_not_of(alpha, colorBegin);

			game.reveals.back()[std::string(raw.substr(colorBegin, colorEnd - colorBegin))] = strToInt(raw.substr(countBegin, countEnd - countBegin));
			subGroupBegin = colorEnd;
		}

		if(groupEnd == std::string_view::npos) {
			break;
		}

		groupBegin = groupEnd + 1;
	}

	return game;
}

template<typename It>
std::vector<Game> parseGames(It first, It last) {
	std::vector<Game> games;

	for(auto it = first; it != last; ++it) {
		games.emplace_back(parseGame(*it));
	}

	return games;
}

Tint getGamePower(Game const& game) {
	Tint power = 1;
	std::unordered_map<std::string_view, Tint> minCounts;

	for(auto const& reveal : game.reveals) {

		for(auto const& [color, count] : reveal) {
			auto it = minCounts.find(color);

			if(it == minCounts.end()) {
				minCounts[color] = count;
			}
			else {
				it->second = std::max(it->second, count);
			}
		}
	}

	for(auto const& [color, count] : minCounts) {
		power *= count;
	}

	return power;
}

template<typename It>
Tint solve(It first, It last) {
	Tint powerSum{0};

	for(auto it = first; it != last; ++it) {
		powerSum += getGamePower(*it);
	}

	return powerSum;
}

int main() {
	auto lines = readAllLines();

	auto games = parseGames(lines.begin(), lines.end());

	auto solution = solve(games.begin(), games.end());

	std::cout << solution << "\n";

	return 0;
}
