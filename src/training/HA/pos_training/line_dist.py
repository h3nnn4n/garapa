#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy
import sys

INTERPOLATE_DATA = False

def pack_data(names):
    data = []
    diversity = []
    oldgen = -1
    geninfo = []

    line_dist = [0 for _ in range(5000)]

    max_clear = 0

    for name in names:
        with open(name) as f:
            lines = f.readlines()
            for k, line in enumerate(lines):
                d = line.split('\n')[0].split(',')

                if d[0] == 'DIVERSITY':
                    diver = float(d[1])
                    diversity.append(diver)
                elif 'double' in d[0]:
                    pass
                elif 'O:' in d[0]:
                    pass
                elif ':' not in line and ',' not in line:
                    pass
                else:
                    gen = int(d[1])
                    current = int(d[3])
                    runs = int(d[5])
                    ps = int(d[6].split(':')[1])
                    lc = int(d[8])
                    r = (gen, current, runs, ps, lc)

                    if oldgen != gen:
                        oldgen = gen
                        if gen > 0:
                            data.append(geninfo)
                            geninfo = []
                    else:
                        geninfo.append(r)

        for generation in data:
            for step in generation:
                gen, current, runs, ps, lc = step
                if runs > 0:
                    line_dist[lc] += 1
                    max_clear = max(max_clear, lc)

    for n in range(max_clear + 5):
        print(n, line_dist[n])


if __name__ == '__main__':
    names = sys.argv[1:]
    pack_data(names)
