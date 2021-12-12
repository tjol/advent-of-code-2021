def run(input):
    y1 = 0
    x1 = 0
    y2 = 0
    x2 = 0
    a2 = 0
    for line in filter(None, input.splitlines()):
        v = int(line[-1])
        if line[0] == 'f':
            x1 += v
            x2 += v
            y2 += a2*v
        elif line[0] == 'd':
            y1 += v
            a2 += v
        elif line[0] == 'u':
            y1 -= v
            a2 -= v
    return x1*y1, x2*y2
