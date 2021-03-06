"""
    MIT License
    Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>

    This module belongs to https://github.com/arthur-bryan/puppeteer
    A implementation of a botnet using Python on server (C&C) side
    and C on the bots side.
"""

LISTEN_ADDRESS = "0.0.0.0"  # server listens on any interface
LISTEN_PORT = 1771
DATABASE_PATH = "database/database.db"
MAX_COMMAND_OUTPUT_SIZE = 48000
INT_C_SIZE = 4  # size (in bytes) of a int in C
CHAR_C_SIZE = 1  # size (in bytes) of a char in C


# For terminal coloring
def to_green(string):
    """ Converts a string to bright green color (16bit)

        Returns:
            str: the string in bright green color

    """
    return f"\u001b[32;1m{string}\u001b[0m"


def to_yellow(string):
    """ Converts a string to yellow color (8bit)

    Returns:
        str: the string in yellow color

    """
    return f"\u001b[33m{string}\u001b[0m"


def to_red(string):
    """ Converts a string to bright red color (16bit)

    Returns:
        str: the string in bright red color

    """
    return f"\u001b[31;1m{string}\u001b[0m"
