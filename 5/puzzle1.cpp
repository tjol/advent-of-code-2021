#include <algorithm>
#include <compare>
#include <functional>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

using Grid = std::vector<std::vector<int>>;

struct Point
{
    int x{}, y{};
    Point(int x, int y) : x(x), y(y) {}
    Point(std::string_view s)
    {
        auto comma = s.find(',');
        auto x_s = std::string(&s[0], comma);
        s.remove_prefix(comma+1);
        auto y_s = std::string(s);
        x = stoi(x_s);
        y = stoi(y_s);
    }
};

struct Line
{
    Point start, end;

    bool is_vertical() const { return start.x == end.x; }
    bool is_horizontal() const { return start.y == end.y; }

    void mark_on_grid(Grid& grid) const
    {
        auto delta_x = end.x - start.x;
        auto delta_y = end.y - start.y;

        if (delta_x == 0) {
            int x = start.x;
            auto dy = delta_y > 0 ? 1 : -1;
            for (int y = start.y; (y - dy) != end.y; y += dy)
                ++grid[y][x];
        } else if (delta_y == 0) {
            int y = start.y;
            auto dx = delta_x > 0 ? 1 : -1;
            for (int x = start.x; (x - dx) != end.x; x += dx)
                ++grid[y][x];
        } else {
            throw std::runtime_error("nope");
        }
    }
};

std::ostream& operator<<(std::ostream& stream, const Point& p)
{
    return stream << '(' << p.x << ',' << p.y << ')';
}

std::ostream& operator<<(std::ostream& stream, const Line& l)
{
    return stream << l.start << " to " << l.end;
}

std::vector<Line> parse_vents(std::istream& input)
{
    std::vector<Line> result;
    for (std::string line; std::getline(input, line);) {
        auto end1 = line.find(" -> ");
        auto begin2 = end1 + 4;
        auto s1 = std::string_view(&line[0], end1);
        auto s2 = std::string_view(&line[begin2]);
        result.push_back({Point(s1), Point(s2)});
    }
    return result;
}

std::ostream& print_grid(std::ostream& out, const Grid& grid)
{
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            if (cell == 0) {
                out << "  .";
            } else {
                out << std::setw(3) << cell;
            }
        }
        out << '\n';
    }
    return out;
}

int main()
{
    auto vent_lines = parse_vents(std::cin);

    // for (const auto& line : vent_lines) {
    //     std::cout << line << '\n';
    // }

    // Get the size of the coordinate system
    auto max_x = [](const Line& l) { return std::max(l.start.x, l.end.x); };
    auto max_y = [](const Line& l) { return std::max(l.start.y, l.end.y); };
    
    auto width = max_x(*std::max_element(vent_lines.cbegin(), vent_lines.cend(),
        [&](const auto& l1, const auto& l2) {
            return max_x(l1) < max_x(l2);
        })) + 1;
    auto height = max_y(*std::max_element(vent_lines.cbegin(), vent_lines.cend(),
        [&](const auto& l1, const auto& l2) {
            return max_y(l1) < max_y(l2);
        })) + 1;

    // std::cout << "size: " << width << " x " << height << '\n';

    auto grid = std::vector { size_t(height), std::vector<int>(width)};

    for (const auto& l : vent_lines) {
        if (l.is_vertical() || l.is_horizontal()) {
            l.mark_on_grid(grid);
        }
    }

    // print_grid(std::cout, grid);

    // count areas >= 2
    auto dangerous = std::transform_reduce(
        grid.cbegin(), grid.cend(),
        0,
        std::plus<>(),
        [](const auto& row) {
            return std::transform_reduce(
                row.begin(), row.end(), 0, std::plus<>(),
                [](int n) { return n > 1 ? 1 : 0; });
        });

    std::cout << dangerous << '\n';

    return 0;
}
