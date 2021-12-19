use std::cmp::Ordering;
use std::io::{stdin, BufRead};

use hashbrown::{HashMap, HashSet};
use itertools::Itertools;
use nalgebra::{point, Matrix4, Point3, Transform3, Vector3};

type Vec3f = Vector3<f32>;
type Pt3f = Point3<f32>;
const MIN_EQUAL_DIST: usize = 10;

#[derive(Debug, Clone)]
struct ScannerPoints {
    points: Vec<Pt3f>,
    sq_distance_map: Vec<HashMap<u32, Vec<usize>>>,
    uniq_dist_lists: Vec<Vec<u32>>,
}

impl ScannerPoints {
    pub fn new(points: Vec<Pt3f>) -> Self {
        // calculate the distances to the other points, for every point.
        let mut sq_distance_map = vec![];
        sq_distance_map.resize_with(points.len(), HashMap::new);

        for i in 0..points.len() {
            for j in (i + 1)..points.len() {
                let p = &points[i];
                let q = &points[j];
                let sq_dist = (p - q).norm_squared() as u32;
                sq_distance_map[i]
                    .entry(sq_dist)
                    .or_insert(Vec::new())
                    .push(j);
                sq_distance_map[j]
                    .entry(sq_dist)
                    .or_insert(Vec::new())
                    .push(i);
            }
        }

        let uniq_dist_lists = sq_distance_map
            .iter()
            .map(|m| {
                m.iter()
                    .filter_map(|(k, v)| if v.len() == 1 { Some(k) } else { None })
                    .copied()
                    .sorted()
                    .collect()
            })
            .collect();

        Self {
            points,
            sq_distance_map,
            uniq_dist_lists,
        }
    }

    pub fn match_with(&self, other: &ScannerPoints) -> Vec<(usize, usize)> {
        let mut matches = vec![];
        for (i, my_dists) in self.uniq_dist_lists.iter().enumerate() {
            if matches.iter().filter(|(a, _)| *a == i).next().is_some() {
                break;
            }
            for (j, their_dists) in other.uniq_dist_lists.iter().enumerate() {
                if matches.iter().filter(|(_, b)| *b == j).next().is_some() {
                    continue;
                }
                let mut k = 0;
                let mut l = 0;
                let mut matched_dists: Vec<(usize, usize)> = vec![];
                while k < my_dists.len() && l < their_dists.len() {
                    match my_dists[k].cmp(&their_dists[l]) {
                        Ordering::Equal => {
                            matched_dists.push((
                                self.sq_distance_map[i][&my_dists[k]][0],
                                other.sq_distance_map[j][&their_dists[l]][0],
                            ));
                            k += 1;
                            l += 1;
                        }
                        Ordering::Less => {
                            k += 1;
                        }
                        Ordering::Greater => {
                            l += 1;
                        }
                    }
                }
                if matched_dists.len() >= MIN_EQUAL_DIST {
                    matches.push((i, j));
                    matches.extend_from_slice(&matched_dists);
                    return matches;
                }
            }
        }

        matches
    }

    pub fn get_transformation_onto(&self, other: &ScannerPoints) -> Option<Transform3<f32>> {
        let matches = self.match_with(other);

        if matches.len() < 4 {
            return None;
        }

        // get the matched points
        let our_points = matches
            .iter()
            .map(|(i, _)| self.points[*i])
            .collect::<Vec<_>>();
        let their_points = matches
            .iter()
            .map(|(_, j)| other.points[*j])
            .collect::<Vec<_>>();

        // find the matrix that maps our_points onto their_points
        let their_mat = Matrix4::from_columns(
            &their_points[..4]
                .iter()
                .map(|p| p.to_homogeneous())
                .collect::<Vec<_>>(),
        );
        let mut our_mat = Matrix4::from_columns(
            &our_points[..4]
                .iter()
                .map(|p| p.to_homogeneous())
                .collect::<Vec<_>>(),
        );
        if !our_mat.try_inverse_mut() {
            panic!("Inversion failed");
        }

        let transformation_mat = their_mat * our_mat;

        Some(Transform3::from_matrix_unchecked(transformation_mat))
    }
}

fn main() {
    let mut scanner_points = vec![];
    let mut current_scanner_points = vec![];
    for line in stdin().lock().lines().flatten() {
        if line.starts_with("---") {
            if !current_scanner_points.is_empty() {
                scanner_points.push(ScannerPoints::new(current_scanner_points));
                current_scanner_points = vec![];
            }
        } else if line.trim().is_empty() {
            // ignore empty lines
        } else {
            current_scanner_points.push(Pt3f::from(Vec3f::from_iterator(
                line.split(',').flat_map(|s| s.parse()),
            )));
        }
    }
    scanner_points.push(ScannerPoints::new(current_scanner_points));

    let mut transformations = HashMap::new();
    transformations.insert(0usize, Transform3::identity());
    let mut unsolved_indices: Vec<usize> = (1..scanner_points.len()).collect();
    let mut impossible: HashSet<(usize, usize)> = HashSet::new();

    'solve_loop: while !unsolved_indices.is_empty() {
        for (meta_i, i) in unsolved_indices.clone().into_iter().enumerate() {
            // Try to match with each solved view
            let known_indices = transformations.keys().collect::<Vec<_>>();
            for j in known_indices {
                if impossible.contains(&(i, *j)) {
                    continue;
                }
                if let Some(trans) = scanner_points[i].get_transformation_onto(&scanner_points[*j])
                {
                    // solved it!
                    let their_trans = transformations[j];
                    transformations.insert(i, their_trans * trans);
                    unsolved_indices.swap_remove(meta_i);
                    continue 'solve_loop;
                } else {
                    impossible.insert((i, *j));
                }
            }
        }
    }

    let scanner_locations = (0..scanner_points.len())
        .map(|i| {
            let origin = transformations[&i] * point![0., 0., 0.];
            Point3::from(Vector3::<i32>::from_iterator(
                origin.coords.iter().map(|x| x.round() as i32),
            ))
        })
        .collect::<Vec<_>>();

    let mut max_dist = 0;
    for (i, loc1) in scanner_locations.iter().enumerate() {
        for loc2 in &scanner_locations[i + 1..] {
            let dist = (loc2 - loc1).abs().sum();
            if dist > max_dist {
                max_dist = dist;
            }
        }
    }
    println!("{}", max_dist);

    // let mut beacons = HashSet::new();

    // for i in 0..scanner_points.len() {
    //     let trans = transformations[&i];
    //     for q in scanner_points[i].points.iter().map(|p| trans * p) {
    //         let q_int = Point3::from(Vector3::<i32>::from_iterator(
    //             q.coords.iter().map(|x| x.round() as i32),
    //         ));
    //         beacons.insert(q_int);
    //     }
    // }

    // // println!("beacons: {:?}", beacons);
    // println!("{}", beacons.len());
}
