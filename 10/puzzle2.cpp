#include <algorithm>
#include <vector>
#include <string_view>
#include <optional>
#include <iostream>
#include <string>

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

struct Chunk {
    Bracket bracket;
    std::vector<Chunk> children;
};

struct ParseResult {
    std::string_view rest;
    std::optional<Chunk> chunk;
    char expected{'\0'};
    std::vector<char> unclosed;
};

ParseResult read_one_chunk(std::string_view s)
{
    auto bracket = parse_opening(s[0]);
    if (bracket.has_value()) {
        std::vector<char> unclosed;
        Chunk c = { *bracket, {} };
        auto closing = closing_char(c.bracket);
        s.remove_prefix(1);
        while (!s.empty()) {
            if (closing == s[0]) {
                // done
                return {s.substr(1), c, 0, {}};
            } else {
                // must be a chunk
                auto result = read_one_chunk(s);
                if (result.chunk.has_value()) {
                    // good
                    c.children.push_back(std::move(result.chunk).value());
                    s = result.rest;
                    unclosed.insert(unclosed.end(), result.unclosed.begin(), result.unclosed.end());
                } else {
                    // damn and blast
                    if (result.expected == 0) {
                        result.expected = closing;
                    }
                    return result;
                }
            }
        }
        // Empty string with unclosed chunk
        unclosed.push_back(closing);
        return {s, c, closing, std::move(unclosed)};
    } else {
        // invalid opening bracket
        return {s, {}, 0, {}};
    }
}

int main()
{
    std::vector<long> scores;

    for (std::string line; std::getline(std::cin, line);) {
        std::vector<Chunk> chunks;
        std::string_view line_view{line};
        while (!line_view.empty()) {
            auto result = read_one_chunk(line_view);
            if (result.chunk.has_value()) {
                chunks.push_back(std::move(result.chunk).value());
                line_view = result.rest;
                if (!result.unclosed.empty()) {
                    // std::string completion_string{result.unclosed.begin(), result.unclosed.end()};
                    // std::cout << "incomplete: " << completion_string << "\n";
                    long score = 0;
                    for (char c : result.unclosed) {
                        score *= 5;
                        switch (c) {
                        case ')': score += 1; break;
                        case ']': score += 2; break;
                        case '}': score += 3; break;
                        case '>': score += 4; break;
                        }
                    }
                    // std::cout << "score = " << score << '\n';
                    scores.push_back(score);
                    break;
                }
            } else {
                // corrupt
                // std::cout << "corrupt\n";
                break;
            }
        }

    }

    // take the median
    auto median = scores.begin() + (scores.size() / 2);
    std::nth_element(scores.begin(), median, scores.end());
    std::cout << *median << '\n';
}
