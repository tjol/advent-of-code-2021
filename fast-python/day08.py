def make_map(patterns):
    for p in patterns:
        l = len(p)
        if l == 2:
            one = p
        elif l == 3:
            seven = p
        elif l == 7:
            eight = p
        elif l == 4:
            four = p
    
    fours_corner = four - one

    for p in patterns:
        l = len(p)
        if l == 5:
            if fours_corner < p:
                five = p
            elif one < p:
                three = p
            else:
                two = p
        elif l == 6:
            if one < p:
                if fours_corner < p:
                    nine = p
                else:
                    zero = p
            else:
                six = p
    
    return {zero: 0, one: 1, two: 2, three: 3, four: 4, five: 5, six: 6, seven: 7, eight: 8, nine: 9}

def run(input):
    easy_digit_count = 0
    sum_of_results = 0
    for line in input.splitlines():
        patterns_str, digits_str = line.split(' | ')
        patterns = [frozenset(p) for p in patterns_str.strip().split()]
        digits_str = [frozenset(d) for d in digits_str.strip().split()]
        pattern_map = make_map(patterns)
        r = 0
        for d in digits_str:
            n = pattern_map[d]
            if n in [1,4,7,8]:
                easy_digit_count += 1
            r *= 10
            r += n
        sum_of_results += r
    return easy_digit_count, sum_of_results

