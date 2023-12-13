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
#include <unordered_set>
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

struct Coord {
	Tint x;
	Tint y;

	constexpr Coord(Tint x, Tint y) noexcept
		: x(x), y(y) {}
};

using Galaxy = std::vector<Coord>;

Galaxy extractGalaxy(std::vector<std::string> const& lines) {
	Galaxy galaxy;
	
	for(Tint y = 0; y < std::ssize(lines); ++y) {
		for(Tint x = 0; x < std::ssize(lines[y]); ++x) {
			if(lines[y][x] == '#') {
				galaxy.emplace_back(x, y);
			}
		}
	}

	return galaxy;
}

Tint solve(Galaxy& galaxy) {
	auto correctGalaxy = [&galaxy](Tint Coord::* member) mutable -> void {
		std::sort(galaxy.begin(), galaxy.end(), [member](Coord const& lhs, Coord const& rhs) {
			return lhs.*member < rhs.*member;
		});

		Tint offset = 0;
		Tint previousCoord = galaxy.front().*member;

		for(auto& coords : galaxy) {
			auto& coord = coords.*member;

			auto const currentOffset = std::max(Tint(0), coord - previousCoord - 1);
			offset += currentOffset * Tint(999'999);

			previousCoord = coord;
			coord += offset;
		}
	};

	auto distance = [](Coord const& a, Coord const& b) {
		return std::abs(a.x - b.x) + std::abs(a.y - b.y);
	};

	correctGalaxy(&Coord::x);
	correctGalaxy(&Coord::y);

	Tint result = 0;

	for(Tint i = 0; i < std::ssize(galaxy); ++i) {
		for(Tint j = i + 1; j < std::ssize(galaxy); ++j) {
			result += distance(galaxy[i], galaxy[j]);
		}
	}

	return result;
}

int main() {
	auto lines = readAllLines();

	auto galaxy = extractGalaxy(lines);

	auto solution = solve(galaxy);

	std::cout << solution << "\n";
	
	return 0;
}
