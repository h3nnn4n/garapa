#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

def pack_data(name):
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

    return best_guy, avg_guy


def main(mode, names):

    if mode == 'avg1':
        _, avg_guy = pack_data(names[0])
        return enumerate(avg_guy)
        #for k, v in enumerate(avg_guy):
            #print(k, v)

    elif mode == 'best1':
        best_guy, _ = pack_data(names[0])
        return enumerate([(lambda x: x[1])(i) for i in best_guy])
        #for k, v in enumerate(best_guy):
            #print(k, v[1])

    elif mode == 'avg':
        means_data = [(lambda x: x[0])(pack_data(name)) for name in names]
        minlen = min([(lambda x: len(x))(values) for values in means_data])
        means = []
        for j in range(0, minlen):
            avg = 0
            for i in range(0, len(means_data)):
                avg += means_data[i][j][1]
            means.append(avg / len(means_data))

        #for k, v in enumerate(means):
            #print(k, v)
        return enumerate(means)

    elif mode == 'best':
        means_data = [(lambda x: x[0])(pack_data(name)) for name in names]
        minlen = min([(lambda x: len(x))(values) for values in means_data])
        best = []
        for j in range(0, minlen):
            m = 0
            for i in range(0, len(means_data)):
                m = max(m, means_data[i][j][1])
            best.append(m)

        #for k, v in enumerate(means):
            #print(k, v)
        return enumerate(best)


if __name__ == '__main__':
    mode = sys.argv[1]
    names = sys.argv[2:]
    for k, v in main(mode, names):
        print(k, v)
