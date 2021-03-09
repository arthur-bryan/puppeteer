"""
    MIT License
    Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>

    This module belongs to https://github.com/arthur-bryan/puppeteer:
    A implementation of a botnet using Python on server (C&C) side
    and C on the puppets side.

    This module contains the class that represents the Database, with the
    responsible methods for create/update the database based on the server
    requests
"""

import sqlite3
from config import to_red


class Database:
    """ Creates database (if not exists), then connect to it, defines
        a cursor and then creates the puppets table if not exists

    """

    def __init__(self, filename):
        """ Args:
                filename (str): the path for the database

        """

        try:
            self.conn = sqlite3.connect(filename, check_same_thread=False)
            self.conn.row_factory = lambda cursor, row: row[0]
        except sqlite3.Error as error:
            print(to_red(f"\n[ DATABASE ERROR ] {error} {filename}\n"))
        else:
            self.cursor = self.conn.cursor()
            self.create_table()

    def create_table(self):
        """ Creates (if not exists) the table to store the puppets infos """
        try:
            self.cursor.execute(
                """
                CREATE TABLE IF NOT EXISTS puppets (
                    ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
                    IP_ADDRESS VARCHAR(15),
                    IS_CONNECTED INTEGER,
                    AUTORUN_ENABLED INTEGER,
                    OP_SYSTEM VARCHAR(15),
                    ARCHITECTURE VARCHAR(15),
                    KERNEL_RELEASE VARCHAR(30),
                    HOSTNAME VARCHAR(20),
                    USERNAME VARCHAR(20),
                    LAST_CONNECTION DATE NOT NULL,
                    HASH TEXT NOT NULL UNIQUE
                );
                """)
        except sqlite3.Error as error:
            print(to_red(f"\n[ DATABASE ERROR ] {error}\n"))
        else:
            self.conn.commit()

    def add_puppet(self, puppet):
        """ Inserts the puppet information to the table
            As some puppet attributes are received as C string (terminated by
            '\x00'), these strings must be sliced to prevent sqlite store it as
            BLOB binary data

            Args:
                puppet (:obj: 'Puppet'):  puppet to be added to database

        """
        try:
            self.cursor.execute(
                """
                INSERT INTO puppets (
                    IP_ADDRESS,
                    IS_CONNECTED,
                    AUTORUN_ENABLED,
                    OP_SYSTEM,
                    ARCHITECTURE,
                    KERNEL_RELEASE,
                    HOSTNAME,
                    USERNAME,
                    LAST_CONNECTION,
                    HASH
                )
                VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
                """, (puppet.ip_address,
                      puppet.is_connected,
                      puppet.autorun_is_enabled,
                      puppet.op_system[:-1],
                      puppet.architecture[:-1],
                      puppet.kernel_release[:-1],
                      puppet.hostname[:-1],
                      puppet.username[:-1],
                      puppet.last_connection,
                      puppet.id_hash)
            )
        except ValueError as error:
            print(to_red(f"\n[ DATABASE ERROR ] {error} inserting\n"))
        else:
            self.conn.commit()

    def update_all_puppet_info(self, puppet):
        """ Update all information of the puppet

            Args:
                puppet (:obj: 'Puppet'):  puppet with new values to replace the
                    old ones

        """
        try:
            self.cursor.execute(
                """
                UPDATE puppets
                SET IP_ADDRESS = ?,
                    IS_CONNECTED = ?,
                    AUTORUN_ENABLED = ?,
                    OP_SYSTEM = ?,
                    ARCHITECTURE = ?,
                    KERNEL_RELEASE = ?,
                    HOSTNAME = ?,
                    USERNAME = ?,
                    LAST_CONNECTION = ?,
                    HASH = ?
                WHERE HASH = ?
                """, (puppet.ip_address,
                      puppet.is_connected,
                      puppet.autorun_is_enabled,
                      puppet.op_system[:-1],
                      puppet.architecture[:-1],
                      puppet.kernel_release[:-1],
                      puppet.hostname[:-1],
                      puppet.username[:-1],
                      puppet.last_connection,
                      puppet.id_hash,
                      puppet.id_hash)
            )
        except sqlite3.Error as error:
            print(to_red(f"\n[ DATABASE ERROR ] {error} updating\n"))
        else:
            self.conn.commit()

    def update_puppet_status(self, puppet, new_status):
        """ Updates a connection status of the puppet

            Args:
                puppet (:obj: 'Puppet'):  puppet to be updated
                new_status (int):    the new value for the status

        """
        try:
            self.cursor.execute(
                """
                UPDATE puppets
                SET IS_CONNECTED = ?
                WHERE HASH = ?
                """, (new_status,
                      puppet.id_hash)
                )
        except sqlite3.Error as error:
            print(to_red(f"\n[ DATABASE ERROR ] {error} update status\n"))
        else:
            self.conn.commit()

    def get_all_puppets(self):
        """ Fetches all puppets on database and returns them in a list of tuples

            Returns:
                puppets (:obj: 'list' of :obj: 'tuples'):  list of tuples with
                    puppet information (one tuple per puppet)

        """
        try:
            puppets = self.cursor.execute("SELECT * FROM puppets;").fetchall()
            return puppets
        except sqlite3.Error as error:
            print(f"\n [ DATABASE ERROR ] {error}\n")

    def get_connected_puppets(self):
        """ Fetches all puppets on database that are currently connected to the
            server and returns them in a list of tuples

            Returns:
                puppets (:obj: 'list' of :obj: 'tuples'):  list of tuples with
                    the connected puppets information (one tuple per puppet)

        """
        try:
            connected_puppets = self.cursor.execute(
                "SELECT * FROM puppets WHERE IS_CONNECTED = 1;"
            ).fetchall()
            return connected_puppets
        except sqlite3.Error as error:
            print(to_red(f"\n [ DATABASE ERROR ] {error}\n"))

    def get_puppets_hashes(self):
        """ Fetches and returns the hashes of all puppets on database

            Returns:
                puppets_hashes (:obj: 'tuple'):     tuple containing the
                    hashes of the puppets in database

        """
        try:
            puppets_hashes = self.cursor.execute(
                "SELECT HASH FROM puppets;"
            ).fetchall()
            return puppets_hashes
        except sqlite3.Error as error:
            print(to_red(f"\n [ DATABASE ERROR ] {error}\n"))

    def disconnect_puppets_on_exit(self):
        """ Updates a connection status of all puppets to 0 when the server
            stops

        """
        try:
            self.cursor.execute(
                """
                UPDATE puppets
                SET IS_CONNECTED = 0
                """
            )
        except sqlite3.Error as error:
            print(to_red(f"\n[ DATABASE ERROR ] {error}\n"))
        else:
            self.conn.commit()
