#!/usr/bin/env python3

import sys
import timeit
from pathlib import Path

project_root = Path(__file__).resolve().parent.parent
input_files = {int(child.name): input_file
    for child in project_root.iterdir()
    if child.is_dir() and (input_file := child / 'input').is_file()}

inputs = [
    open(fn).read()
    for i in range(25)
    if (fn := input_files.get(i+1)) is not None
    ]

import day01
import day02
import day03
import day04
import day05
import day06
import day07
import day08
import day09
import day10
import day11
import day12

def benchmark_day(day_nr, day_module):
    p1, p2 = day_module.run(inputs[day_nr-1])
    print(f'Day {day_nr} part 1: {p1}')
    print(f'Day {day_nr} part 2: {p2}')
    t0 = timeit.timeit('func(input_str)', number=1,
        globals={'func': day_module.run, 'input_str': inputs[day_nr-1]})
    N = int(1 / t0) + 1
    t = timeit.timeit('func(input_str)', number=N,
        globals={'func': day_module.run, 'input_str': inputs[day_nr-1]})
    t = t * 1000 / N
    print(f'Day {day_nr} time: {t:8.5} ms')

def run_day(day_nr, day_module):
    p1, p2 = day_module.run(inputs[day_nr-1])
    print(f'Day {day_nr} part 1: {p1}')
    print(f'Day {day_nr} part 2: {p2}')

def runall(run_fn):
    run_fn(1, day01)
    run_fn(2, day02)
    run_fn(3, day03)
    run_fn(4, day04)
    run_fn(5, day05)
    run_fn(6, day06)
    run_fn(7, day07)
    run_fn(8, day08)
    run_fn(9, day09)
    run_fn(10, day10)
    run_fn(11, day11)
    run_fn(12, day12)

if __name__ == '__main__':
    if len(sys.argv) > 1 and '-B' == sys.argv[1]:
        runall(run_day)
    else:
        runall(benchmark_day)