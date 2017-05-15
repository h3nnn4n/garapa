#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

name = sys.argv[1]

ngens = 0
data = []
diversity = []
oldgen = -1
geninfo = []

best_guy = []
avg_guy = []

with open(name) as f:
    lines = f.readlines()
    for k, line in enumerate(lines):
        d = line.split('\n')[0].split(',')

        if d[0] == 'DIVERSITY':
            diver = float(d[1])
            diversity.append(diver)
        else:
            #print(d)
            gen     = int(d[1])
            current = int(d[3])
            runs    = int(d[5])
            ps      = int(d[6].split(':')[1])
            lc      = int(d[8])
            #print(gen, current, runs, ps, lc)
            r = (gen, current, runs, ps, lc)

            if oldgen != gen:
                oldgen = gen
                if gen > 0:
                    data.append(geninfo)
                    geninfo = []
            else:
                geninfo.append(r)


for generation in data:
    best_ps = 0
    best_lc = 0
    avg     = 0
    ss      = 0

    for step in generation:
        gen, current, runs, ps, lc = step
        if runs > 0:
            if lc > best_lc:
                best_lc = lc
                best_ps = ps
            avg += lc
            ss += 1

    avg_guy.append(avg / ss)
    ss = 0
    best_guy.append((best_ps, best_lc))

#for k, v in enumerate(best_guy):
    #print(k, v[1])

#for k, v in enumerate(avg_guy):
    #print(k, v)
