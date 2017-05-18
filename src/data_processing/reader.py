#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy
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
                gen     = int(d[1])
                current = int(d[3])
                runs    = int(d[5])
                ps      = int(d[6].split(':')[1])
                lc      = int(d[8])
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

    elif mode == 'best1':
        best_guy, _ = pack_data(names[0])
        return enumerate([(lambda x: x[1])(i) for i in best_guy])

    elif mode == 'avg_all':
        means_data = [(lambda x: x[0])(pack_data(name)) for name in names]
        minlen = min([(lambda x: len(x))(values) for values in means_data])
        maxlen = max([(lambda x: len(x))(values) for values in means_data])

        means = [ 0 for _ in range(0, maxlen)]
        ns    = [ 0 for _ in range(0, maxlen)]
        for i in range(0, len(means_data)):
            for j in range(0, len(means_data[i])):
                means[j] += means_data[i][j][1]
                ns[j] += 1

        for j in range(0, maxlen):
            means[j] /= ns[j]

        return enumerate(means)

    elif mode == 'avg':
        means_data = [(lambda x: x[0])(pack_data(name)) for name in names]
        minlen = min([(lambda x: len(x))(values) for values in means_data])

        means = []
        for j in range(0, minlen):
            avg = 0
            for i in range(0, len(means_data)):
                avg += means_data[i][j][1]
            means.append(avg / len(means_data))

        return enumerate(means)

    elif mode == 'best_all':
        means_data = [(lambda x: x[0])(pack_data(name)) for name in names]
        minlen = min([(lambda x: len(x))(values) for values in means_data])
        maxlen = max([(lambda x: len(x))(values) for values in means_data])

        best = [ 0 for _ in range(0, maxlen)]
        for i in range(0, len(means_data)):
            for j in range(0, len(means_data[i])):
                best[j] = max(best[j], means_data[i][j][1])

        return enumerate(best)

    elif mode == 'best':
        means_data = [(lambda x: x[0])(pack_data(name)) for name in names]
        minlen = min([(lambda x: len(x))(values) for values in means_data])

        best = []
        for j in range(0, minlen):
            m = 0
            for i in range(0, len(means_data)):
                m = max(m, means_data[i][j][1])
            best.append(m)

        return enumerate(best)

    elif mode == 'avg_scaled':
        means_data = [(lambda x: x[0])(pack_data(name)) for name in names]
        minlen = min([(lambda x: len(x))(values) for values in means_data])
        maxlen = max([(lambda x: len(x))(values) for values in means_data])

        data = [[-1 for j in range(maxlen)] for i in range(len(means_data))]

        means = [ 0 for _ in range(0, maxlen)]
        ns    = [ 0 for _ in range(0, maxlen)]
        for i in range(0, len(means_data)):
            for j in range(0, len(means_data[i])):
                x, y = ((j+1) * maxlen/(len(means_data[i]))) - 1, means_data[i][j][1]
                data[i][int(round(x))] = y

        for d in data:
            if d[0] == -1:
                i = 0
                while d[i] == -1:
                    i += 1

                d[0] = d[i]
                d[i] = 0

            #for v in d:
                #print("%4d" % v,end=' ')
            #print()

            for pos, value in enumerate(d):
                if value == -1:
                    p1 = pos
                    p2 = pos

                    while d[p1] == -1:
                        p1 -= 1

                    while d[p2] == -1:
                        p2 += 1

                    v1 = [p1, p2]
                    v2 = [d[p1], d[p2]]
                    a, b = numpy.polyfit(v1, v2, 1)
                    d[pos] = int(round(a * pos + b))

                means[pos] += d[pos]
                ns[pos] += 1

        for j in range(0, maxlen):
            means[j] /= ns[j]
            #print("%4d" % means[j],end=' ')
        #print()

        #exit()

        return enumerate(means)


if __name__ == '__main__':
    mode = sys.argv[1]
    names = sys.argv[2:]
    for k, v in main(mode, names):
        print(k, v)
