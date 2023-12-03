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

bool isGear(char c) {
	return c == '*';
}

static constexpr std::string_view nums{ "0123456789" };

int extractNumber(std::string_view line, int pos) {
	auto xTo = line.find_first_not_of(nums, pos);
	auto xFrom = line.find_last_not_of(nums, pos);
	++xFrom;

	return strToInt(line.substr(xFrom, xTo - xFrom));
}

std::vector<int> extractAdjacentNumbers(std::vector<std::string> const& board, int gearX, int gearY) {
	std::vector<int> adjacentNumbers;

	for(int y = gearY - 1; y <= gearY + 1; ++y) {
		if(y < 0 || y >= board.size()) {
			continue;
		}

		if(nums.find(board[y][gearX]) == std::string::npos) {
			// no number in the center

			// look left
			if(gearX > 0 && nums.find(board[y][gearX - 1]) != std::string::npos) {
				// there is num to the left
				adjacentNumbers.emplace_back(extractNumber(board[y], gearX - 1));
			}

			// look right
			if(gearX < std::ssize(board) - 1 && nums.find(board[y][gearX + 1]) != std::string::npos) {
				// there is num to the right
				adjacentNumbers.emplace_back(extractNumber(board[y], gearX + 1));
			}
		}
		else {
			adjacentNumbers.emplace_back(extractNumber(board[y], gearX));
		}
	}

	return adjacentNumbers;
}

Tint solve(std::vector<std::string> const& board) {
	Tint result{0};

	for(int y = 0; y < board.size(); ++y) {
		for(int x = 0; x < board[y].size(); ++x) {
			if(isGear(board[y][x])) {
				auto adjacentNumbers = extractAdjacentNumbers(board, x, y);

				if(adjacentNumbers.size() == 2) {
					result += adjacentNumbers.front() * adjacentNumbers.back();
				}
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
