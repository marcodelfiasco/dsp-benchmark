import os
import matplotlib.pyplot as plt
import pandas as pd
import glob

OUTPUT_DIR = 'results/plots'
FIR_SIZES = (16, 32, 64, 128, 256, 512, 1024, 2048, 4096)
BUFFER_SIZES = (8, 16, 32, 64, 128)
TIME_COLS = ('min_nsec', 'avg_nsec', 'max_nsec')  # preserve min, avg, max ordering!
NMAC_COLS = ('min_mac_per_nanosec', 'avg_mac_per_nanosec', 'max_mac_per_nanosec')  # preserve min, avg, max ordering!
FONT_SIZE = 12
FIG_SIZE = (11.69, 8.27)
FIG_DPI = 300
VERBOSE = False

def read_csv_files():
    csv_files = glob.glob('results/*.csv')
    dataframes = {}
    for file in csv_files:
        dataframes[file] = pd.read_csv(file)
        if not dataframes[file][dataframes[file]['result'].apply(lambda x: 'OK' not in x)].empty:
            raise Exception(f'{file} has failed tests!')
    return dataframes


def dataframes_drop_tests(input, names):
    dataframes = {}
    for (file, df) in zip(input.items()):
        dataframes[file] = df[df['name'].apply(lambda x: x not in names)]
    return dataframes


def plot_compare_multiple_arch(dataframes, test_names, buffer_size, fir_sizes, columns, ylabel, output_file, VERBOSE=False):
    if VERBOSE:
        print('Creating plot ' + output_file)

    plt.figure(figsize=FIG_SIZE, dpi=FIG_DPI)
    plt.rcParams['font.size'] = FONT_SIZE

    colors = ['blue', 'red', 'green']

    y_max = 0
    for (file, df), test_name, color in zip(dataframes.items(), test_names, colors):
        if VERBOSE:
            print(test_name)
            print(buffer_size)

        filtered_data = df.query('name == @test_name')
        filtered_data = filtered_data.query('buffer_size == @buffer_size')
        filtered_data = filtered_data.query('fir_size in @fir_sizes')
        filtered_data = filtered_data.sort_values(by='fir_size')
        if VERBOSE:
            print(f'{file}:{test_name}:{buffer_size}')
            print(filtered_data)

        if (filtered_data.empty):
            raise Exception(f'Empty data: {test_name}-{buffer_size}')

        x_values = filtered_data['fir_size']
        y_name = f'{os.path.splitext(os.path.basename(file))[0]}-{test_name}-{buffer_size}'

        min_label = columns[0]
        avg_label = columns[1]
        max_label = columns[2]

        plt.semilogx(x_values, filtered_data[min_label], 
                        marker='o', linestyle='-', color=color, alpha=0.3,
                    label=f'{y_name}, min')

        plt.semilogx(x_values, filtered_data[avg_label],
                    marker='s', linestyle='None', color=color,
                    label=f'{y_name}, avg')

        plt.semilogx(x_values, filtered_data[max_label],
                    marker='o', linestyle='-', color=color, alpha=0.3,
                    label=f'{y_name}, max')

        # Fill between min and max
        plt.fill_between(x_values, 
                        filtered_data[min_label],
                        filtered_data[max_label],
                        color=color, alpha=0.1)

        # Update maximum
        y_max = max(y_max, filtered_data[max_label].max())

    plt.title(f'{output_file}')
    plt.grid(True, which="both", ls="-", alpha=0.2)
    plt.tick_params(
        axis='x',
        which='both',
        bottom=False,
        top=False,
        labelbottom=True)
    plt.xticks(x_values, x_values)
    plt.xlabel('FIR size')
    plt.ylabel(ylabel)
    plt.ylim(0, y_max * 1.05)
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.tight_layout()

    os.makedirs(OUTPUT_DIR, exist_ok=True)
    plt.savefig(f'{OUTPUT_DIR}/{output_file}.pdf', bbox_inches='tight')
    plt.close()


def plot_compare_one_arch(dataframes, file, test_names, buffer_size, fir_sizes, column, ylabel, output_file):
    df = dataframes[file]

    if VERBOSE:
        print('Creating plot ' + output_file)

    plt.figure(figsize=FIG_SIZE, dpi=FIG_DPI)
    plt.rcParams['font.size'] = FONT_SIZE

    markers = ['o', '.', ',', 'x', '+', 'v', '^', '<', '>', 's', 'd']

    y_max = 0
    for test_name, marker in zip(test_names, markers):
        filtered_data = df.query('name in @test_name')
        filtered_data = filtered_data.query('buffer_size == @buffer_size')
        filtered_data = filtered_data.query('fir_size in @fir_sizes')
        filtered_data = filtered_data.sort_values(by='fir_size')
        if VERBOSE:
            print(f'{file}:{test_name}:{buffer_size}')
            print(filtered_data)

        if (filtered_data.empty):
            raise Exception(f'Empty data: {test_name}-{buffer_size}')

        x_values = filtered_data['fir_size']
        y_name = f'{os.path.splitext(os.path.basename(file))[0]}-{test_name}-{buffer_size}'

        label = column

        plt.semilogx(x_values, filtered_data[label],
                     marker=marker, linestyle='-',
                     label=f'{y_name}, avg')

    # Update maximum
        y_max = max(y_max, filtered_data[label].max())

    plt.title(f'{output_file}')
    plt.grid(True, which="both", ls="-", alpha=0.2)
    plt.tick_params(
        axis='x',
        which='both',
        bottom=False,
        top=False,
        labelbottom=True)
    plt.xticks(x_values, x_values)
    plt.xlabel('FIR size')
    plt.ylabel(ylabel)
    plt.ylim(0, y_max * 1.05)
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.tight_layout()

    os.makedirs(OUTPUT_DIR, exist_ok=True)
    plt.savefig(f'{OUTPUT_DIR}/{output_file}.pdf', bbox_inches='tight')
    plt.close()


