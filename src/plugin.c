/******************************************************************************
 * Copyright (C) 2019  Renan S. Silva                                         *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgment in the product documentation would be   *
 *    appreciated but is not required.                                        *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#include "plugin.h"

void (*plugin)();

void load_plugin() {
#ifdef __APPLE__
    load_plugin_dylib();
#endif

#ifdef __linux__
    load_pluginso();
#endif
}

void load_plugin_dylib() {
    char *error;
    void *handle;

    handle = dlopen("./plugin.dylib", RTLD_LAZY);
    if (!handle) {
        fputs(dlerror(), stderr);
        printf("plugin.dylib was not found");
        return;
    }

    plugin = dlsym(handle, "global_callback");
    if ((error = dlerror()) != NULL) {
        fputs(error, stderr);
        exit(1);
    }
}

void load_plugin_so() {
    char *error;
    void *handle;

    handle = dlopen("./plugin.so", RTLD_LAZY);
    if (!handle) {
        fputs(dlerror(), stderr);
        printf("plugin.so was not found");
        return;
    }

    plugin = dlsym(handle, "global_callback");
    if ((error = dlerror()) != NULL) {
        fputs(error, stderr);
        exit(1);
    }
}
