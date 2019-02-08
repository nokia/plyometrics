#!/usr/bin/env python3

import operator
import json
import sys
from collections import defaultdict
import matplotlib.pyplot as plt


def trim(s, max_len=100):
    if len(s) > max_len:
        return s[:max_len] + '...'
    return s


def sorted_by_key(dic):
    return sorted(dic, key=operator.itemgetter(0))


def visualize(grouped_data):
    plt.style.use('ggplot')
    plt.rcParams["figure.figsize"] = (10,8)
    for name, data in grouped_data.items():
        plt.title(name)
        for type_name, results in sorted_by_key(data.items()):
            x = [sample['number'] for sample in results]
            y = [sample['time'] for sample in results]
            plt.plot(x, y, label=trim(type_name), marker='o')
        plt.xlabel('number')
        plt.ylabel('time')
        plt.legend()
        plt.show()


def group(data):
    benchmarks = defaultdict(lambda: defaultdict(list))
    for b in data:
        benchmarks[b["name"]][b["type"]].append(b)
    return benchmarks


def main():
    try:
        data = json.load(sys.stdin)
        visualize(group(data))
    except json.decoder.JSONDecodeError as e:
        print(e)
        print("Did you forgot -x switch?")


if __name__ == "__main__":
    main()
