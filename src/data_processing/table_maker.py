#!/usr/bin/env python
# -*- coding: utf-8 -*-

import reader
import os
import numpy
import sys
from math import sqrt
import scipy
from scipy import stats

TESTS = ['CMA', 'KBR', 'FBDP', 'NDP', 'HA', 'LELmark', 'ALL']
#TESTS = ['HA', 'LELmark', 'ALL']
TEST_FOLDER = 'tests_folder'
MAIN = 'LELmark'

if __name__ == '__main__':
    data = {}
    best = {}
    for test in TESTS:
        best[test] = []
        paths = []
        values = []
        sys.stderr.write('Parsing %s\n' % test)
        path = os.path.join(TEST_FOLDER, test)
        for datafile in os.listdir(path):
            paths.append(os.path.join(path, datafile))
            print(os.path.join(path, datafile))

        for p in paths:
            v = reader.main('best1', [p])
            bests = [(lambda x: x[1])(w) for w in v]
            bests.append(0)
            values.append(max(bests))

            b = reader.main('most_cleared_lines1', [p])[0][1]
            best[test].append(b)

        data[test] = values

# This is for safety, as the tests are still running
# if the sample size is different it is padded u
    minlen = 9999999
    maxlen = 0
    for k, v in best.items():
        minlen = min(minlen, len(v))
        maxlen = max(maxlen, len(v))
        #print(k, v)

    #print(minlen, maxlen)

    for k, v in best.items():
        if len(v) < maxlen:
            for i in range(maxlen - len(v)):
                best[k].append(numpy.mean(v))

    for k, v in best.items():
        #minlen = min(minlen, len(v))
        #maxlen = max(maxlen, len(v))
        #print(k, v)
        print("%8s" % k, end=' ')
        for i in v:
            print("%6d" % i, end=' ')
        print()

    print()
#End of safety measures

    for k, v in best.items():
        if k != 'LELmark':
            print("%8s x %8s: " % (MAIN, k), end = ' ')
            print("%8d %16.8f" % scipy.stats.wilcoxon(best[MAIN], v))

    print()

    for k, v in data.items():
        mean = numpy.mean(v)
        std = numpy.std(v)
        mcl = max(v)
        wilcox = 0
        comp = ""

        if k != MAIN:
            #vv = [best[MAIN][i] - best[k][i] for i in range(0, maxlen)]
            #vv = [best[k][i] - best[MAIN][i] for i in range(0, maxlen)]
            wilcox = scipy.stats.wilcoxon(best[MAIN], best[k])[1]
            #wilcox = scipy.stats.wilcoxon(vv)[1]

        if wilcox > 0.05:
            comp = "$\\approx$"
        elif wilcox == 0.0:
            comp = "N/A"
        else:
            if numpy.mean(best[MAIN]) > numpy.mean(best[k]):
                comp = "+"
            else:
                comp = "-"

        print("%8s & %8d & %8.2f $\pm$ %8.2f & %16.8f & %8s \\\\ \hline" % (k, mcl, mean, std, wilcox, comp))

