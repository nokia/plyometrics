#!/usr/bin/env python3

import operator
import json
import sys
from os import path
from argparse import ArgumentParser
from collections import defaultdict
import matplotlib.pyplot as plt


def trim(s, max_len=100):
    if len(s) > max_len:
        return s[:max_len] + '...'
    return s


def sorted_by_key(dic):
    return sorted(dic, key=operator.itemgetter(0))


def visualize(grouped_data, output_directory=None, overwrite=False):
    plt.style.use('ggplot')
    plt.rcParams["figure.figsize"] = (10,8)

    for name, data in grouped_data.items():
        plt.clf()
        plt.title(name)
        for type_name, results in sorted_by_key(data.items()):
            x = [sample['number'] for sample in results]
            y = [sample['time'] for sample in results]
            plt.plot(x, y, label=trim(type_name), marker='o')
        plt.xlabel('number')
        plt.ylabel('time')
        plt.legend()

        if output_directory is None:
            plt.show()
        else:
            filename = path.join(output_directory, name + '.png')
            if overwrite or not path.exists(filename):
                plt.savefig(filename)
            else:
                print('{} already exists, use -f to force overwrite'.format(filename))


def group(data):
    benchmarks = defaultdict(lambda: defaultdict(list))
    for b in data:
        benchmarks[b["name"]][b["type"]].append(b)
    return benchmarks


def main():
    parser = ArgumentParser(description='Draws chars out of benchmark data')
    parser.add_argument('-o', dest='output_directory', help='directory where chart images will be dumped')
    parser.add_argument('-f', action='store_true', dest='overwrite', help='force files to be overwritten')
    args = parser.parse_args()

    try:
        data = json.load(sys.stdin)
        visualize(group(data), output_directory=args.output_directory, overwrite=args.overwrite)
    except json.decoder.JSONDecodeError as e:
        print(e)
        print("Did you forgot -x switch?")


if __name__ == "__main__":
    main()
