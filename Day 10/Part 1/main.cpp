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

	bool operator==(Coord const&) const noexcept = default;

	struct Hash {
		size_t operator()(Coord const& coord) const noexcept {
			static std::hash<Tint> stdHash{};
			return (stdHash(coord.x) << 32) ^ stdHash(coord.y);
		}
	};

	constexpr Coord(Tint x, Tint y) noexcept
		: x(x), y(y) {}
};

using PipeSystem = std::vector<std::vector<std::vector<Coord>>>;

PipeSystem extractPipeSystem(std::vector<std::string> const& lines, Coord& outStart) {
	PipeSystem pipeSystem;
	
	for(auto const& [y, line] : std::views::enumerate(lines)) {
		auto& pipeLine = pipeSystem.emplace_back();

		
		for(auto const& [x, c] : std::views::enumerate(line)) {
			auto& coords = pipeLine.emplace_back();

			switch(c) {
			case '|':
				coords.emplace_back(0, 1);
				coords.emplace_back(0, -1);
				break;
			case '-':
				coords.emplace_back(1, 0);
				coords.emplace_back(-1, 0);
				break;
			case 'L':
				coords.emplace_back(0, -1);
				coords.emplace_back(1, 0);
				break;
			case 'J':
				coords.emplace_back(0, -1);
				coords.emplace_back(-1, 0);
				break;
			case '7':
				coords.emplace_back(0, 1);
				coords.emplace_back(-1, 0);
				break;
			case 'F':
				coords.emplace_back(0, 1);
				coords.emplace_back(1, 0);
				break;
			case 'S':
				outStart.x = x;
				outStart.y = y;
				break;
			}
		}
	}

	auto& startCoords = pipeSystem[outStart.y][outStart.x];

	if(outStart.x > 0) {
		for(auto const [dx, dy] : pipeSystem[outStart.y][outStart.x - 1]) {
			if(dx == 1) {
				startCoords.emplace_back(-1, 0);
				break;
			}
		}
	}

	if(outStart.x < std::ssize(pipeSystem.front()) - 1) {
		for(auto const [dx, dy] : pipeSystem[outStart.y][outStart.x + 1]) {
			if(dx == -1) {
				startCoords.emplace_back(1, 0);
				break;
			}
		}
	}

	if(outStart.y > 0) {
		for(auto const [dx, dy] : pipeSystem[outStart.y - 1][outStart.x]) {
			if(dy == 1) {
				startCoords.emplace_back(0, -1);
				break;
			}
		}
	}

	if(outStart.y < std::ssize(pipeSystem) - 1) {
		for(auto const [dx, dy] : pipeSystem[outStart.y + 1][outStart.x]) {
			if(dy == -1) {
				startCoords.emplace_back(0, 1);
				break;
			}
		}
	}

	return pipeSystem;
}

Tint solve(PipeSystem& pipeSystem, Coord& startCoord) {
	Tint const height = std::ssize(pipeSystem);
	Tint const width = std::ssize(pipeSystem.front());

	std::unordered_set<Coord, Coord::Hash> visited;
	visited.emplace(startCoord);

	std::vector<Coord> current, next;
	current.emplace_back(startCoord);

	while(!current.empty()) {
		next.clear();

		for(auto const [x, y] : current) {
			for(auto const [dx, dy] : pipeSystem[y][x]) {
				auto const xx = x + dx;
				auto const yy = y + dy;
				Coord const coord(xx, yy);

				if(xx >= 0 && yy >= 0 && xx < width && yy < height && pipeSystem[yy][xx].size() > 0 && visited.emplace(coord).second) {
					next.emplace_back(coord);
				}
			}
		}

		std::swap(current, next);
	}

	return std::ssize(visited) / 2;
}

int main() {
	auto lines = readAllLines();

	Coord start(-1, -1);
	auto pipeSystem = extractPipeSystem(lines, start);

	auto solution = solve(pipeSystem, start);

	std::cout << solution << "\n";
	
	return 0;
}
