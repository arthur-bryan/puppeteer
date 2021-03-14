// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 *
 * Sends data from disk over a connected socket
 *
 * Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>
 */
#ifdef _WIN32
#include "../include/utsname_windows.h"
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>

uint16_t
uname(struct w_utsname *name) {
    OSVERSIONINFO   versionInfo;
    SYSTEM_INFO     sysInfo;
    WSADATA         wsaData;

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    memset(&versionInfo, 0, sizeof(OSVERSIONINFO));
    versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&versionInfo);

    memset(&sysInfo, 0, sizeof(SYSTEM_INFO));
    GetSystemInfo(&sysInfo);
    snprintf(name->sysname, sizeof name->sysname, "Windows");
    itoa(versionInfo.dwBuildNumber, name->release, 10);
    snprintf(name->version, sizeof versionInfo.dwMajorVersion +
                            sizeof versionInfo.dwMinorVersion + 1,
                            "%lu.%lu",
                            versionInfo.dwMajorVersion,
                            versionInfo.dwMinorVersion);

    gethostname(name->nodename, UTSNAME_FIELD_MAXLEN);
    switch (sysInfo.wProcessorArchitecture) {
    case PROCESSOR_ARCHITECTURE_AMD64:
        snprintf(name->machine, sizeof name->machine, "x86_64");
        break;
    case PROCESSOR_ARCHITECTURE_IA64:
        snprintf(name->machine, sizeof name->machine, "ia64");
        break;
    case PROCESSOR_ARCHITECTURE_INTEL:
        snprintf(name->machine, sizeof name->machine, "x86");
        break;
    case PROCESSOR_ARCHITECTURE_UNKNOWN:
    default:
        snprintf(name->machine, sizeof name->machine, "Unknown");
    }
    WSACleanup();
    return 0;
}
#endif
