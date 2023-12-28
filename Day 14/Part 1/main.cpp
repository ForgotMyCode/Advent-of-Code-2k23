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

Tint solve(std::vector<std::string>& lines) {
	std::vector<Tint> rootRow(lines.front().length(), 0);

	Tint result{};
	Tint const fullLoad = std::ssize(lines);

	for(int idxLine = 0; idxLine < std::ssize(lines); ++idxLine) {
		auto const& line = lines[idxLine];

		for(Tint i = 0; i < std::ssize(line); ++i) {
			switch(line[i]) {
			case 'O':
				result += fullLoad - rootRow[i];
				++rootRow[i];
				break;
			case '#':
				rootRow[i] = idxLine + 1;
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

int main() {
	auto lines = readAllLines();

	auto solution = solve(lines);

	std::cout << solution << "\n";
	
	return 0;
}
