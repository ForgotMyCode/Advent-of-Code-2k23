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

enum class EHandRate : Tint {
	ehrHighCard,
	ehrOnePair,
	ehrTwoPairs,
	ehrTreeOfAKind,
	ehrFullHouse,
	ehrFourOfAKind,
	ehrFiveOfAKind
};

EHandRate rateHand(std::string_view hand) {
	std::unordered_map<char, Tint> cardCounts;
	Tint nJokers = 0;

	for(auto const c : hand) {
		if(c == 'J') {
			++nJokers;
		}
		else {
			++cardCounts[c];
		}
	}

	if(nJokers == 5) {
		return EHandRate::ehrFiveOfAKind;
	}

	std::max_element(cardCounts.begin(), cardCounts.end(), [](auto&& lhs, auto&& rhs) {return lhs.second < rhs.second;})->second += nJokers;

	std::unordered_map<Tint, std::vector<char>> countCards;

	for(auto const& [card, count] : cardCounts) {
		countCards[count].emplace_back(card);
	}

	if(countCards.count(5) > 0) {
		return EHandRate::ehrFiveOfAKind;
	}

	if(countCards.count(4) > 0) {
		return EHandRate::ehrFourOfAKind;
	}

	if(countCards.count(3) > 0) {
		return countCards.count(2) > 0 ? EHandRate::ehrFullHouse : EHandRate::ehrTreeOfAKind;
	}

	if(countCards.count(2) > 0 && countCards.at(2).size() > 1) {
		return EHandRate::ehrTwoPairs;
	}

	if(countCards.count(2) > 0) {
		return EHandRate::ehrOnePair;
	}

	return EHandRate::ehrHighCard;
}

Tint evaluateCard(char card) {
	return static_cast<Tint>(std::string_view("J23456789TQKA").find(card));
}

struct HandAndBid {
	std::string hand;
	Tint bid{};
};

struct HandAndBidHandComparator {
	bool operator()(HandAndBid const& lhs, HandAndBid const& rhs) const noexcept {
		auto lhsRate = rateHand(lhs.hand);
		auto rhsRate = rateHand(rhs.hand);

		if(lhsRate != rhsRate) {
			return lhsRate < rhsRate;
		}

		for(int i = 0; i < 5; ++i) {
			auto lVal = evaluateCard(lhs.hand[i]);
			auto rVal = evaluateCard(rhs.hand[i]);

			if(lVal == rVal) {
				continue;
			}

			return lVal < rVal;
		}

		return false;
	}
};

std::vector<HandAndBid> extractHandsAndBids(std::vector<std::string> const& lines) {
	std::vector<HandAndBid> ret;

	for(auto const& line : lines) {
		std::string hand = line.substr(0, 5);
		
		auto bidBegin = line.find_first_of(nums, 6);
		auto bidEnd = line.find_first_not_of(nums, bidBegin);

		Tint bid = strToInt(line.substr(bidBegin, bidEnd - bidBegin));

		ret.emplace_back(HandAndBid{hand, bid});
	}

	return ret;
}

Tint solve(std::vector<HandAndBid>& handsAndBids) {

	Tint result = 0;
	
	std::sort(handsAndBids.begin(), handsAndBids.end(), HandAndBidHandComparator{});

	for(Tint i = 0; i < std::ssize(handsAndBids); ++i) {
		result += (i + 1) * handsAndBids[i].bid;
	}

	return result;
}

int main() {
	auto lines = readAllLines();

	auto handsAndBids = extractHandsAndBids(lines);

	auto solution = solve(handsAndBids);

	std::cout << solution << "\n";
	
	return 0;
}
