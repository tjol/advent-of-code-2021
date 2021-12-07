use std::io::{stdin, Read};

struct CrabArmy {
    positions: Vec<i32>,
}

impl CrabArmy {
    pub fn new(positions: Vec<i32>) -> Self {
        Self { positions }
    }

    pub fn cost(&self, dest: i32) -> i32 {
        self.positions.iter().map(|pos| (pos - dest).abs()).sum()
    }

    pub fn max(&self) -> i32 {
        *self.positions.iter().max().unwrap()
    }

    pub fn min(&self) -> i32 {
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
