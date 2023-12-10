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

std::vector<std::vector<Tint>> extractHistories(std::vector<std::string> const& lines) {
	std::vector<std::vector<Tint>> extracted;
	extracted.reserve(lines.size());

#ifndef __INTELLISENSE__
	for(auto const& line : lines) {
		extracted.emplace_back(
			line
				| std::views::split(' ')
				| std::views::transform([](auto&& range){ return strToInt(std::string_view(range.begin(), range.end())); })
				| std::ranges::to<std::vector>()
		);
	}
#endif

	return extracted;
}

Tint extrapolate(std::vector<Tint> const& history) {
	std::vector<std::vector<Tint>> diffs;
	diffs.emplace_back(history);

	while(true) {
		std::vector<Tint> newRow;

		bool isZerosOnly = true;
		
		for(size_t i = 1; i < diffs.back().size(); ++i) {
			auto const diff = diffs.back()[i] - diffs.back()[i - 1];
			newRow.emplace_back(diff);

			if(diff != 0) {
				isZerosOnly = false;
			}
		}

		diffs.emplace_back(std::move(newRow));

		if(isZerosOnly) {
			break;
		}
	}

	diffs.back().emplace_back(0);

	for(Tint i = std::ssize(diffs) - 2; i >= 0; --i) {
		// A B
		//  C
		// C = B - A
		// B = C + A
		diffs[i].emplace_back(diffs[i].back() + diffs[i + 1].back());
	}

	return diffs.front().back();
}

Tint solve(std::vector<std::vector<Tint>> const& histories) {
	Tint result{};

	for(auto const& history : histories) {
		result += extrapolate(history);
	}

	return result;
}

int main() {
	auto lines = readAllLines();

	auto histories = extractHistories(lines);

	auto solution = solve(histories);

	std::cout << solution << "\n";
	
	return 0;
}
