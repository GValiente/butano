"""
Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
zlib License, see LICENSE file.
"""

import multiprocessing
import os


def create_pool():
    processes = multiprocessing.cpu_count()

    if os.name == 'nt':
        processes = min(processes, 60)

    return multiprocessing.Pool(processes)
