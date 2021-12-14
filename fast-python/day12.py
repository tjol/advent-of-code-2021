from collections import defaultdict
import numpy as np

def count_paths(g, node, visited_small, used_joker=True):
    using_joker_now = False
    if node == 'end':
        return (1, 1)
    elif node[0].islower():
        if node in visited_small:
            if used_joker or node == 'start':
                return (0, 0)
            else:
                using_joker_now = True
                used_joker = True
        else:
            visited_small = visited_small | {node}
    sum1, sum2 = 0, 0
    for next_node in g[node]:
        r1, r2 = count_paths(g, next_node, visited_small, used_joker)
        sum1 += r1
        sum2 += r2
    if using_joker_now:
        return (0, sum2)
    else:
        return (sum1, sum2)


def run(input):
    graph = defaultdict(set)
    for line in input.splitlines():
        a, b = line.split('-')
        graph[a].add(b)
        graph[b].add(a)

    return count_paths(graph, 'start', set(), False)
