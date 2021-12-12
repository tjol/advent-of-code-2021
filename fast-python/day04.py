import numpy as np

def calc_score(board, marked, num):
    return board[~marked.astype('bool')].sum() * num

def run(input):
    line1, rest = input.split('\n', 1)
    nums = np.fromstring(line1, dtype='i2', sep=',')
    boards = np.fromstring(rest, dtype='i2', sep=' ').reshape(100, 5, 5)
    marked = np.zeros_like(boards)
    
    winning_score = None
    remaining = len(boards)
    squid_score = None

    for n in nums:
        eliminate = []

        marked[boards == n] = 1
        # any winners?
        # check rows
        row_winners = np.argwhere(marked.sum(axis=2) == 5)
        col_winners = np.argwhere(marked.sum(axis=1) == 5)
        winners = np.concatenate([row_winners, col_winners])
        for winner in winners:
            winning_board = winner[0]   
            if winning_score is None:
                winning_score = calc_score(boards[winning_board], marked[winning_board], n)
            elif len(boards) == 1:
                squid_score = calc_score(boards[winning_board], marked[winning_board], n)
                break
            eliminate.append(winning_board)
        if squid_score is not None:
            break

        if eliminate:
            boards = np.delete(boards, eliminate, axis=0)
            marked = np.delete(marked, eliminate, axis=0)

    return winning_score, squid_score
