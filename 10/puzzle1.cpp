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
};

ParseResult read_one_chunk(std::string_view s)
{
    auto bracket = parse_opening(s[0]);
    if (bracket.has_value()) {
        Chunk c = { *bracket, {} };
        auto closing = closing_char(c.bracket);
        s.remove_prefix(1);
        while (!s.empty()) {
            if (closing == s[0]) {
                // done
                return {s.substr(1), c, 0};
            } else {
                // must be a chunk
                auto result = read_one_chunk(s);
                if (result.chunk.has_value()) {
                    // good
                    c.children.push_back(std::move(result.chunk).value());
                    s = result.rest;
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
        return {s, {}, closing};
    } else {
        // invalid opening bracket
        return {s, {}, 0};
    }
}

int main()
{
    int score = 0;
    for (std::string line; std::getline(std::cin, line);) {
        char error_char = 0;
        std::vector<Chunk> chunks;
        std::string_view line_view{line};
        while (!line_view.empty()) {
            auto result = read_one_chunk(line_view);
            if (result.chunk.has_value()) {
                chunks.push_back(std::move(result.chunk).value());
                line_view = result.rest;
            } else {
                // if (result.rest.empty()) {
                //     std::cout << "Incomplete\n";
                // } else {
                //     std::cout << "Illegal " << result.rest[0];
                //     if (result.expected != 0) {
                //         std::cout << " (expected " << result.expected << ")";
                //     }
                //     std::cout << '\n';
                // }
                error_char = result.rest.empty() ? -1 : result.rest[0];
                break;
            }
        }
        switch (error_char) {
        case 0:
            // std::cout << "valid\n";
            break; // valid
        case ')':
            score += 3;
            break;
        case ']':
            score += 57;
            break;
        case '}':
            score += 1197;
            break;
        case '>':
            score += 25137;
            break;
        case -1:
            // incomplete
            break;
        default:
            return 127;
        }
    }
    std::cout << score << '\n';
}
