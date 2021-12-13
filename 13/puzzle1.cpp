#include <string>
#include <iostream>
#include <set>
#include <tuple>

template<int axis>
void fold(std::set<std::tuple<int,int>>& points, int boundary)
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

int main ()
{
    std::set<std::tuple<int,int>> points;
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
        break;
    }
    std::cout << points.size() << std::endl;
    return 0;
}