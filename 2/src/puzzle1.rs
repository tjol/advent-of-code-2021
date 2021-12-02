use std::io::stdin;
use std::io::BufRead;

fn main() {
    let mut x = 0;
    let mut y = 0;
    for s in stdin().lock().lines().filter_map(|r| r.ok()) {
        if let Some((cmd, arg)) = s.split_once(' ') {
            let arg: i32 = arg.parse().unwrap();
            match cmd {
                "forward" => {
                    x += arg;
                }
                "down" => {
                    y += arg;
                }
                "up" => {
                    y -= arg;
                }
                _ => {
                    eprintln!("ERROR: invalid command {}", cmd);
                }
            }
        }
    }
    println!("{}", x * y);
}
