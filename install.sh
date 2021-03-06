#!/bin/bash

# THIS SCRIPT REQUIRE ELEVATED PRIVILEGES
if [ $EUID -ne 0 ]; then
    echo "Invalid permission!"
else
    # INSTALL PACKAGES TO BE ABLE TO COMPILE THE SOURCE FOR WINDOWS
    echo -n "[ ? ] You want to be able to compile for windows? (Y/N)>> "
    read -r WINDOWS_COMPILE
    if [ "$WINDOWS_COMPILE" = "Y" ] || [ "$WINDOWS_COMPILE" = "y" ]; then
        apt install mingw-w64 2>/dev/null
        clear
    fi
    # INSTALL PACKAGES TO BE ABLE TO COMPILE SOURCE FOR 32BIT LINUX
    SYSTEM_ARCH=$(uname -m)
    if [ "$SYSTEM_ARCH" = "x86_64" ];then
        echo -n "[ ? ] You want to be able to compile for 32bit Linux? (Y/N)>> "
        read -r LINUX_32_COMPILE
        if [ "$LINUX_32_COMPILE" = "Y" ] || [ "$LINUX_32_COMPILE" = "y" ]; then
            apt install gcc-multilib
            clear
        fi
    fi
    echo "[ + ] Tools installed successfully!"
fi
