#!/usr/bin/env python
# -*- coding: utf-8 -*-

import reader
import os

#TESTS = ['CMA', 'KBR', 'FBDP', 'NDP', 'HA', 'LELmark', 'ALL']
TESTS = ['CMA', 'HA', 'LELmark', 'ALL']
TEST_FOLDER = 'tests_folder'

if __name__ == '__main__':
    maxlen = 0
    print('Preprocessing data')
    for test in TESTS:
        path = os.path.join(TEST_FOLDER, test)
        for datafile in os.listdir(path):
            maxlen = max(maxlen, reader.get_maxlen(os.path.join(path, datafile)))

    for test in TESTS:
        paths = []
        print('Parsing %s' % test)
        path = os.path.join(TEST_FOLDER, test)
        for datafile in os.listdir(path):
            print(os.path.join(path, datafile))
            paths.append(os.path.join(path, datafile))

        output = "log_" + test + "_avg.txt"
        with open(output, 'wt') as outfile:
            print('Writing to %s' % output)
            for k, v in reader.main('avg_scaled', paths, maxlen):
                outfile.write(("%d %f\n" % (k, v)))

        output = "log_" + test + "_best.txt"
        with open(output, 'wt') as outfile:
            print('Writing to %s' % output)
            for k, v in reader.main('best', paths):
                outfile.write(("%d %d\n" % (k, v)))
