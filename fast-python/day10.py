close_map = dict(['()', '[]', '{}', '<>'])
part1_pts = {')': 3, ']': 57, '}': 1197, '>': 25137}
part2_pts = {'(': 1, '[': 2, '{': 3, '<': 4}
opening = '([{<'

import statistics

def run(input):
    part1_score = 0
    part2_scores = []
    for line in input.splitlines():
        stack = []
        for c in line:
            if c in opening:
                stack.append(c)
            elif c == close_map[stack.pop()]:
                pass
            else:
                # corrupt
                part1_score += part1_pts[c]
                break
        else:
            # incomplete
            score = 0
            for c in stack[::-1]:
                score *= 5
                score += part2_pts[c]
            part2_scores.append(score)

    return part1_score, statistics.median(part2_scores)
