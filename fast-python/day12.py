from collections import defaultdict

def count_paths(g, node, visited_small, used_joker=True):
    if node == 'end':
        return 1
    elif node[0].islower():
        if node in visited_small:
            if used_joker or node == 'start':
                return 0
            else:
                used_joker = True
        else:
            visited_small = visited_small | {node}
    return sum(count_paths(g, next_node, visited_small, used_joker) for next_node in g[node])


def run(input):
    graph = defaultdict(set)
    for line in input.splitlines():
        a, b = line.split('-')
        graph[a].add(b)
        graph[b].add(a)

    return (count_paths(graph, 'start', set(), True), count_paths(graph, 'start', set(), False))
