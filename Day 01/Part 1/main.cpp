#include <algorithm>
#include <cassert>
#include <cctype>
#include <charconv>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
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

Tint getCalibrationValue(std::string_view str) {
	Tint firstDigit = -1;

	for(auto it = str.begin(); it != str.end(); ++it) {
		if(std::isdigit(*it)) {
			firstDigit = (*it) - '0';
			break;
		}
	}

	assert(firstDigit != -1);

	Tint lastDigit = -1;

	for(auto it = str.rbegin(); it != str.rend(); ++it) {
		if(std::isdigit(*it)) {
			lastDigit = (*it) - '0';
			break;
		}
	}

	assert(lastDigit != -1);

	return firstDigit*10 + lastDigit;
}

int main() {
	auto lines = readAllLines();

	auto calibrationsRange = lines |
		std::views::transform(&getCalibrationValue);

	auto result = std::ranges::fold_left(calibrationsRange, Tint{}, std::plus<Tint>{});

	std::cout << result << "\n";

	return 0;
}
