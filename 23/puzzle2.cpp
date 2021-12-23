
/*


squares:

0  1  .  2  .  3  .  4  .  5  6
      7     9    11    13
      8    10    12    14
     15    17    19    21
     16    18    20    22


matrix: hallway 0-6 to room 7-14
number of steps
rows are from, columns are to


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
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 0, 2, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 0, 2, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0
};

const std::vector<int64_t> DISTANCE = {
    0,  1,  3,  5,  7,  9,  10, 3,  4,  5,  6,  7,  8,  9,  10, 5,  6,  7,  8,  9,  10, 11, 12,
    1,  0,  2,  4,  6,  8,  9,  2,  3,  4,  5,  6,  7,  8,  9,  4,  5,  6,  7,  8,  9,  10, 11,
    3,  2,  0,  2,  4,  6,  7,  2,  3,  2,  3,  4,  5,  6,  7,  4,  5,  4,  5,  6,  7,  8,  9,
    5,  4,  2,  0,  2,  4,  5,  4,  5,  2,  3,  2,  3,  4,  5,  6,  7,  4,  5,  4,  5,  6,  7,
    7,  6,  4,  2,  0,  2,  3,  6,  7,  4,  5,  2,  3,  2,  3,  8,  9,  6,  7,  4,  5,  4,  5,
    9,  8,  6,  4,  2,  0,  1,  8,  9,  6,  7,  4,  5,  2,  3,  10, 11, 8,  9,  6,  7,  4,  5,
    10, 9,  7,  5,  3,  1,  0,  9,  10, 7,  8,  5,  6,  3,  4,  11, 12, 9,  10, 7,  8,  5,  6,
    3,  2,  2,  4,  6,  8,  9,  0,  1,  4,  5,  6,  7,  8,  9,  2,  3,  6,  7,  8,  9,  10, 11,
    4,  3,  3,  5,  7,  9,  10, 1,  0,  5,  6,  7,  8,  9,  10, 1,  2,  7,  8,  9,  10, 11, 12,
    5,  4,  2,  2,  4,  6,  7,  4,  5,  0,  1,  4,  5,  6,  7,  6,  7,  2,  3,  6,  7,  8,  9,
    6,  5,  3,  3,  5,  7,  8,  5,  6,  1,  0,  5,  6,  7,  8,  7,  8,  1,  2,  7,  8,  9,  10,
    7,  6,  4,  2,  2,  4,  5,  6,  7,  4,  5,  0,  1,  4,  5,  8,  9,  6,  7,  2,  3,  6,  7,
    8,  7,  5,  3,  3,  5,  6,  7,  8,  5,  6,  1,  0,  5,  6,  9,  10, 7,  8,  1,  2,  7,  8,
    9,  8,  6,  4,  2,  2,  3,  8,  9,  6,  7,  4,  5,  0,  1,  10, 11, 8,  9,  6,  7,  2,  3,
    10, 9,  7,  5,  3,  3,  4,  9,  10, 7,  8,  5,  6,  1,  0,  11, 12, 9,  10, 7,  8,  1,  2,
    5,  4,  4,  6,  8,  10, 11, 2,  1,  6,  7,  8,  9,  10, 11, 0,  1,  8,  9,  10, 11, 12, 13,
    6,  5,  5,  7,  9,  11, 12, 3,  2,  7,  8,  9,  10, 11, 12, 1,  0,  9,  10, 11, 12, 13, 14,
    7,  6,  4,  4,  6,  8,  9,  6,  7,  2,  1,  6,  7,  8,  9,  8,  9,  0,  1,  8,  9,  10, 11,
    8,  7,  5,  5,  7,  9,  10, 7,  8,  3,  2,  7,  8,  9,  10, 9,  10, 1,  0,  9,  10, 11, 12,
    9,  8,  6,  4,  4,  6,  7,  8,  9,  6,  7,  2,  1,  6,  7,  10, 11, 8,  9,  0,  1,  8,  9,
    10, 9,  7,  5,  5,  7,  8,  9,  10, 7,  8,  3,  2,  7,  8,  11, 12, 9,  10, 1,  0,  9,  10,
    11, 10, 8,  6,  4,  4,  5,  10, 11, 8,  9,  6,  7,  2,  1,  12, 13, 10, 11, 8,  9,  0,  1,
    12, 11, 9,  7,  5,  5,  6,  11, 12, 9,  10, 7,  8,  3,  2,  13, 14, 11, 12, 9,  10, 1,  0,
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
    std::array<CellState, 23> cells;

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
        for (size_t i = 0; i < 23; ++i) {
            h <<= 1;
            h ^= std::hash<int>{}((int)b.cells[i]);
        }
        return h;
    }
};

std::vector<BurrowState::Move> BurrowState::possible_moves() const
{
    std::vector<Move> result;
    for (size_t i = 0; i < 23; ++i) {
        auto moves_here = possible_moves_by(i);
        result.insert(result.end(), moves_here.begin(), moves_here.end());
    }
    return result;
}

std::array<size_t, 4> get_target_room(CellState amphipod)
{
    switch (amphipod) {
        case CellState::A:
            return {7, 8, 15, 16};
        case CellState::B:
            return {9, 10, 17, 18};
        case CellState::C:
            return {11, 12, 19, 20};
        case CellState::D:
            return {13, 14, 21, 22};
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

    auto room = get_target_room(amphipod);

    if (cell < 7) {
        // We're in the hallway, and we want to get home
        size_t destination = 0;
        for (int i = 3; i >= 0; --i) {
            if (cells[room[i]] == CellState::Open) {
                bool all_clear = true;
                for (int j = i-1; j >= 0; --j) {
                    if (cells[room[j]] != CellState::Open) {
                        all_clear = false;
                        break;
                    }
                }
                if (all_clear) {
                    destination = room[i];
                    break;
                }
            } else if (cells[room[i]] != amphipod) {
                return {}; // occupied
            }
        }
        return find_all_paths(cell, {destination});
    } else {
        for (size_t i = 3; i >= 0; --i) {
            if (cell == room[i]) {
                return {};
            } else if (cells[room[i]] != amphipod) {
                break;
            }
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
    for (size_t i = 0; i < 23; ++i) {
        auto cost = ADJACENCY[from * 23 + i];
        if (cost > 0 && (i < 7 || from > 6 || i == get_target_room(cells[from])[0])
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
    return cells == std::array<CellState, 23>{CellState::Open, CellState::Open, CellState::Open, CellState::Open, CellState::Open, CellState::Open, CellState::Open,
        CellState::A, CellState::A, CellState::B, CellState::B, CellState::C, CellState::C, CellState::D, CellState::D,
        CellState::A, CellState::A, CellState::B, CellState::B, CellState::C, CellState::C, CellState::D, CellState::D};
}

int64_t BurrowState::badness() const
{
    int64_t result = 0;
    for (size_t i = 0; i < 23; ++i) {
        if (cells[i] != CellState::Open) {
            auto room = get_target_room(cells[i]);
            if (std::find(room.begin(), room.end(), i) == room.end()) {
                result += DISTANCE[i * 23 + room[0]] * base_cost(cells[i]);
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
    
    result.cells[15] = parse_cell('D');
    result.cells[17] = parse_cell('B');
    result.cells[19] = parse_cell('A');
    result.cells[21] = parse_cell('C');
    map.remove_prefix(map.find("  #") + 3);
    result.cells[8] = parse_cell('D');
    result.cells[10] = parse_cell('C');
    result.cells[12] = parse_cell('B');
    result.cells[14] = parse_cell('A');
    
    result.cells[16] = parse_cell(map[0]);
    result.cells[18] = parse_cell(map[2]);
    result.cells[20] = parse_cell(map[4]);
    result.cells[22] = parse_cell(map[6]);
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
    os << "\n  " << b.cells[15] << ' ' << b.cells[17] << ' ' << b.cells[19] << ' ' << b.cells[21];
    os << "\n  " << b.cells[16] << ' ' << b.cells[18] << ' ' << b.cells[20] << ' ' << b.cells[22];
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

    while (!queue.empty()) {
        std::pop_heap(queue.begin(), queue.end(), priocmp);
        auto this_move = queue.back();
        queue.resize(queue.size() - 1);
        if (!discovered.insert(this_move.target_state).second) continue;
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
    throw std::runtime_error("No path found?!");
}

int main()
{
    std::string map;
    map.resize(14*5);
    auto len = read(0, map.data(), map.size());
    map.resize(len);
    auto burrow = BurrowState::from_string(map);
    std::cout << search_for_solution(burrow) << std::endl;
    return 0;
}

