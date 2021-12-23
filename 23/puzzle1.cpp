
/*


squares:

0  1  .  2  .  3  .  4  .  5  6
      7     9    11    13
      8    10    12    14


matrix: hallway 0-6 to room 7-14
number of steps
rows are from, columns are to

3   4   5   6   7   8   9   10
2   3   4   5   6   7   8   9
2   3   2   3   4   5   6   7
4   5   2   3   2   3   4   5
6   7   4   5   2   3   2   3
8   9   6   7   4   5   2   3
9   10  7   8   5   6   3   4

adjacency matrix
0   1   0   0   0   0   0   0   0   0   0   0   0   0   0
1   0   2   0   0   0   0   2   0   0   0   0   0   0   0
0   2   0   2   0   0   0   2   0   2   0   0   0   0   0
0   0   2   0   2   0   0   0   0   2   0   2   0   0   0
0   0   0   2   0   2   0   0   0   0   0   2   0   2   0
0   0   0   0   2   0   1   0   0   0   0   0   0   2   0
0   0   0   0   0   1   0   0   0   0   0   0   0   0   0
0   2   2   0   0   0   0   0   1   0   0   0   0   0   0
0   0   0   0   0   0   0   1   0   0   0   0   0   0   0
0   0   2   2   0   0   0   0   0   0   1   0   0   0   0
0   0   0   0   0   0   0   0   0   1   0   0   0   0   0
0   0   0   2   2   0   0   0   0   0   0   0   1   0   0
0   0   0   0   0   0   0   0   0   0   0   1   0   0   0
0   0   0   0   2   2   0   0   0   0   0   0   0   0   1
0   0   0   0   0   0   0   0   0   0   0   0   0   1   0

distance matrix
0   1   3   5   7   9   10  3   4   5   6   7   8   9   10
1   0   2   4   6   8   9   2   3   4   5   6   7   8   9
3   2   0   2   4   6   7   2   3   2   3   4   5   6   7
5   4   2   0   2   4   5   4   5   2   3   2   3   4   5
7   6   4   2   0   2   3   6   7   4   5   2   3   2   3
9   8   6   4   2   0   1   8   9   6   7   4   5   2   3
10  9   7   5   3   1   0   9   10  7   8   5   6   3   4
3   2   2   4   6   8   9   0   1   4   5   6   7   8   9
4   3   3   5   7   9   10  1   0   5   6   7   8   9   10
5   4   2   2   4   6   7   4   5   0   1   4   5   6   7
6   5   3   3   5   7   8   5   6   1   0   5   6   7   8
7   6   4   2   2   4   5   6   7   4   5   0   1   4   5
8   7   5   3   3   5   6   7   8   5   6   1   0   5   6
9   8   6   4   2   2   3   8   9   6   7   4   5   0   1
10  9   7   5   3   3   4   9   10  7   8   5   6   1   0

"Regions"
hallway: 0,1,2,3,4,5,6
A goal: 8, 7
B goal: 10, 9
C goal: 12, 11
D goal: 14, 13

*/

#include <array>
#include <string_view>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_set>

#include <cstdint>
#include <unistd.h>

const std::vector<int64_t> ADJACENCY = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 0, 2, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0,
    0, 0, 2, 0, 2, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0,
    0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 2, 0,
    0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0
};

const std::vector<int64_t> DISTANCE = {
    0, 1, 3, 5, 7, 9, 10, 3, 4, 5, 6, 7, 8, 9, 10,
    1, 0, 2, 4, 6, 8, 9, 2, 3, 4, 5, 6, 7, 8, 9,
    3, 2, 0, 2, 4, 6, 7, 2, 3, 2, 3, 4, 5, 6, 7,
    5, 4, 2, 0, 2, 4, 5, 4, 5, 2, 3, 2, 3, 4, 5,
    7, 6, 4, 2, 0, 2, 3, 6, 7, 4, 5, 2, 3, 2, 3,
    9, 8, 6, 4, 2, 0, 1, 8, 9, 6, 7, 4, 5, 2, 3,
    10, 9, 7, 5, 3, 1, 0, 9, 10, 7, 8, 5, 6, 3, 4,
    3, 2, 2, 4, 6, 8, 9, 0, 1, 4, 5, 6, 7, 8, 9,
    4, 3, 3, 5, 7, 9, 10, 1, 0, 5, 6, 7, 8, 9, 10,
    5, 4, 2, 2, 4, 6, 7, 4, 5, 0, 1, 4, 5, 6, 7,
    6, 5, 3, 3, 5, 7, 8, 5, 6, 1, 0, 5, 6, 7, 8,
    7, 6, 4, 2, 2, 4, 5, 6, 7, 4, 5, 0, 1, 4, 5,
    8, 7, 5, 3, 3, 5, 6, 7, 8, 5, 6, 1, 0, 5, 6,
    9, 8, 6, 4, 2, 2, 3, 8, 9, 6, 7, 4, 5, 0, 1,
    10, 9, 7, 5, 3, 3, 4, 9, 10, 7, 8, 5, 6, 1, 0,
};

