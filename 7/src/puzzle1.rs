use std::io::{stdin, Read};

struct CrabArmy {
    positions: Vec<i64>,
}

impl CrabArmy {
    pub fn new(positions: Vec<i64>) -> Self {
        Self { positions }
    }

    pub fn cost(&self, dest: i64) -> i64 {
        self.positions.iter().map(|pos| (pos - dest).abs()).sum()
    }

    pub fn max(&self) -> i64 {
        *self.positions.iter().max().unwrap()
    }

    pub fn min(&self) -> i64 {
        *self.positions.iter().min().unwrap()
    }
}

fn find_minimum<F: FnMut(i64) -> i64>(mut f: F, mut begin: i64, mut end: i64) -> (i64, i64) {
    let mut mid = (end - begin) / 2;
    let mut step = mid - begin;
    let mut y0 = f(begin);
    let mut y1 = f(mid);
    let mut y2 = f(end);
    loop {
        if y1 <= y0 && y1 <= y2 {
            // termination condition
            if step <= 1 {
                return (mid, y1);
            }
            step /= 2;
            begin = mid - step;
            end = mid + step;
            y0 = f(begin);
            y2 = f(end);
        } else if y0 <= y1 {
            end = mid;
            y2 = y1;
            mid = begin;
            y1 = y0;
            begin -= step;
            y0 = f(begin);
        } else if y2 <= y1 {
            begin = mid;
            y0 = y1;
            mid = end;
            y1 = y2;
            end += step;
            y2 = f(end);
        }
    }
}

fn main() {
    let mut s = String::new();
    stdin().lock().read_to_string(&mut s).unwrap();
    let battalion = CrabArmy::new(s.split(',').map(|w| w.trim().parse().unwrap()).collect());

    let (_, min_cost) = find_minimum(|d| battalion.cost(d), battalion.min(), battalion.max());

    // let min_cost = (battalion.min()..=battalion.max())
    //     .map(|dest| battalion.cost(dest))
    //     .min()
    //     .unwrap();

    println!("{}", min_cost)
}
