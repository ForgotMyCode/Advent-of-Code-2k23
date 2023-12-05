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

using Tint = long long;
static constexpr std::string_view nums{ "0123456789" };
static constexpr std::string_view alpha{ "qwertzuiopasdfghjklyxcvbnm" };

Tint strToInt(std::string_view str) {
	Tint ret{};

	if(std::from_chars(str.data(), str.data() + str.length(), ret).ec == std::errc{}) {
		return ret;
	}

	throw;
}

struct Map {
	struct KeyVal {
		Tint destination{};
		Tint source{};
		Tint length{};

		bool operator<(KeyVal const& rhs) const noexcept {
			return source < rhs.source;
		}

		bool operator>(KeyVal const& rhs) const noexcept {
			return source > rhs.source;
		}
	};

	std::string sourceName;
	std::string resultName;
	std::vector<KeyVal> transfers;
};

struct MapExtractionResult {
	std::vector<Tint> seeds;
	std::unordered_map<std::string, Map> maps;
};

MapExtractionResult extractMaps(std::vector<std::string> const& rawMaps) {
	std::unordered_map<std::string, Map> extractedMaps;
	std::vector<Tint> seeds;
	Map currentMap;

	enum class EParserState {
		expectFirstRow,
		expectNumber,
		expectMapHead,
		expectBlank,
	};

	EParserState parserState = EParserState::expectFirstRow;

	for(auto const& line : rawMaps) {
		switch(parserState) {
		case EParserState::expectFirstRow:
		{
			seeds = line
				| std::views::split(' ')
				| std::views::drop(1)
				| std::views::transform([](auto&& in) -> Tint {
					return strToInt(std::string_view(in.begin(), in.end()));
				})
				| std::ranges::to<std::vector>();

			parserState = EParserState::expectBlank;
		}
		break;

		case EParserState::expectBlank:
		{
			parserState = EParserState::expectMapHead;
			continue;
		}
		break;

		case EParserState::expectNumber:
		{
			if(line.length() == 0) {
				parserState = EParserState::expectMapHead;
				std::sort(currentMap.transfers.begin(), currentMap.transfers.end());
				extractedMaps[currentMap.sourceName] = std::move(currentMap);
				currentMap = Map{};
				continue;
			}

			auto const triplet = line
				| std::views::split(' ')
				| std::views::transform([](auto&& in) -> Tint {
					return strToInt(std::string_view(in.begin(), in.end()));
				})
				| std::ranges::to<std::vector>();

			assert(triplet.size() == 3);

			currentMap.transfers.emplace_back(
				Map::KeyVal{.destination = triplet[0], .source = triplet[1], .length = triplet[2]}
			);
		}
		break;

		case EParserState::expectMapHead:
		{
			auto sourceNameBeginIdx = 0;
			auto sourceNameEndIdx = line.find_first_not_of(alpha);
			
			auto destNameBeginIdx = sourceNameEndIdx + 4;
			auto destNameEndIdx = line.find_first_not_of(alpha, destNameBeginIdx);

			currentMap.sourceName = line.substr(sourceNameBeginIdx, sourceNameEndIdx - sourceNameBeginIdx);
			currentMap.resultName = line.substr(destNameBeginIdx, destNameEndIdx - destNameBeginIdx);

			parserState = EParserState::expectNumber;
		}
		break;
		}
	}
	
	return {seeds, extractedMaps};
}

Tint transfer(std::unordered_map<std::string, Map> const& maps, std::string const& state, Tint value) {
	auto const& newState = maps.at(state).resultName;

	auto const& transfers = maps.at(state).transfers;

	auto it = std::lower_bound(transfers.rbegin(), transfers.rend(), Map::KeyVal{0, value, 0}, std::greater<Map::KeyVal>{});

	if(it == transfers.rend() || it->source + it->length < value) {
		return value;
	}

	return it->destination + (value - it->source);
}

Tint solve(std::vector<Tint> const& seeds, std::unordered_map<std::string, Map> const& maps) {
	std::string currentState = "seed";

	std::vector<Tint> values = seeds;

	while(currentState != "location") {
		for(auto& value : values) {
			value = transfer(maps, currentState, value);
		}

		currentState = maps.at(currentState).resultName;
	}

	return *std::min_element(values.begin(), values.end());
}

int main() {
	auto lines = readAllLines();
	lines.emplace_back();

	auto [seeds, maps] = extractMaps(lines);	

	auto solution = solve(seeds, maps);

	std::cout << solution << "\n";

	return 0;
}
