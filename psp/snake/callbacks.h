#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_

#include <pspkernel.h>

int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks();

#endif /*_CALLBACKS_H_*/
