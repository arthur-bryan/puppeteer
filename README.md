![puppet](https://user-images.githubusercontent.com/34891953/108602305-11687a80-7380-11eb-971c-cf5c3b66bb5f.png)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/c67853a2e7794106bf30e920c14fce45)](https://app.codacy.com/gh/arthur-bryan/puppeteer?utm_source=github.com&utm_medium=referral&utm_content=arthur-bryan/puppeteer&utm_campaign=Badge_Grade_Settings)
[![Open Source](https://img.shields.io/badge/Open%20Source-05C230?logo=Github&logoColor=white&link=https://github.com/arthur-bryan/pisco)](https://github.com/arthur-bryan/pisco)
[![Status Badge](https://img.shields.io/badge/status-development-05C230)](https://github.com/arthur-bryan/puppeteer)
[![GitHub License](https://img.shields.io/github/license/arthur-bryan/puppeteer?color=05C230)](https://github.com/arthur-bryan/puppeteer/blob/master/LICENSE.md)
[![GitHub tag (latest by date)](https://img.shields.io/github/v/tag/arthur-bryan/puppeteer?color=05C230)](https://github.com/arthur-bryan/puppeteer/tags)
[![Python Badge](https://img.shields.io/badge/-Python-05C230?logo=Python&logoColor=white&link=https://www.python.org/)](https://github.com/arthur-bryan?tab=repositories&q=&type=&language=python)
[![C Badge](https://img.shields.io/badge/-C-05C230?logo=C&logoColor=white)](https://github.com/arthur-bryan?tab=repositories&q=&type=&language=c)
![GitHub all releases](https://img.shields.io/github/downloads/arthur-bryan/puppeteer/total?color=05C230)
![GitHub repo size](https://img.shields.io/github/repo-size/arthur-bryan/puppeteer?color=05C230)

# :octocat: Puppeteer
Implementation of a botnet with the C&C server (puppeteer) written in Python and the bots (puppets) code written in C

-   :heavy_check_mark: Interact with puppets while keeps listening incoming connections
-   :heavy_check_mark: Interact with one or all puppets at a time
-   :heavy_check_mark: Executable makes a copy of itself to a hidden folder
-   :heavy_check_mark: Store/update puppets information as soon as the puppet connect
-   :heavy_check_mark: Puppets will run the backdoor automatically at reboots
-   :heavy_check_mark: Puppets attempts to reconnect automatically in case of connection lost
-   :heavy_check_mark: SYN Flood attack with random generated IP Address
-   :heavy_check_mark: Download/Upload files up to 4 GB from/to puppets
-   :heavy_check_mark: List all files (and sizes and permissions) of the puppets directories
-   :heavy_check_mark: Run shell commands on the puppets

## :hammer_and_wrench: Installation/compilation
Compiling sources for puppet executables:
```sh
git clone https://github.com/arthur-bryan/puppeteer
cd botnet
make
```
By default, the make command will compile for Linux 64 bits. If you want to compile for 32 bits (Windows 32/64 bits are not avaliable yet) follow these steps:
```sh
chmod +x install.sh
sudo ./install.sh
make OUTPUT=linux32
```
The compiled binaries will be stored at the `bin` folder

## :computer: Usage
PS: Server IP Addres / Port are defined at `puppets/linux/include/sockets.h`

Start the server
```sh
cd botnet
python3 main.py
```
Wait or make an bot execute the compiled binary then starts interaction

![Demo](https://user-images.githubusercontent.com/34891953/110195368-6ba31a00-7e1b-11eb-8d04-af5bfdf6eb72.png)

## :octocat: License
[MIT License](https://github.com/arthur-bryan/puppeteer/blob/master/LICENSE.md)

## :construction: In progress
-   :hourglass_flowing_sand: Puppet sources for Windows