def main():
    dataframes = read_csv_files()

    COMPARE_ONE_ADSP = [
        'FIR.basic',
        'FIR.basic_restrict',
        'FIR.basic_dual_bank',
        'FIR.basic_aligned',
        'FIR.basic_loop_cnt',
        'FIR.circular',
        'FIR.circular_opt',
        'FIR.lib',
    ]
    COMPARE_ONE_IMXRT = [
        'FIR.basic_tcm',
        'FIR.basic_tcm_restrict',
        'FIR.circular_tcm',
        'FIR.circular_tcm_restrict',
        'FIR.circular_tcm_unroll',
        'FIR.opt_tcm',
        'FIR.cmsis_tcm',
    ]
    COMPARE_ONE_RPI4 = [
        'FIR.basic',
        'FIR.basic_restrict',
        'FIR.circular',
        'FIR.circular_restrict',
        'FIR.opt',
        'FIR.cmsis',
    ]

    COMPARE_MULTI_BASIC = [
        'FIR.basic_tcm',    #imxrt
        'FIR.basic',        #rpi
        'FIR.basic'         #adsp
    ]

    COMPARE_MULTI_BEST = [
        'FIR.cmsis_tcm',    #imxrt
        'FIR.opt',          #rpi
        'FIR.lib'           #adsp
    ]

    COMPARE_MULTI_BASIC_CACHE_THRASH = [
        'FIR.basic_ddr_cache_thrash',   #imxrt
        'FIR.basic_cache_thrash',       #rpi
        'FIR.basic'                     #adsp
    ]

    COMPARE_MULTI_BEST_CACHE_THRASH = [
        'FIR.cmsis_ddr_cache_thrash',    #imxrt
        'FIR.opt_cache_thrash',          #rpi
        'FIR.lib'                        #adsp
    ]

    for buffer_size in BUFFER_SIZES:
        # To see which version is best for each architecture
        plot_compare_one_arch(dataframes, 'results/adsp.csv',  COMPARE_ONE_ADSP,  buffer_size, FIR_SIZES, 'avg_mac_per_nanosec', 'MAC/nsec', f'01-adsp-variants-{buffer_size}')
        plot_compare_one_arch(dataframes, 'results/imxrt.csv', COMPARE_ONE_IMXRT, buffer_size, FIR_SIZES, 'avg_mac_per_nanosec', 'MAC/nsec', f'02-imxrt-variants-{buffer_size}')
        plot_compare_one_arch(dataframes, 'results/rpi4.csv',  COMPARE_ONE_RPI4,  buffer_size, FIR_SIZES, 'avg_mac_per_nanosec', 'MAC/nsec', f'03-rpi4-variants-{buffer_size}')

        # To see how the naive basic implementation behaves among different targets
        plot_compare_multiple_arch(dataframes, COMPARE_MULTI_BASIC, buffer_size, FIR_SIZES, TIME_COLS, 'nsec',     f'04-basic-time-{buffer_size}')
        plot_compare_multiple_arch(dataframes, COMPARE_MULTI_BASIC, buffer_size, FIR_SIZES, NMAC_COLS, 'MAC/nsec', f'05-basic-norm-mac-{buffer_size}')

        # To see how the best implementation behaves among different targets
        # warning: COMPARE_MULTI_BEST must be set pointing to the best test names!
        plot_compare_multiple_arch(dataframes, COMPARE_MULTI_BEST, buffer_size, FIR_SIZES, TIME_COLS, 'nsec',     f'06-best-time-{buffer_size}')
        plot_compare_multiple_arch(dataframes, COMPARE_MULTI_BEST, buffer_size, FIR_SIZES, NMAC_COLS, 'MAC/nsec', f'07-best-norm-mac-{buffer_size}')

        # To compare the effect of cache thrashing among different architectures (imxrt using ddr for this test, adsp still running from internal memory but there for reference)
        # warning: COMPARE_MULTI_BASIC_CACHE_THRASH must be set pointing to the best test names!
        plot_compare_multiple_arch(dataframes, COMPARE_MULTI_BASIC_CACHE_THRASH, buffer_size, FIR_SIZES, TIME_COLS, 'nsec',     f'08-basic_cache_thrash-time-{buffer_size}')
        plot_compare_multiple_arch(dataframes, COMPARE_MULTI_BASIC_CACHE_THRASH, buffer_size, FIR_SIZES, NMAC_COLS, 'MAC/nsec', f'09-basic_cache_thrash-norm-mac-{buffer_size}')

        # To compare the effect of cache thrashing among different architectures (imxrt using ddr for this test, adsp still running from internal memory but there for reference)
        # warning: COMPARE_MULTI_BEST_CACHE_THRASH must be set pointing to the best test names!
        plot_compare_multiple_arch(dataframes, COMPARE_MULTI_BEST_CACHE_THRASH, buffer_size, FIR_SIZES, TIME_COLS, 'nsec',     f'10-best_cache_thrash-time-{buffer_size}')
        plot_compare_multiple_arch(dataframes, COMPARE_MULTI_BEST_CACHE_THRASH, buffer_size, FIR_SIZES, NMAC_COLS, 'MAC/nsec', f'11-best_cache_thrash-norm-mac-{buffer_size}')
        
if __name__ == '__main__':
    main()
