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
#include <print>

using Tint = long long;
static constexpr std::string_view nums{ "0123456789" };
static constexpr std::string_view alpha{ "qwertzuiopasdfghjklyxcvbnm" };

std::vector<std::string> readAllLines() {
	std::vector<std::string> lines;
	std::string line;

	while(std::getline(std::cin, line)) {
		lines.emplace_back(line);
	}

	return lines;
}

Tint strToInt(std::string_view str) {
	Tint ret{};

	if(std::from_chars(str.data(), str.data() + str.length(), ret).ec == std::errc{}) {
		return ret;
	}

	throw;
}

enum class Direction : Tint {
	DNorth,
	DSouth,
	DEast,
	DWest
};

void rotateHorizontal(std::vector<std::string>& lines, Tint bottom, Tint top) {
	std::vector<Tint> rootCol(lines.size(), bottom);
	Tint const d = top > bottom ? 1 : -1;

	for(Tint col = bottom; col != top; col += d) {
		for(Tint row = 0; row < std::ssize(lines); ++row) {
			switch(lines[row][col]) {
			case 'O':
				lines[row][col] = '.';
				lines[row][rootCol[row]] = 'O';
				rootCol[row] += d;
				break;
			case '#':
				rootCol[row] = col + d;
				break;
			case '.':
				break;
			default:
				throw;
			}
		}
	}
}

void rotateVertical(std::vector<std::string>& lines, Tint bottom, Tint top) {
	std::vector<Tint> rootRow(lines.front().size(), bottom);
	Tint const d = top > bottom ? 1 : -1;

	for(Tint row = bottom; row != top; row += d) {
		for(Tint col = 0; col < std::ssize(lines[row]); ++col) {
			switch(lines[row][col]) {
			case 'O':
				lines[row][col] = '.';
				lines[rootRow[col]][col] = 'O';
				rootRow[col] += d;
				break;
			case '#':
				rootRow[col] = row + d;
				break;
			case '.':
				break;
			default:
				throw;
			}
		}
	}
}

Tint evaluate(std::vector<std::string>& lines) {
	Tint result{};
	Tint const fullLoad = std::ssize(lines);

	for(int idxLine = 0; idxLine < std::ssize(lines); ++idxLine) {
		auto const& line = lines[idxLine];

		for(Tint i = 0; i < std::ssize(line); ++i) {
			switch(line[i]) {
			case 'O':
				result += fullLoad - idxLine;
				break;
			case '#':
				break;
			case '.':
				break;
			default:
				throw;
			}
		}
	}

	return result;
}

Tint solve(std::vector<std::string>& lines) {
	Tint const height = std::ssize(lines);
	Tint const width = std::ssize(lines.front());

	std::unordered_map<std::string, Tint> cache;

	Tint const ub = 1'000'000'000LL;
	Tint i;

	for(i = 0; i < ub; ++i) {
		rotateVertical(lines, 0, height); // north
		rotateHorizontal(lines, 0, width); // west
		rotateVertical(lines, height - 1, -1); // south
		rotateHorizontal(lines, width - 1, -1); // east

		auto key = std::reduce(lines.begin(), lines.end());
		auto it = cache.find(key);
		if(it != cache.end()) {
			Tint const diff = i - it->second;
			Tint remains = ub - i;
			Tint repeats = remains / diff;
			i += 1 + repeats * diff;
			break;
		}
		cache[key] = i;
	}

	for(; i < ub; ++i) {
		rotateVertical(lines, 0, height); // north
		rotateHorizontal(lines, 0, width); // west
		rotateVertical(lines, height - 1, -1); // south
		rotateHorizontal(lines, width - 1, -1); // east
	}

	return evaluate(lines);
}

int main() {
	auto lines = readAllLines();

	auto solution = solve(lines);

	std::cout << solution << "\n";
	
	return 0;
}
