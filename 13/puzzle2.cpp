#include <string>
#include <iostream>
#include <unordered_set>
#include <tuple>
#include <algorithm>
#include <functional>

namespace std {
    template<>
    struct hash<std::tuple<int, int>> {
        size_t operator()(const std::tuple<int, int>& t) const noexcept {
            const auto& [i, j] = t;
            return (hash<int>{}(i) << 1) ^ hash<int>{}(j);
        }
    };
}

template<int axis>
void fold(std::unordered_set<std::tuple<int,int>>& points, int boundary)
{
    auto iter = points.begin();
    while (iter != points.end()) {
        const auto& point = *iter;
        int val = std::get<axis>(point);
        if (val > boundary) {
            auto new_point{point};
            std::get<axis>(new_point) = 2 * boundary - val;
            points.erase(iter++);
            points.insert(new_point);
        } else {
            ++iter;
        }
    }
}

std::tuple<int, int> get_size(const std::unordered_set<std::tuple<int,int>>& points)
{
    auto max_x = std::get<0>(*std::max_element(points.begin(), points.end(),
        [](const auto& a, const auto& b) {
            return std::get<0>(a) < std::get<0>(b);
        }));
    auto max_y = std::get<1>(*std::max_element(points.begin(), points.end(),
        [](const auto& a, const auto& b) {
            return std::get<1>(a) < std::get<1>(b);
        }));
    return {max_x + 1, max_y + 1};
}

std::string to_string(const std::unordered_set<std::tuple<int,int>>& points)
{
    auto [w, h] = get_size(points);
    // std::cout << "size " << w << ',' << h << '\n';
    auto line_w = w + 1;
    std::string s(line_w * h, ' ');
    for (int y = 0; y < h; ++y)
        s[w + y * line_w] = '\n';
    for (const auto& [x, y] : points)
        s[x + y * line_w] = '#';
    return s;
}

int main ()
{
    std::unordered_set<std::tuple<int,int>> points;
    for (std::string line; std::getline(std::cin, line) && !line.empty();) {
        auto comma_idx = line.find(',');
        int x = std::stoi(line.substr(0, comma_idx));
        int y = std::stoi(line.substr(comma_idx+1));
        points.emplace(x, y);
    }

    // got empty line
    std::string fold_instr = "fold along ";
    auto axis_idx = fold_instr.size();
    auto boundary_idx = fold_instr.size() + 2;
    for (std::string line; std::getline(std::cin, line);) {
        if (!line.starts_with(fold_instr)) return 1;

        int boundary = std::stoi(line.substr(boundary_idx));

        switch (line[axis_idx]) {
            case 'x':
                fold<0>(points, boundary);
                break;
            case 'y':
                fold<1>(points, boundary);
                break;
            default:
                return 2;
        }
    }
    
    std::cerr << to_string(points);
    return 0;
}