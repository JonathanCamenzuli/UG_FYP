##
# @file pub_plots.py
#
# @author Jonathan Camenzuli
#
# @brief Script for Publishing Plots
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# @date 30/06/2023
#

import sys
import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt


def get_merged_data(current_df, label_df):

    # Convert the timestamps to datetime objects
    current_df_copy = current_df.copy()
    label_df_copy = label_df.copy()

    current_df_copy['Time'] = pd.to_datetime(
        current_df_copy['Time'], format='%H:%M:%S.%f').dt.time
    label_df_copy['Timestamps'] = pd.to_datetime(
        label_df_copy['Timestamps'], format='%H:%M:%S.%f').dt.time

    # Remove the decimal point from timestamps
    current_df_copy['Time'] = current_df_copy['Time'].apply(
        lambda x: x.strftime('%H:%M:%S'))
    label_df_copy['Timestamps'] = label_df_copy['Timestamps'].apply(
        lambda x: x.strftime('%H:%M:%S'))

    # Merge the dataframes based on the timestamp column
    merged_data = pd.merge(current_df_copy, label_df_copy,
                           left_on='Time', right_on='Timestamps', how='left')
    merged_data.drop(['Timestamps'], axis=1, inplace=True)
    return merged_data


def plot_data(data, title, path=None):
    t = np.arange(len(data['Current (mA)']))

    # Plot the data
    plt.figure()
    plt.plot(t, data['Current (mA)'])
    plt.xlabel('Time (Seconds)')
    plt.ylabel('Current (mA)')
    plt.title(f'Current Measurement - {title}')
    plt.grid(True)

    if path != None:
        plt.savefig(f'{path}.pdf', dpi=300)
        plt.savefig(f'{path}.png', dpi=300)

    plt.show()


def plot_data_markers(data, title, path=None):
    t = np.arange(len(data['Current (mA)']))

    # Plot the data
    plt.figure()
    plt.plot(t, data['Current (mA)'])
    plt.xlabel('Time (Seconds)')
    plt.ylabel('Current (mA)')
    plt.title(f'Current Measurement - {title}')
    plt.grid(True)

    for index, row in data.iterrows():
        if not pd.isna(row['Labels']):
            print(index, data['Labels'][index])
            plt.axvline(x=index, color='r', linestyle='--')

    if path != None:
        plt.savefig(f'{path}.pdf', dpi=300)
        plt.savefig(f'{path}.png', dpi=300)

    plt.show()


def get_current_average(data, type):
    average = data['Current (mA)'].mean()
    print(f'The average current for {type} over 3 cycles is {average:.2f} mA')
    return average


def calc_battery_life(data, type, battery_cap_mAh=10000):
    load_current = get_current_average(data, type)
    battery_life_hrs = battery_cap_mAh/load_current
    battery_life_days = battery_life_hrs/24
    print(f'For a {battery_cap_mAh} mAh battery, {type} lasts {battery_life_hrs:.2f} hours ({battery_life_days:.2f} days)')


def plot_data_pub(data_source, start_idx, end_idx):
    if data_source == 'cps_ss' or data_source == 'cps_cis':
        current_df = pd.read_csv(f'./cps/{data_source}_data.csv')
        label_df = pd.read_csv(f'./cps/{data_source}_timestamps.csv')
    else:
        current_df = pd.read_csv(f'./{data_source}/{data_source}_data.csv')
        label_df = pd.read_csv(f'./{data_source}/timestamps.csv')

    merged_df = get_merged_data(current_df, label_df)

    cycle_df = merged_df.drop(merged_df.index[end_idx:], axis=0)
    cycle_df = cycle_df.drop(cycle_df.index[0:start_idx], axis=0)
    cycle_df = cycle_df.reset_index()

    if data_source == 'cps_ss':
        plot_data(cycle_df, 'CPS (No Change in State)',
                  path=f'./assets/{data_source}_plot')
    elif data_source == 'cps_cis':
        plot_data(cycle_df, 'CPS (Change in State)',
                  path=f'./assets/{data_source}_plot')
    else:
        plot_data(cycle_df, data_source.upper(),
                  path=f'./assets/{data_source}_plot')


if __name__ == '__main__':
    mpl.rcParams['font.family'] = 'Lato'
    mpl.rcParams['font.sans-serif'] = 'Lato'
    mpl.rcParams['figure.figsize'] = [7.5, 3.75]

    data_sources = ['cps_cis', 'cps_ss', 'aqms', 'fds']
    start_idxs = [10, 15, 10, 10]
    end_idxs = [438, 232, 5409, 419]
    for idx, elem in enumerate(data_sources):
        plot_data_pub(elem, start_idxs[idx], end_idxs[idx])
