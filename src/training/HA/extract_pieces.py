#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys


def main(names):
    pieces = ['O', 'I', 'T', 'J', 'L', 'S', 'Z']
    piece_count = {}

    for p in pieces:
        piece_count[p] = []

    first = True
    for name in names:
        with open(name) as f:
            lines = f.readlines()

            for line_number, line in enumerate(lines):
                d = line.split('\n')[0].split(',')
                if 'O: ' in d[0]:
                    data = ' '.join(d[0].replace(':', '').split()).split(' ')
                    if first:
                        first = False
                        for k, v in zip(data[::2], data[1::2]):
                            piece_count[k].append(int(v))
                    else:
                        for k, v in zip(data[::2], data[1::2]):
                            df = int(v) - piece_count[k][-1]
                            if int(v) < piece_count[k][-1]:
                                piece_count[k].append(int(v))
                            else:
                                #if df < 0:
                                    #df = -df
                                piece_count[k].append(df)

    #for k, v in piece_count.items():
        #print(k, v)

    l = len(piece_count[pieces[1]])

    for i in range(l):
        print(i, end=' ')
        for p in pieces:
            print(piece_count[p][i], end=' ')

        print()


if __name__ == '__main__':
    names = sys.argv[1:]
    main(names)
