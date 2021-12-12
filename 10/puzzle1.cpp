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

char process(std::string_view s)
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
                return c;
            }
        }
        s.remove_prefix(1);
    }
    return '\0';
}

int main()
{
    int score = 0;
    for (std::string line; std::getline(std::cin, line);) {
        auto error_char = process(line);
        switch (error_char) {
        case 0:
            break; // valid or incomplete
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
        default:
            return 127;
        }
    }
    std::cout << score << '\n';
}
