#!/usr/bin/env python
# -*- coding: utf-8 -*-

import reader
import os

TESTS = ['CMA', 'KBR', 'FBDP', 'NDP', 'HA', 'LELmark']
TEST_FOLDER = 'tests_folder'

if __name__ == '__main__':
    for test in TESTS:
        paths = []
        print('Parsing %s' % test)
        path = os.path.join(TEST_FOLDER, test)
        for datafile in os.listdir(path):
            paths.append(os.path.join(path, datafile))

        output = "log_" + test + "_avg.txt"
        with open(output, 'wt') as outfile:
            print('Writing to %s' % output)
            for k, v in reader.main('avg', paths):
                outfile.write(("%d %f\n" % (k, v)))

        output = "log_" + test + "_best.txt"
        with open(output, 'wt') as outfile:
            print('Writing to %s' % output)
            for k, v in reader.main('best', paths):
                outfile.write(("%d %d\n" % (k, v)))
