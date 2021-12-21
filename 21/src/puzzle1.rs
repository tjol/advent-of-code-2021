use std::io::{stdin, BufRead};

#[derive(Debug)]
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

fn main() {
    let mut players = stdin()
        .lock()
        .lines()
        .flatten()
        .map(|l| Player::new(l.split_once(':').unwrap().1.trim().parse::<i32>().unwrap() - 1))
        .collect::<Vec<_>>();

    let mut throws = 0;
    let winner;
    'game_loop: loop {
        for (i, player) in players.iter_mut().enumerate() {
            let eyes = 3 * throws + 6;
            throws += 3;
            player.roll(eyes);
            if player.score >= 1000 {
                winner = Some(i);
                break 'game_loop;
            }
        }
    }
    let losing_score = match winner {
        Some(0) => players[1].score,
        Some(1) => players[0].score,
        _ => unreachable!(),
    };
    println!("{}", losing_score * throws);
}
