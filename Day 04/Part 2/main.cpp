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

using Tint = int;

Tint strToInt(std::string_view str) {
	Tint ret{};

	if(std::from_chars(str.data(), str.data() + str.length(), ret).ec == std::errc{}) {
		return ret;
	}

	throw;
}

struct ScratchCard {
	int id{};
	std::vector<int> winningNumbers;
	std::vector<int> numbers;
};

static constexpr std::string_view nums{ "0123456789" };

std::vector<ScratchCard> extractScratchCards(std::vector<std::string> const& rawScratchCards) {
	std::vector<ScratchCard> extractedScratchCards;

	for(auto const& rawScratchCard : rawScratchCards) {

		ScratchCard scratchCard{};

		// extract id
		auto idBeginIdx = rawScratchCard.find_first_of(nums, 0);
		auto idEndIdx = rawScratchCard.find_first_not_of(nums, idBeginIdx);
		scratchCard.id = strToInt(rawScratchCard.substr(idBeginIdx, idEndIdx - idBeginIdx));
		
		// find delim
		auto delimIdx = rawScratchCard.find('|', idEndIdx);
		assert(delimIdx != std::string::npos);

		// extract winning numbers

		auto endIdx = idEndIdx;

		while(true) {
			auto winningNumBeginIdx = rawScratchCard.find_first_of(nums, endIdx);
			if(winningNumBeginIdx >= delimIdx) {
				break;
			}

			auto winningNumEndIdx = rawScratchCard.find_first_not_of(nums, winningNumBeginIdx);

			scratchCard.winningNumbers.emplace_back(strToInt(rawScratchCard.substr(winningNumBeginIdx, winningNumEndIdx - winningNumBeginIdx)));
			endIdx = winningNumEndIdx;
		}

		// extract numbers

		while(true) {
			auto numBeginIdx = rawScratchCard.find_first_of(nums, endIdx);
			if(numBeginIdx == std::string::npos) {
				break;
			}

			auto numEndIdx = rawScratchCard.find_first_not_of(nums, numBeginIdx);

			scratchCard.numbers.emplace_back(strToInt(rawScratchCard.substr(numBeginIdx, numEndIdx - numBeginIdx)));
			endIdx = numEndIdx;
		}

		extractedScratchCards.emplace_back(std::move(scratchCard));
	}
	
	return extractedScratchCards;
}

Tint solve(std::vector<ScratchCard> const& scratchCards) {
	Tint result{0};

	std::vector<int> cardCounts(scratchCards.size(), 1);
	cardCounts.front() = 1;

	for(int i = 0; i < scratchCards.size(); ++i) {
		auto const& scratchCard = scratchCards[i];

		std::unordered_set<int> winningNumbers(scratchCard.winningNumbers.begin(), scratchCard.winningNumbers.end());

		int matches = 0;

		for(auto const num : scratchCard.numbers) {
			if(winningNumbers.count(num) > 0) {
				++matches;
			}
		}

		for(int j = i + 1; j <= std::min(std::ptrdiff_t(i + matches), std::ssize(scratchCards) - 1); ++j) {
			cardCounts[j] += cardCounts[i];
		}

		result += cardCounts[i];
	}

	return result;
}

int main() {
	auto lines = readAllLines();

	auto scratchCards = extractScratchCards(lines);

	auto solution = solve(scratchCards);

	std::cout << solution << "\n";

	return 0;
}
