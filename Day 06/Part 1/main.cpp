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
static constexpr std::string_view nums{ "0123456789" };
static constexpr std::string_view alpha{ "qwertzuiopasdfghjklyxcvbnm" };

Tint strToInt(std::string_view str) {
	Tint ret{};

	if(std::from_chars(str.data(), str.data() + str.length(), ret).ec == std::errc{}) {
		return ret;
	}

	throw;
}

struct Race {
	int time{};
	int recordDistance{};
};

std::vector<Race> extractRaces(std::vector<std::string> const& lines) {
	auto nonEmpty = [](auto&& range) -> bool {
		return std::distance(range.begin(), range.end()) > 0;
	};

	auto toInt = [](auto&& range) -> Tint {
		return strToInt(std::string_view(range.begin(), range.end()));
	};

	std::string_view timesLine{lines[0]};
	std::string_view distancesLine{lines[1]};
	
#ifndef __INTELLISENSE__
	auto timesRange = timesLine
		| std::views::split(' ')
		| std::views::drop(1)
		| std::views::filter(nonEmpty)
		| std::views::transform(toInt);

	auto distancesRange = distancesLine
		| std::views::split(' ')
		| std::views::drop(1)
		| std::views::filter(nonEmpty)
		| std::views::transform(toInt);

	return std::views::zip(timesRange, distancesRange)
		| std::views::transform([](auto&& tuple)->Race{ return Race{std::get<0>(tuple), std::get<1>(tuple)}; })
		| std::ranges::to<std::vector>();
#endif
}

Tint solve(std::vector<Race> const& races) {

	Tint result = 1;
	
	for(auto const [time, recordDistance] : races) {
		Tint const a = -1;
		Tint const b = time;
		Tint const c = -recordDistance-1;

		double const d = std::sqrt(b*b - 4*a*c);

		Tint const low = static_cast<Tint>(std::ceil((-b+d)/(2*a)));
		Tint const high = static_cast<Tint>(std::floor((-b-d)/(2*a)));

		result *= (high - low + 1);
	}

	return result;
}

int main() {
	auto lines = readAllLines();
	lines.emplace_back();

	auto races = extractRaces(lines);

	auto solution = solve(races);

	std::cout << solution << "\n";
	
	return 0;
}
