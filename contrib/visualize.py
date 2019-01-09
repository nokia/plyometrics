#!/usr/bin/env python3

import json
import sys
from collections import defaultdict
import matplotlib.pyplot as plt


def visualize(grouped_data):
    for name, data in grouped_data.items():
        plt.title(name)
        for type_name, results in data.items():
            print(name, type_name, results)
            x = [sample['number'] for sample in results]
            y = [sample['time'] for sample in results]
            plt.plot(x, y, label=type_name)
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
        print(group(data))
        visualize(group(data))
    except json.decoder.JSONDecodeError as e:
        print(e)
        print("Did you forgot -x switch?")


if __name__ == "__main__":
    main()
