/******************************************************************************
 * Copyright (C) 2016-2017  Renan S. Silva                                    *
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

#include <stdio.h>

#include "ff_controller.h"
#include "lelmark.h"

static _ff_controller ff_ctrl;

void ff_ctrl_reset() {
    ff_ctrl.current = -1;
    fprintf(stderr, "ff_ctrl_reset() was called\n");
}

int ff_ctrl_next() {
    ff_ctrl.current += 1;

    fprintf(stderr, "ff_ctrl_next() was called: %2d\n", ff_ctrl.current);
    return ff_ctrl.current;
}

int ff_ctrl_current() {
    fprintf(stderr, "ff_ctrl_current() was called: %2d\n", ff_ctrl.current);
    return ff_ctrl.current;
}

int ff_ctrl_ngens() {
    fprintf(stderr, "ff_ctrl_ngens() was called: %2d\n", GEN_P_FUNCTION * (ff_ctrl_current() + 1));
    return GEN_P_FUNCTION * (ff_ctrl.current + 1);
}
