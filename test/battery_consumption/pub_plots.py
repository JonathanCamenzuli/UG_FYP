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
    """
    Merge Current and Label Data

    This function merges the current data and label data based on the timestamp column. It converts the timestamps to datetime objects and removes the decimal point from the timestamps. The merged dataframe is returned.

    Args:
        current_df (pd.DataFrame): The dataframe containing the current data.
        label_df (pd.DataFrame): The dataframe containing the label data.

    Returns:
        pd.DataFrame: The merged dataframe.
    """

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
    """
    Plot Data

    This function plots the current data over time and saves the plot as a PDF and PNG file if a path is provided. The title of the plot is also set.

    Args:
        data (pd.DataFrame): The dataframe containing the data to be plotted.
        title (str): The title of the plot.
        path (str): The path to save the plot. If None, the plot is only displayed.
    """

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
    """
    Plot Data with Markers

    This function plots the current data over time and adds vertical markers at specific points which indicate a particular event. The points with non-null 'Labels' are marked with red vertical lines. The plot is saved as a PDF and PNG file if a path is provided.

    Args:
        data (pd.DataFrame): The dataframe containing the data to be plotted.
        title (str): The title of the plot.
        path (str): The path to save the plot. If None, the plot is only displayed.
    """
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
        plt.savefig(f'{path}.pdf', dpi=300, bbox_inches='tight')
        plt.savefig(f'{path}.png', dpi=300, bbox_inches='tight')

    plt.show()


def get_current_desc(data, type):
    """
    Get Current Descriptions

    This function calculates and prints the average, maximum, and minimum current values for current data over 3 cycles.

    Args:
        data (pd.DataFrame): The dataframe containing the current data.
        type (str): The type of data.

    Returns:
        float: The average current value.
    """

    average = data['Current (mA)'].mean()
    maximum = data['Current (mA)'].max()
    minimum = data['Current (mA)'].min()
    print(f'The average current for {type} over 3 cycles is {average:.2f} mA')
    print(f'The maximum current for {type} over 3 cycles is {maximum:.2f} mA')
    print(f'The minimum current for {type} over 3 cycles is {minimum:.2f} mA')
    return average


def calc_battery_life(data, type, battery_cap_mAh=10000):
    """
    Calculate Battery Life

    This function calculates and prints the battery life for a specific type of data based on the average current and the battery capacity.

    Args:
        data (pd.DataFrame): The dataframe containing the current data.
        type (str): The type of data.
        battery_cap_mAh (float): The battery capacity in milliampere-hours (mAh). Default is 10000.
    """

    load_current = get_current_desc(data, type)
    battery_life_hrs = battery_cap_mAh/load_current
    battery_life_days = battery_life_hrs/24
    print(f'For a {battery_cap_mAh} mAh battery, {type} lasts {battery_life_hrs:.2f} hours ({battery_life_days:.2f} days)')


def plot_data_pub(data_source, start_idx, end_idx):
    """
    Plot Data for Publication

    This function reads the current and label data, merges them, and selects a specific range of data based on the start and end indices. It then plots the data and saves it as a PDF and PNG file.

    Args:
        data_source (str): The data source name.
        start_idx (int): The starting index of the data range.
        end_idx (int): The ending index of the data range.
    """

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


def set_plot_size(width, fraction=1):
    """
    Set Plot Size

    This function calculates the width and height of a plot based on the desired width in points. It returns the width and height in inches.

    Args:
        width (float): The desired width of the plot in points.
        fraction (float): The fraction of the golden ratio to be used as the height. Default is 1.

    Returns:
        tuple: The width and height of the plot in inches.
    """

    # Width of figure (in pts)
    fig_width_pt = width * fraction

    # Convert from pt to inches
    inches_per_pt = 1 / 72.27

    golden_ratio = (5**.5 - 1) / 2

    # Figure width in inches
    fig_width_in = fig_width_pt * inches_per_pt
    # Figure height in inches
    fig_height_in = fig_width_in * golden_ratio
    print(
        f'Plot width and height: {fig_width_in:.2f}x{fig_height_in:.2f} inches')

    return fig_width_in, fig_height_in


if __name__ == '__main__':
    mpl.rcParams['font.family'] = 'Lato'
    mpl.rcParams['font.sans-serif'] = 'Lato'

    width_pt = 417.5
    plot_width, plot_height = set_plot_size(width_pt)

    mpl.rcParams['figure.figsize'] = [plot_width, plot_height]

    data_sources = ['cps_cis', 'cps_ss', 'aqms', 'fds']
    start_idxs = [10, 15, 10, 10]
    end_idxs = [438, 232, 5409, 419]
    for idx, elem in enumerate(data_sources):
        plot_data_pub(elem, start_idxs[idx], end_idxs[idx])
