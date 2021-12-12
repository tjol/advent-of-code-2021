#include <algorithm>
#include <vector>
#include <string_view>
#include <optional>
#include <iostream>
#include <string>
#include <numeric>

enum class Bracket {
    curvy,
    curly,
    square,
    pointy
};

inline std::optional<Bracket> parse_opening(char c)
{
    switch (c) {
        case '(': return Bracket::curvy;
        case '{': return Bracket::curly;
        case '[': return Bracket::square;
        case '<': return Bracket::pointy;
        default: return {};
    }
}

inline char closing_char(Bracket b)
{
    switch (b) {
        case Bracket::curvy: return ')';
        case Bracket::curly: return '}';
        case Bracket::square: return ']';
        case Bracket::pointy: return '>';
        default: return 0;
    }
}


std::vector<Bracket> process(std::string_view s)
{
    std::vector<Bracket> stack;
    char closing = '\xff';

    while (!s.empty()) {
        char c = s[0];
        if (c == closing) {
            stack.pop_back();
            closing = stack.empty() ? '\xff' : closing_char(stack.back());
        } else {
            // valid opening?
            auto bracket = parse_opening(s[0]);
            if (bracket.has_value()) {
                stack.push_back(*bracket);
                closing = closing_char(*bracket);
            } else {
                // corrupt
                return {};
            }
        }
        s.remove_prefix(1);
    }
    return stack;
}

int main()
{
    std::vector<long> scores;
    for (std::string line; std::getline(std::cin, line);) {
        auto stack = process(line);

        long score = std::accumulate(stack.rbegin(), stack.rend(), 0L,
            [](long score, Bracket b) {
                score *= 5;
                switch (b) {
                case Bracket::curvy:  score += 1; break;
                case Bracket::square: score += 2; break;
                case Bracket::curly:  score += 3; break;
                case Bracket::pointy: score += 4; break;
                }
                return score;
            });

        if (score != 0)
            scores.push_back(score);
    }

    // take the median
    auto median = scores.begin() + (scores.size() / 2);
    std::nth_element(scores.begin(), median, scores.end());
    std::cout << *median << '\n';
}