enum class CellState {
    Open,
    A,
    B,
    C,
    D
};

CellState parse_cell(char c)
{
    switch (c) {
        case '.': return CellState::Open;
        case 'A': return CellState::A;
        case 'B': return CellState::B;
        case 'C': return CellState::C;
        case 'D': return CellState::D;
        default: throw std::runtime_error("parse error");
    }
}

int64_t base_cost(CellState amphipod)
{
    switch (amphipod) {
        case CellState::A: return 1;
        case CellState::B: return 10;
        case CellState::C: return 100;
        case CellState::D: return 1000;
        default: return (int64_t)(-1L);
    }
}

struct BurrowState
{
    std::array<CellState, 15> cells;

    static BurrowState from_string(std::string_view map);

    struct Move;

    std::vector<Move> possible_moves() const;
    std::vector<Move> possible_moves_by(size_t cell) const;
    std::vector<Move> find_all_paths(size_t from, std::vector<size_t> to, std::vector<size_t> trodden = {}) const;

    int64_t badness() const;

    bool is_done() const;

    bool operator==(const BurrowState& other) const { return other.cells == cells; }
};

struct BurrowState::Move
{
    int64_t cost;
    int64_t badness;
    BurrowState target_state;
};

template<>
struct std::hash<BurrowState>
{
    size_t operator()(const BurrowState& b) const noexcept
    {
        size_t h = 0;
        for (size_t i = 0; i < 15; ++i) {
            h <<= 1;
            h ^= std::hash<int>{}((int)b.cells[i]);
        }
        return h;
    }
};

std::vector<BurrowState::Move> BurrowState::possible_moves() const
{
    std::vector<Move> result;
    for (size_t i = 0; i < 15; ++i) {
        auto moves_here = possible_moves_by(i);
        result.insert(result.end(), moves_here.begin(), moves_here.end());
    }
    return result;
}

std::pair<size_t, size_t> get_target_room(CellState amphipod)
{
    switch (amphipod) {
        case CellState::A:
            return std::make_pair(7, 8);
        case CellState::B:
            return std::make_pair(9, 10);
        case CellState::C:
            return std::make_pair(11, 12);
        case CellState::D:
            return std::make_pair(13, 14);
        default:
            throw std::runtime_error("not an amphipod");
    }
}

std::vector<BurrowState::Move> BurrowState::possible_moves_by(size_t cell) const
{
    auto amphipod = cells[cell];
    if (amphipod == CellState::Open) {
        return {};
    }

    auto [shallow, deep] = get_target_room(amphipod);

    if (cell < 7) {
        // We're in the hallway, and we want to get home
        size_t destination;
        if (cells[deep] == CellState::Open && cells[shallow] == CellState::Open) {
            destination = deep;
        } else if (cells[deep] == amphipod && cells[shallow] == CellState::Open) {
            destination = shallow;
        } else {
            // Room is occupied
            return {};
        }
        return find_all_paths(cell, {destination});
    } else {
        if (cell == deep || (cell == shallow && cells[deep] == amphipod)) {
            return {};
        }
        // We're in a room, and we might want to get out
        return find_all_paths(cell, {0,1,2,3,4,5,6});
    }
}

std::vector<BurrowState::Move> BurrowState::find_all_paths(size_t from, std::vector<size_t> to, std::vector<size_t> trodden) const
{
    if (trodden.empty()) {
        trodden.push_back(from);
    }

    // Where can I go in principle?
    std::vector<std::pair<size_t, int64_t>> neighbours;
    for (size_t i = 0; i < 15; ++i) {
        auto cost = ADJACENCY[from * 15 + i];
        if (cost > 0 && (i < 7 || i == to[0] || i == to[0] - 1 || from > 6)
            && std::find(trodden.begin(), trodden.end(), i) == trodden.end()) {
            if (cells[i] == CellState::Open) {
                neighbours.emplace_back(i, cost);
            }
        }
    }

    // Remove all directly reachable points from the list of destinations
    std::vector<bool> was_target;
    for (const auto& [idx, _] : neighbours) {
        auto n_erased = std::erase(to, idx);
        was_target.push_back(n_erased != 0);
        trodden.push_back(idx);
    }

    std::vector<Move> result;

    for (size_t i = 0; i < neighbours.size(); ++i) {
        const auto& [next_step, dist] = neighbours[i];
        BurrowState new_state = *this;
        new_state.cells[from] = CellState::Open;
        new_state.cells[next_step] = cells[from];
        auto cost = dist * base_cost(cells[from]);
        if (was_target[i]) {
            result.push_back({cost, new_state.badness(), new_state});
        }

        if (!to.empty()) {
            auto onward_paths = new_state.find_all_paths(next_step, to, trodden);
            std::transform(onward_paths.begin(), onward_paths.end(), std::back_inserter(result),
                [cost](auto move) {
                    move.cost += cost;
                    return move;
                });
        }
    }

    return result;
}

