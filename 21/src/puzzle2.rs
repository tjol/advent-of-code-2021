use std::io::{stdin, BufRead};

use hashbrown::HashMap;

const WINNING_SCORE: i32 = 21;

#[derive(Debug, Hash, Copy, PartialEq, Eq, Clone)]
struct Player {
    pos: i32,
    score: i32,
}

impl Player {
    fn new(starting_pos: i32) -> Self {
        Self {
            pos: starting_pos,
            score: 0,
        }
    }

    fn roll(&mut self, eyes: i32) {
        self.pos = (self.pos + eyes) % 10;
        self.score += self.pos + 1;
    }
}

#[derive(Debug, Hash, Copy, PartialEq, Eq, Clone)]
struct GameState {
    players: [Player; 2],
}

#[derive(Debug, Copy, PartialEq, Eq, Clone, Hash)]
enum GameOutcome {
    NoWinnerYet,
    PlayerWin(usize),
}

impl GameState {
    fn new(pos1: i32, pos2: i32) -> Self {
        Self {
            players: [Player::new(pos1), Player::new(pos2)],
        }
    }

    fn winner(&self) -> GameOutcome {
        for (i, p) in self.players.iter().enumerate() {
            if p.score >= WINNING_SCORE {
                return GameOutcome::PlayerWin(i);
            }
        }
        GameOutcome::NoWinnerYet
    }
}

fn cast_dice(player: usize, statespace: &HashMap<GameState, usize>) -> HashMap<GameState, usize> {
    let mut result = HashMap::new();
    result.reserve(statespace.len());

    for (game_state, frequency) in statespace {
        match game_state.winner() {
            GameOutcome::NoWinnerYet => {
                for (roll, combinations) in [(3, 1), (4, 3), (5, 6), (6, 7), (7, 6), (8, 3), (9, 1)]
                {
                    let mut new_state = *game_state;
                    new_state.players[player].roll(roll);
                    *result.entry(new_state).or_default() += combinations * frequency;
                }
            }
            _ => {
                *result.entry(*game_state).or_default() += frequency;
            }
        }
    }

    result
}

fn main() {
    let player_positions: Vec<_> = stdin()
        .lock()
        .lines()
        .flatten()
        .map(|l| l.split_once(':').unwrap().1.trim().parse::<i32>().unwrap() - 1)
        .collect();

    let initial_state = GameState::new(player_positions[0], player_positions[1]);
    let mut state_space = HashMap::new();
    state_space.insert(initial_state, 1usize);

    while state_space
        .iter()
        .any(|(g, _)| g.winner() == GameOutcome::NoWinnerYet)
    {
        for player in &[0, 1] {
            state_space = cast_dice(*player, &state_space)
        }
    }

    // Tally up the wins
    let mut win_counts = [0; 2];
    for (game_state, frequency) in state_space {
        match game_state.winner() {
            GameOutcome::PlayerWin(i) => {
                win_counts[i] += frequency;
            }
            GameOutcome::NoWinnerYet => unreachable!(),
        }
    }

    println!("{}", win_counts.iter().max().unwrap());
}
