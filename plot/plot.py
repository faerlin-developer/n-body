import glob
import os
import sys

import matplotlib.pyplot as plt
import numpy as np


def main():
    """Plot the number of cores versus observed frame rates."""

    if len(sys.argv) < 2:
        print('USAGE: plot <DATA_DIRECTORY>')
        return

    data_dir = sys.argv[1]
    data = getFrameRates(data_dir)
    fig, axes = plt.subplots(1)
    plotFrameRate(axes, data)
    plt.show()


def plotFrameRate(ax, data):
    naive_x, naive_y, naive_std = getXY(data, 'Naive')
    barnes_x, barnes_y, barnes_std = getXY(data, 'BarnesHut')

    ax.axhline(y=24, color='black', linestyle='dotted', linewidth=1.5, label='24 Hz')

    (_, caps1, _) = ax.errorbar(barnes_x, barnes_y, barnes_std,
                                color='blue', marker='.', markersize=8, capsize=3,
                                linestyle='--', linewidth=1.0,
                                label='Barnes-Hut Inspired')

    (_, caps2, _) = ax.errorbar(naive_x, naive_y, naive_std,
                                color='red', marker='.', markersize=8, capsize=3,
                                linestyle='--', linewidth=1.0,
                                label='Brute-Force')

    caps1[0].set_markeredgewidth(1)
    caps1[1].set_markeredgewidth(1)
    caps2[0].set_markeredgewidth(1)
    caps2[1].set_markeredgewidth(1)

    ax.set_xlabel('Number of Cores', fontsize=14)
    ax.set_ylabel('Frame Rate (Hz)', fontsize=14)
    ax.set_title('Performance of N-Body Simulation', fontsize=14)
    ax.legend(loc='lower right', prop={'size': 12})


def getXY(data, simulator):
    xs = []
    ys = []
    stds = []

    points = []
    for key, value in data[simulator].items():
        points.append((int(key), value['avg'], value['std']))

    points.sort(key=lambda x: x)
    for point in points:
        x, y, std = point
        xs.append(x)
        ys.append(y)
        stds.append(std)

    return np.array(xs), np.array(ys), np.array(stds)


def getFrameRates(data_dir: str):
    """Read the frame rate data from the files in the given directory"""

    os.chdir(data_dir)

    data = {}
    filenames = glob.glob("*.data")
    for filename in filenames:

        name, _ = tuple(filename.split('.'))
        simulator, duration, size, cores = tuple(name.split('-'))

        if simulator not in data:
            data[simulator] = {}

        if cores not in data[simulator]:
            data[simulator][cores] = {}

        frame_rates = getColumn(filename, 0)
        data[simulator][cores]['avg'] = np.mean(frame_rates)
        data[simulator][cores]['std'] = np.std(frame_rates)

    return data


def getColumn(filename: str, column: int):
    data = np.loadtxt(filename, comments='#', dtype=float)
    return data[:, column]


if __name__ == '__main__':
    main()
