use std::io::stdin;
use std::io::BufRead;

#[derive(Debug, PartialEq, Eq)]
enum BingoField {
    NotYet(i32),
    Bingo,
}

type BingoBoard = Vec<Vec<BingoField>>;

fn read_board<I>(lines: &mut I) -> BingoBoard
where
    I: Iterator<Item = String>,
{
    let mut board = BingoBoard::new();
    for _ in 0..5 {
        let line = lines.next().unwrap();
        board.push(
            line.split(' ')
                .filter(|&s| s != "")
                .map(|s| BingoField::NotYet(s.parse().unwrap()))
                .collect(),
        );
        assert_eq!(board[board.len() - 1].len(), 5);
    }
    return board;
}

fn play_turn(board: &mut BingoBoard, num: i32) {
    for line in board {
        for item in line {
            if *item == BingoField::NotYet(num) {
                *item = BingoField::Bingo;
            }
        }
    }
}

fn is_bingo(board: &BingoBoard) -> bool {
    // Check the lines
    board.iter().any(|line| line.iter().all(|f| *f == BingoField::Bingo))
    // Check the columns
    || (0..5).any(|j| (0..5).all(|i| board[i][j] == BingoField::Bingo))
}

fn calc_score(board: &BingoBoard, num: i32) -> i32 {
    board
        .iter()
        .map(|line| {
            line.iter()
                .map(|field| match field {
                    BingoField::NotYet(i) => *i,
                    _ => 0,
                })
                .sum::<i32>()
        })
        .sum::<i32>()
        * num
}

fn main() {
    let stdin = stdin();
    let mut lines_iter = stdin.lock().lines().filter_map(|r| r.ok());
    let draw: Vec<i32> = lines_iter
        .next()
        .unwrap()
        .split(",")
        .map(|s| s.parse().unwrap())
        .collect();
    // parse bingo boards
    let mut boards = Vec::<BingoBoard>::new();

    // skip blank line and read boards
    while lines_iter.next() == Some("".to_owned()) {
        boards.push(read_board(&mut lines_iter));
    }

    // Play the game!
    'game_loop: for num in draw {
        // println!("draw {}", num);
        for board in boards.iter_mut() {
            play_turn(board, num);
            if is_bingo(board) {
                // println!("{:?}", board);
                println!("{}", calc_score(board, num));
                break 'game_loop;
            }
        }
    }
}
