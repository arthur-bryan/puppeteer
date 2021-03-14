![puppet](https://user-images.githubusercontent.com/34891953/108602305-11687a80-7380-11eb-971c-cf5c3b66bb5f.png)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/c435ffb74d7547e3a3801baea4b88b8e)](https://www.codacy.com/gh/arthur-bryan/puppeteer/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=arthur-bryan/puppeteer&amp;utm_campaign=Badge_Grade)
[![Open Source](https://img.shields.io/badge/Open%20Source-05C230?logo=Github&logoColor=white&link=https://github.com/arthur-bryan/pisco)](https://github.com/arthur-bryan/pisco)
[![Status Badge](https://img.shields.io/badge/status-development-05C230)](https://github.com/arthur-bryan/puppeteer)
![GitHub release (latest SemVer)](https://img.shields.io/github/v/release/arthur-bryan/puppeteer?color=05C230)
[![GitHub tag (latest by date)](https://img.shields.io/github/v/tag/arthur-bryan/puppeteer?color=05C230)](https://github.com/arthur-bryan/puppeteer/tags)
[![GitHub License](https://img.shields.io/github/license/arthur-bryan/puppeteer?color=05C230)](https://github.com/arthur-bryan/puppeteer/blob/master/LICENSE.md)
[![Python Badge](https://img.shields.io/badge/-Python-05C230?logo=Python&logoColor=white&link=https://www.python.org/)](https://github.com/arthur-bryan?tab=repositories&q=&type=&language=python)
[![C Badge](https://img.shields.io/badge/-C-05C230?logo=C&logoColor=white)](https://github.com/arthur-bryan?tab=repositories&q=&type=&language=c)
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
PS: Server IP Address / Port are defined at `puppets/linux/include/sockets.h`

Start the server
```sh
cd botnet
python3 main.py
```
Wait or make an bot execute the compiled binary then starts interaction

![DEMO](https://user-images.githubusercontent.com/34891953/111054361-e0450c80-844a-11eb-9c40-1be541d4b2fc.png)

## :copyright: License
[MIT License](https://github.com/arthur-bryan/puppeteer/blob/master/LICENSE.md)

## :construction: In progress
-   :hourglass_flowing_sand: Puppet sources for Windows
