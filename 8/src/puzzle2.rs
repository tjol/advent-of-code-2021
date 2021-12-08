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

type SegmentMap = [u8; 10];

fn decrypt(digits: &[u8]) -> Option<SegmentMap> {
    let matches_per_digit = [0b1111111111_u16; 10];
    let segment_results = [0b1111111_u8; 7];

    try_decrypt(digits, &matches_per_digit, &segment_results)
}

fn try_decrypt(
    digits: &[u8],
    matches_per_digit: &[u16],
    segment_results: &[u8],
) -> Option<SegmentMap> {
    let mut matches_per_digit: [u16; 10] = matches_per_digit.try_into().unwrap();
    let mut segment_results: [u8; 7] = segment_results.try_into().unwrap();
    let mut result = [0_u8; 10];

    let mut prev_matches = [0u16; 10];
    let mut prev_segments = [0u8; 7];
    while prev_matches != matches_per_digit || prev_segments != segment_results {
        prev_matches.copy_from_slice(&matches_per_digit);
        prev_segments.copy_from_slice(&segment_results);
        for k in 0..10 {
            let encoded = digits[k];
            let m = matches_per_digit[k];
            match m.count_ones() {
                1 => {
                    // found it
                    let digit = m.trailing_zeros() as usize;
                    result[digit] = encoded;
                    // mark the segments
                    for (j, segment) in segment_results.iter_mut().enumerate() {
                        if (SEGMENT_SETS[digit] & (1 << j)) != 0 {
                            *segment &= encoded;
                        }
                    }
                    // Compare with the others we've already identified
                    for other_digit in 0..10 {
                        if result[other_digit] == 0 || other_digit == digit {
                            continue;
                        }
                        // real segments used here, but not there
                        let extra_segments = SEGMENT_SETS[digit] & (!SEGMENT_SETS[other_digit]);
                        // endoded segments used here, but not there
                        let extra_segments_enc = encoded & (!result[other_digit]);
                        // match them up
                        for j in 0..7 {
                            if (extra_segments & (1 << j)) != 0 {
                                segment_results[j] &= extra_segments_enc;
                            }
                        }
                    }
                }
                0 => {
                    return None;
                }
                _ => {
                    for (i, candidate) in SEGMENT_SETS.iter().enumerate() {
                        if candidate.count_ones() != encoded.count_ones() {
                            // can't be this
                            matches_per_digit[k] &= !(1 << i);
                        } else {
                            // could this still be it?
                            for j in 0..7 {
                                let bitmask = 1 << j;
                                if ((candidate & bitmask) != 0)
                                    && ((segment_results[j] & encoded) == 0)
                                {
                                    // We need this bit, but we can't have it
                                    matches_per_digit[k] &= !(1 << i);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if result.iter().all(|&n| n != 0) {
        // done
        Some(result)
    } else {
        // Getting nowhere
        for k in 0..10 {
            let m = matches_per_digit[k];
            let n_cands = m.count_ones();
            if n_cands > 1 {
                // trial and error
                for i in 0..10 {
                    let bitmask = 1 << i;
                    if (m & bitmask) != 0 {
                        let mut new_matches = matches_per_digit.clone();
                        new_matches[k] = bitmask;
                        if let Some(result) = try_decrypt(digits, &new_matches, &segment_results) {
                            return Some(result);
                        }
                    }
                }
                return None;
            }
        }
        None // contradiction
    }
}

// fn format_map(map: &SegmentMap) -> String {
//     map.iter()
//         .map(|&s| format_segments(s))
//         .collect::<Vec<_>>()
//         .join(" ")
// }

// fn format_segments(segments: u8) -> String {
//     let mut result = String::new();
//     for (i, c) in "abcdefg".chars().enumerate() {
//         let bitmask = 1 << i;
//         if (segments & bitmask) != 0 {
//             result.push(c);
//         }
//     }
//     result
// }

fn decode(num_digits: &[u8], map: &SegmentMap) -> u32 {
    let mut result = 0;
    'outer: for digit in num_digits {
        for (i, code) in map.iter().enumerate() {
            if code == digit {
                result *= 10;
                result += i as u32;
                continue 'outer;
            }
        }
        panic!("invalid map");
    }
    result
}

fn main() {
    println!(
        "{}",
        stdin()
            .lock()
            .lines()
            .filter_map(|l| l.ok())
            .map(|line| {
                let (digits_s, number_s) = line.split_once('|').unwrap();
                decode(
                    &parse_digits(number_s),
                    &decrypt(&parse_digits(digits_s)).unwrap(),
                )
            })
            .sum::<u32>()
    );
}
