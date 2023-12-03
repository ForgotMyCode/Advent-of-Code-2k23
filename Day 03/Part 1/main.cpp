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

Tint strToInt(std::string_view str) {
	Tint ret{};

	if(std::from_chars(str.data(), str.data() + str.length(), ret).ec == std::errc{}) {
		return ret;
	}

	throw;
}

struct Number {
	Tint value{};
	int xFrom{};
	int xTo{};
	int y{};
};

static constexpr std::string_view nums{ "0123456789" };

std::vector<Number> extractNumbers(std::vector<std::string> const& board) {
	std::vector<Number> extractedNumbers;

	for(int y = 0; y < board.size(); ++y) {
		auto& line = board[y];

		size_t xFrom = 0;

		while(true) {

			xFrom = line.find_first_of(nums, xFrom);

			if(xFrom == std::string::npos) {
				break;
			}

			size_t xTo = line.find_first_not_of(nums, xFrom);

			extractedNumbers.emplace_back(Number{
				.value = strToInt(line.substr(xFrom, xTo - xFrom)),
				.xFrom = static_cast<int>(xFrom),
				.xTo = static_cast<int>(xTo == std::string::npos ? board.size() : xTo), 
				.y = y
			});

			xFrom = xTo;
		}
	}

	return extractedNumbers;
}

bool hasSymbolAround(Number const& number, std::vector<std::string> const& board) {
	for(int y = std::max(0, number.y - 1); y <= std::min(number.y + 1, int(std::ssize(board) - 1)); ++y) {
		for(int x = std::max(0, number.xFrom - 1); x < std::min(number.xTo + 1, int(std::ssize(board))); ++x) {
			auto const query = board[y][x];
			if(query != '.' && nums.find(query) == std::string::npos) {
				return true;
			}
		}
	}

	return false;
}

Tint solve(std::vector<std::string> const& board) {
	auto numbers = extractNumbers(board);
	Tint result{0};

	for(auto const& number : numbers) {
		if(hasSymbolAround(number, board)) {
			result += number.value;
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
