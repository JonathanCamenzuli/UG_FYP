##
# @file pub_plot.py
#
# @author Jonathan Camenzuli
#
# @brief Script for Publishing Plot
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# @date 01/07/2023
#

import sys
import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt


def get_merged_data(resource_df, label_df):

    # Convert the timestamps to datetime objects
    resource_df_copy = resource_df.copy()
    label_df_copy = label_df.copy()

    resource_df_copy['Timestamp'] = pd.to_datetime(
        resource_df_copy['Timestamp'], format='%Y-%m-%d %H:%M:%S').dt.time
    label_df_copy['Timestamp'] = pd.to_datetime(
        label_df_copy['Timestamp'], format='%Y-%m-%d %H:%M:%S').dt.time

    # Merge the dataframes based on the timestamp columns
    merged_data = pd.merge(resource_df_copy, label_df_copy,
                           left_on='Timestamp', right_on='Timestamp', how='left')
    return merged_data


def plot_data(data, path=None):
    t = np.arange(len(data['Timestamp']))
    t *= 2

    plt.plot(t, data['CPU (%)'])
    plt.plot(t, data['Memory (%)'])
    plt.plot(t, data['Disk (%)'])
    # plt.plot(t, data['Temperature (C)'])

    plt.xlabel('Time (Seconds)')
    plt.ylabel('Utilisation (%)')
    plt.title('System Resource Utilisation')
    plt.legend(['CPU Utilisation (%)', 'Memory Utilisation (%)',
               'Disk Utilisation (%)'])
    plt.grid(True)

    if path != None:
        plt.savefig(f'{path}.pdf', dpi=300, bbox_inches='tight')
        plt.savefig(f'{path}.png', dpi=300, bbox_inches='tight')

    # Display the plot
    plt.show()


def plot_data_markers(data, path=None):
    t = np.arange(len(data['Timestamp']))
    t *= 2

    fig, axs = plt.subplots(1, 1)

    axs.plot(t, data['CPU (%)'], label='CPU Utilisation (%)')
    axs.plot(t, data['Memory (%)'], label='Memory Utilisation (%)')
    axs.plot(t, data['Disk (%)'], label='Disk Utilisation (%)')
    # plt.plot(t, data['Temperature (C)'])

    axs.set_xlabel('Time (Seconds)')
    axs.set_ylabel('Utilisation (%)')
    axs.set_title('System Resource Utilisation')
    axs.grid(True)

    # Create an empty dictionary to store the line types that have been plotted
    plotted_lines = {}

    for index, row in data.iterrows():
        if not pd.isna(row['Label']):
            if "CoAP Payload Received from" in row['Label']:
                if "Renderable Error" in row['Label']:
                    line_type = 'CoAP Renderable Error (Unexpected)'
                    line_color = 'orange'
                else:
                    line_type = 'Expected CoAP Message Received'
                    line_color = 'red'
            else:
                if "192.168.1.254" in row['Label']:
                    line_type = 'Expected HTTP Request'
                    line_color = 'mediumblue'
                else:
                    line_type = 'Unexpected HTTP Request'
                    line_color = 'purple'

            if line_type not in plotted_lines:
                # Plot the vertical line only if it hasn't been plotted before
                axs.axvline(x=index*2, color=line_color,
                            linestyle='--', label=line_type)
                plotted_lines[line_type] = True
            else:
                # Plot the vertical line without a label if it has been plotted before
                axs.axvline(x=index*2, color=line_color, linestyle='--')

    axs.legend(loc='lower center', bbox_to_anchor=(
        0.5, -0.4, 0, 0), ncol=2, fontsize=7.5)

    fig.subplots_adjust(bottom=0.15)

    if path != None:
        plt.savefig(f'{path}.pdf', dpi=300, bbox_inches='tight')
        plt.savefig(f'{path}.png', dpi=300, bbox_inches='tight')

    plt.show()


def set_plot_size(width, fraction=1):
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

    resource_df = pd.read_csv('./resource_data.csv')
    label_df = pd.read_csv('./timestamps.csv')

    merged_df = get_merged_data(resource_df, label_df)

    plot_data(merged_df, 'load_plot')
    plot_data_markers(merged_df, 'load_plot_markers')
