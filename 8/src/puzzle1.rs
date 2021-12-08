use std::io::{stdin, BufRead};

const SEGMENT_SETS: [u8; 10] = [
    0b01110111, // 0
    0b00100100, // 1
    0b01011101, // 2
    0b01101101, // 3
    0b00101110, // 4
    0b01101011, // 5
    0b01111011, // 6
    0b00100101, // 7
    0b01111111, // 8
    0b01101111, // 9
];

const LETTERS: &str = &"abcdefg";

fn parse_digit_code(s: &str) -> u8 {
    s.chars()
        .map(|c| (1 << LETTERS.find(c).unwrap()) as u8)
        .reduce(|a, b| a | b)
        .unwrap()
}

fn parse_digits(s: &str) -> Vec<u8> {
    s.trim()
        .split_ascii_whitespace()
        .map(parse_digit_code)
        .collect()
}

fn main() {
    println!(
        "{}",
        stdin()
            .lock()
            .lines()
            .filter_map(|l| l.ok())
            .flat_map(|l| parse_digits(l.split_once('|').unwrap().1))
            .filter(|d| [2, 3, 4, 7].contains(&d.count_ones()))
            .count()
    )
}
