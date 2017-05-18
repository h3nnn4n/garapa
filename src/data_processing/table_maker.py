#!/usr/bin/env python
# -*- coding: utf-8 -*-

import reader
import os
import numpy
import sys
from math import sqrt

TESTS = ['CMA', 'KBR', 'FBDP', 'NDP', 'HA', 'LELmark', 'ALL']
TEST_FOLDER = 'tests_folder'

if __name__ == '__main__':
    data = {}
    for test in TESTS:
        paths = []
        values = []
        sys.stderr.write('Parsing %s\n' % test)
        path = os.path.join(TEST_FOLDER, test)
        for datafile in os.listdir(path):
            paths.append(os.path.join(path, datafile))

        for p in paths:
            v = reader.main('best1', [p])
            bests = [(lambda x: x[1])(w) for w in v]
            values.append(max(bests))

        data[test] = values

    for k, v in data.items():
        mean = numpy.mean(v)
        std = numpy.std(v)
        best = max(v)
        print("%8s & %8d & %8.2f & %8.2f \\\\ \hline" % (k, best, mean, std))
