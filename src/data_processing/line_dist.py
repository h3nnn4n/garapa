#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys


def pack_data(names):
    line_dist = [0 for _ in range(5000)]

    max_clear = 0

    for name in names:
        with open(name) as f:
            lines = f.readlines()
            for k, line in enumerate(lines):
                d = line.split('\n')[0].split(',')

                if d[0] == 'DIVERSITY':
                    pass
                elif 'double' in d[0]:
                    pass
                elif 'O:' in d[0]:
                    pass
                elif ':' not in line and ',' not in line:
                    pass
                else:
                    runs = int(d[5])
                    lc = int(d[8])

                    if runs > 0:
                        line_dist[lc] += 1
                        max_clear = max(max_clear, lc)

    for n in range(max_clear + 5):
        print(n, line_dist[n])


if __name__ == '__main__':
    names = sys.argv[1:]
    pack_data(names)
