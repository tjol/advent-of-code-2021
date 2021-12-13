import sys

def run(input):
    point_list, instructions = input.split('\n\n')

    points = set(tuple(map(int, p_s.strip().split(',')))
        for p_s in point_list.splitlines())

    for idx, instruction in enumerate(instructions.splitlines()):
        command, argument = instruction.split('=')
        boundary = int(argument)
        axis = command[-1]
        if axis == 'x':
            points = set((2 * boundary - x if x > boundary else x, y) for (x, y) in points)
        else:
            points = set((x, 2 * boundary - y if y > boundary else y) for (x, y) in points)
        
        if idx == 0:
            count1 = len(points)

    max_x = 0
    max_y = 0
    for (x, y) in points:
        max_x = max(x, max_x)
        max_y = max(y, max_y)
    lines = [[' '] * (max_x + 1) for i in range(max_y + 1)]

    for (x, y) in points:
        lines[y][x] = '#'

    picture = '\n'.join(''.join(line) for line in lines)

    return count1, picture

