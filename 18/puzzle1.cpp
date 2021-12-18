#include <array>
#include <cmath>
#include <iostream>
#include <utility>
#include <stdexcept>

class Snailfish
{
    // max depth is 5 -> 2^5 = 32
    std::array<int8_t, 32> m_values;

    void explode(size_t idx);

public:
    Snailfish();
    Snailfish(const Snailfish&) = default;
    Snailfish& operator=(const Snailfish&) = default;

    Snailfish(int8_t value);

    Snailfish& operator+=(const Snailfish& other);
    Snailfish operator+(const Snailfish& other) const;
    
    void reduce();

    std::pair<Snailfish, Snailfish> split() const;
    bool is_single_number() const;
    long magnitude() const;

    friend std::ostream& operator<<(std::ostream& str, const Snailfish& fish);
    friend std::istream& operator>>(std::istream& stream, Snailfish& fish);
};

Snailfish::Snailfish()
{
    std::fill(m_values.begin(), m_values.end(), int8_t(-1));
}

Snailfish::Snailfish(int8_t value)
    : Snailfish()
{
    m_values[0] = value;
}

Snailfish& Snailfish::operator+=(const Snailfish& other)
{
    if (m_values[0] < 0) {
        // empty number!
        return (*this = other);
    }
    // Shift our values
    for (size_t i = 0; i < 16; ++i) {
        m_values[i] = m_values[i<<1];
    }
    // Copy their values
    for (size_t i = 0; i < 16; ++i) {
        m_values[16+i] = other.m_values[i<<1];
    }

    reduce();

    return *this;
}

Snailfish Snailfish::operator+(const Snailfish& other) const
{
    Snailfish result = *this;
    result += other;
    return result;
}

void Snailfish::reduce()
{
reduce_loop:
    // Check for depth-5 pairs
    for (size_t i = 1; i < 32; i += 2) {
        if (m_values[i] >= 0) {
            explode(i - 1);
            goto reduce_loop;
        }
    }

    // Check for > 9
    for (size_t i = 0; i < 32; i += 2) {
        if (m_values[i] > 9) {
            // split
            float half = 0.5f * float(m_values[i]);
            int8_t left = int8_t(std::floor(half));
            int8_t right = int8_t(std::ceil(half));

            size_t step = 2;
            while (i + step < 32 && m_values[i+step] < 0) step <<= 1;
            step >>= 1;

            m_values[i] = left;
            m_values[i+step] = right;
            goto reduce_loop;
        }
    }
}

void Snailfish::explode(size_t idx)
{
    for (int i = idx - 1; i >= 0; --i) {
        if (m_values[i] >= 0) {
            m_values[i] += m_values[idx];
            break;
        }
    }
    for (int i = idx + 2; i < 32; ++i) {
        if (m_values[i] >= 0) {
            m_values[i] += m_values[idx+1];
            break;
        }
    }
    m_values[idx] = 0;
    m_values[idx+1] = -1;
}

long Snailfish::magnitude() const
{
    if (is_single_number()) {
        return m_values[0];
    } else {
        auto [a, b] = split();
        return 3 * a.magnitude() + 2 * b.magnitude();
    }
}

std::pair<Snailfish, Snailfish> Snailfish::split() const
{
    Snailfish a, b;
    for (size_t i = 0; i < 16; ++i) {
        a.m_values[i<<1] = m_values[i];
        b.m_values[i<<1] = m_values[16 + i];
    }
    return {a, b};
}

bool Snailfish::is_single_number() const
{
    return std::all_of(m_values.begin() + 1, m_values.end(), [](auto v) { return v < 0; });
}

std::ostream& operator<<(std::ostream& stream, const Snailfish& fish)
{
    if (fish.is_single_number()) {
        stream << int(fish.m_values[0]);
    } else {
        auto [a, b] = fish.split();
        stream << '[' << a << ',' << b << ']';
    }

    return stream;
}

std::istream& operator>>(std::istream& stream, Snailfish& fish)
{
    int next_char;
    while (isspace(next_char = stream.peek())) stream.get();
    if (next_char == EOF) return stream;

    if (next_char == '[') {
        stream.get();
        stream >> fish;
        if (stream.get() != ',') throw std::runtime_error("syntax error");
        Snailfish other;
        stream >> other;
        fish += other;
        if (stream.get() != ']') throw std::runtime_error("syntax error");
    } else {
        // this better be a number
        int val;
        stream >> val;
        fish = Snailfish(int8_t(val));
    }

    return stream;
}

int main()
{
    Snailfish acc;
    do {
        Snailfish n;
        std::cin >> n;
        if (std::cin.eof()) break;
        acc += n;
    } while (1);
    std::cout << acc.magnitude() << '\n';
    return 0;
}
