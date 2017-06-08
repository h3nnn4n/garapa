#!/usr/bin/env python
# -*- coding: utf-8 -*-

from random import shuffle
import sys


def main(names):
    pieces = ['O', 'I', 'T', 'J', 'L', 'S', 'Z']
    piece_count = {}
    p2 = {}

    for p in pieces:
        piece_count[p] = []
        p2[p] = 0

    for name in names:
        with open(name) as f:
            lines = f.readlines()
            last = None

            for line_number, line in enumerate(lines):
                d = line.split('\n')[0].split(',')
                if 'O: ' in d[0]:
                    data = ' '.join(d[0].replace(':', '').split()).split(' ')
                    if last is None:
                        for k, v in zip(data[::2], data[1::2]):
                            piece_count[k].append(int(v))
                    else:
                        for w, (k, v) in enumerate((zip(data[::2], data[1::2]))):
                            df = int(v) - int(list(zip(last[::2], last[1::2]))[w][1])
                            piece_count[k].append(df)

                    last = data

    l = len(piece_count[pieces[1]])

    for p in pieces:
        shuffle(piece_count[p])

    for i in range(l):
        print(i, end=' ')
        for p in pieces:
            p2[p] += piece_count[p][i]
            print(p2[p], end=' ')

        print()


if __name__ == '__main__':
    names = sys.argv[1:]
    main(names)
