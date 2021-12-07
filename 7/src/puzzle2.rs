use std::io::{stdin, Read};

fn cost(d: i64) -> i64 {
    // from https://oeis.org/A000217
    let d_plus_1 = d + 1;
    (d_plus_1 * d_plus_1 * d_plus_1 - d * d * d - 1) / 6
}

struct CrabArmy {
    positions: Vec<i64>,
}

impl CrabArmy {
    pub fn new(positions: Vec<i64>) -> Self {
        Self { positions }
    }

    pub fn cost(&self, dest: i64) -> i64 {
        self.positions
            .iter()
            .map(|pos| cost((pos - dest).abs()))
            .sum()
    }

    pub fn max(&self) -> i64 {
        *self.positions.iter().max().unwrap()
    }

    pub fn min(&self) -> i64 {
        *self.positions.iter().min().unwrap()
    }
}

fn main() {
    let mut s = String::new();
    stdin().lock().read_to_string(&mut s).unwrap();
    let battalion = CrabArmy::new(s.split(',').map(|w| w.trim().parse().unwrap()).collect());

    let min_cost = (battalion.min()..=battalion.max())
        .map(|dest| battalion.cost(dest))
        .min()
        .unwrap();

    println!("{}", min_cost)
}
