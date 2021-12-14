use rayon::prelude::*;
use std::collections::HashMap;
use std::fmt;
use std::io::{stdin, BufRead};
use std::ops::Deref;
use std::str::FromStr;

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
enum CaveId {
    Start,
    End,
    Small(String),
    Large(String),
}

impl FromStr for CaveId {
    type Err = ();
    fn from_str(s: &str) -> Result<CaveId, ()> {
        match s {
            "start" => Ok(CaveId::Start),
            "end" => Ok(CaveId::End),
            n if n.chars().next().unwrap().is_lowercase() => Ok(CaveId::Small(n.to_owned())),
            n => Ok(CaveId::Large(n.to_owned())),
        }
    }
}

impl fmt::Display for CaveId {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> Result<(), fmt::Error> {
        match self {
            CaveId::Start => "start".fmt(f),
            CaveId::End => "end".fmt(f),
            CaveId::Small(n) | CaveId::Large(n) => n.fmt(f),
        }
    }
}

#[derive(Debug, Clone)]
struct Cave {
    id: CaveId,
    connected: Vec<usize>,
}

impl Cave {
    pub fn new(id: CaveId) -> Self {
        Self {
            id,
            connected: vec![],
        }
    }
}

#[derive(Debug, Clone)]
struct CaveMap {
    caves: Vec<Cave>,
    start_idx: usize,
    end_idx: usize,
}

impl CaveMap {
    pub fn from_lines<I, S>(lines: I) -> Self
    where
        I: IntoIterator<Item = S>,
        S: Deref<Target = str>,
    {
        let mut caves = vec![];
        let mut cave_indices = HashMap::new();
        let mut start_idx = None;
        let mut end_idx = None;
        for l in lines {
            let (from, to) = l.split_once('-').unwrap();
            let from_id: CaveId = from.parse().unwrap();
            let to_id: CaveId = to.parse().unwrap();
            let from_idx = cave_indices.get(&from_id).copied().unwrap_or_else(|| {
                let idx = caves.len();
                caves.push(Cave::new(from_id.clone()));
                cave_indices.insert(from_id.clone(), idx);
                if from_id == CaveId::Start {
                    start_idx = Some(idx);
                }
                idx
            });
            let to_idx = cave_indices.get(&to_id).copied().unwrap_or_else(|| {
                let idx = caves.len();
                caves.push(Cave::new(to_id.clone()));
                cave_indices.insert(to_id.clone(), idx);
                if to_id == CaveId::End {
                    end_idx = Some(idx);
                }
                idx
            });
            caves[from_idx].connected.push(to_idx);
            caves[to_idx].connected.push(from_idx);
        }
        Self {
            caves,
            start_idx: start_idx.expect("there must be a start"),
            end_idx: end_idx.expect("there must be an end"),
        }
    }

    pub fn find_all_paths(&self) -> usize {
        let start_idx = self.start_idx;
        self.find_all_paths_from(start_idx, vec![], false)
    }

    fn find_all_paths_from(
        &self,
        idx: usize,
        mut visited_small: Vec<usize>,
        mut used_joker: bool,
    ) -> usize {
        // Is this the end?
        if idx == self.end_idx {
            return 1;
        }

        let cave = &self.caves[idx];

        if matches!(cave.id, CaveId::Small(_)) {
            if visited_small.contains(&idx) {
                if used_joker {
                    return 0;
                } else {
                    used_joker = true;
                }
            } else {
                visited_small.push(idx);
            }
        }

        cave.connected
            .par_iter()
            .filter(|&i| self.caves[*i].id != CaveId::Start)
            .map(|&i| self.find_all_paths_from(i, visited_small.clone(), used_joker))
            .sum()
    }
}

fn main() {
    let cave_map = CaveMap::from_lines(stdin().lock().lines().filter_map(|r| r.ok()));
    let paths = cave_map.find_all_paths();
    println!("{}", paths);
}
