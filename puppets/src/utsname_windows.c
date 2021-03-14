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
    OSVERSIONINFO versionInfo;
    SYSTEM_INFO sysInfo;

    // Get Windows version info
    memset(&versionInfo, 0, sizeof(OSVERSIONINFO));
    versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&versionInfo);

    // Get hardware info
    memset(&sysInfo, 0, sizeof(SYSTEM_INFO));
    GetSystemInfo(&sysInfo);

    // Set implementation name
    snprintf(name->sysname, sizeof name->sysname, "Windows");
    //strcpy(name->sysname, "Windows");
    itoa(versionInfo.dwBuildNumber, name->release, 10);
    snprintf(name->version, sizeof versionInfo.dwMajorVersion +
                            sizeof versionInfo.dwMinorVersion,
                            "%lu.%lu",
                            versionInfo.dwMajorVersion,
                            versionInfo.dwMinorVersion);

    // Set hostname
    gethostname(name->nodename, UTSNAME_FIELD_MAXLEN);

    // Set processor architecture
    switch (sysInfo.wProcessorArchitecture) {
    case PROCESSOR_ARCHITECTURE_AMD64:
        //strcpy(name->machine, "x86_64");
        snprintf(name->machine, sizeof name->machine, "x86_64");
        break;
    case PROCESSOR_ARCHITECTURE_IA64:
        //strcpy(name->machine, "ia64");
        snprintf(name->machine, sizeof name->machine, "ia64");
        break;
    case PROCESSOR_ARCHITECTURE_INTEL:
        //strcpy(name->machine, "x86");
        snprintf(name->machine, sizeof name->machine, "x86");
        break;
    case PROCESSOR_ARCHITECTURE_UNKNOWN:
    default:
        //strcpy(name->machine, "unknown");
        snprintf(name->machine, sizeof name->machine, "Unknown");
    }
    return 0;
}
#endif
