# Libaries
import sys
import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt

mpl.rcParams['font.family'] = 'Lato'
mpl.rcParams['font.sans-serif'] = 'Lato'


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


def plot_data(data, title, savefig, path=None):
    t = np.arange(len(data['Current (mA)']))

    # Plot the data
    plt.figure()
    plt.plot(t, data['Current (mA)'])
    plt.xlabel('Time (Seconds)')
    plt.ylabel('Current (mA)')
    plt.title(f'Current Measurement - {title}')
    plt.grid(True)

    if savefig == True:
        if path == None:
            print("Does Not Include Path", sys.stderr)
        else:
            plt.savefig(f'{path}.pdf', dpi=300)
            plt.savefig(f'{path}.png', dpi=300)

    plt.show()


def plot_data_markers(data, title, savefig, path=None):
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

    if savefig == True:
        if path == None:
            print("Does Not Include Path", file=sys.stderr)
        else:
            plt.savefig(f'{path}.pdf', dpi=300)
            plt.savefig(f'{path}.png', dpi=300)

    plt.show()


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
                  True, path=f'./assets/{data_source}_plot')
    elif data_source == 'cps_cis':
        plot_data(cycle_df, 'CPS (Change in State)', True,
                  path=f'./assets/{data_source}_plot')
    else:
        plot_data(cycle_df, data_source.upper(), True,
                  path=f'./assets/{data_source}_plot')


if __name__ == '__main__':
    data_sources = ['cps_cis', 'cps_ss', 'aqms', 'fds']
    start_idxs = [10, 15, 10, 10]
    end_idxs = [438, 232, 1822, 163]
    for idx, elem in enumerate(data_sources):
        plot_data_pub(elem, start_idxs[idx], end_idxs[idx])