bool BurrowState::is_done() const
{
    return cells == std::array<CellState, 15>{CellState::Open, CellState::Open, CellState::Open, CellState::Open, CellState::Open, CellState::Open, CellState::Open,
        CellState::A, CellState::A, CellState::B, CellState::B, CellState::C, CellState::C, CellState::D, CellState::D};
}

int64_t BurrowState::badness() const
{
    int64_t result = 0;
    for (size_t i = 0; i < 15; ++i) {
        if (cells[i] != CellState::Open) {
            auto [shallow, deep] = get_target_room(cells[i]);
            if (i != shallow && i != deep) {
                result += DISTANCE[i * 15 + shallow] * base_cost(cells[i]);
            }
        }
    }
    return result;
}

BurrowState BurrowState::from_string(std::string_view map)
{
    map.remove_prefix(map.find("#\n#")+3);
    BurrowState result;
    result.cells[0] = parse_cell(map[0]);
    result.cells[1] = parse_cell(map[1]);
    result.cells[2] = parse_cell(map[3]);
    result.cells[3] = parse_cell(map[5]);
    result.cells[4] = parse_cell(map[7]);
    result.cells[5] = parse_cell(map[9]);
    result.cells[6] = parse_cell(map[10]);
    map.remove_prefix(map.find("###") + 3);
    result.cells[7] = parse_cell(map[0]);
    result.cells[9] = parse_cell(map[2]);
    result.cells[11] = parse_cell(map[4]);
    result.cells[13] = parse_cell(map[6]);
    map.remove_prefix(map.find("  #") + 3);
    result.cells[8] = parse_cell(map[0]);
    result.cells[10] = parse_cell(map[2]);
    result.cells[12] = parse_cell(map[4]);
    result.cells[14] = parse_cell(map[6]);
    return result;
}

std::ostream& operator<<(std::ostream& os, CellState cs)
{
    switch (cs) {
        case CellState::Open: return os << '.';
        case CellState::A: return os << 'A';
        case CellState::B: return os << 'B';
        case CellState::C: return os << 'C';
        case CellState::D: return os << 'D';
        default: return os << '?';
    }
}

std::ostream& operator<<(std::ostream& os, const BurrowState& b)
{
    os << '\n';
    os << b.cells[0] << b.cells[1] << '+' << b.cells[2] << '+' << b.cells[3] << '+' << b.cells[4] << '+' << b.cells[5] << b.cells[6];
    os << "\n  " << b.cells[7] << ' ' << b.cells[9] << ' ' << b.cells[11] << ' ' << b.cells[13];
    os << "\n  " << b.cells[8] << ' ' << b.cells[10] << ' ' << b.cells[12] << ' ' << b.cells[14];
    os << '\n';
    return os;
}

int64_t search_for_solution(BurrowState initial_state)
{
    const auto priocmp = [](const BurrowState::Move& a, const BurrowState::Move& b) {
        // return a.target_state.badness() > b.target_state.badness();
        // return a.cost > b.cost;
        return a.cost + a.badness > b.cost + b.badness;
    };
    std::vector<BurrowState::Move> queue{{0, initial_state.badness(), initial_state}};
    std::unordered_set<BurrowState> discovered;

    while (true) {
        std::pop_heap(queue.begin(), queue.end(), priocmp);
        auto this_move = queue.back();
        queue.resize(queue.size() - 1);
        discovered.insert(this_move.target_state);
        if (this_move.target_state.is_done()) {
            // YAY
            return this_move.cost;
        }
        // std::cout << "COST " << this_move.cost << this_move.target_state << "---";
        // Get the next possible moves
        auto next_moves = this_move.target_state.possible_moves();
        queue.reserve(queue.size() + next_moves.size());
        for (auto& move : next_moves) {
            // Check that we haven't done this one yet
            if (discovered.contains(move.target_state)) continue;
            // Update the cost
            move.cost += this_move.cost;
            queue.push_back(move);
            std::push_heap(queue.begin(), queue.end(), priocmp);
        }
    }
}

int main()
{
    std::string map;
    map.resize(14*5);
    auto len = read(0, map.data(), map.size());
    map.resize(len);
    auto burrow = BurrowState::from_string(map);
    std::cout << search_for_solution(burrow) << std::endl;
    // std::cout << burrow;
    // std::cout << "possible moves\n";
    // for (const auto& move : burrow.possible_moves()) {
    //     std::cout << "COST " << move.cost << move.target_state;
    // }
    return 0;
}


