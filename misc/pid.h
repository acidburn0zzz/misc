#ifndef __PID_H__
#define __PID_H__

void write_pid(const char *pid_file);

/*
 * Check if application is running at pid specified in file
 * Returns : 0 if application not running, -1 if pid file exists but can't be read
 *           pid of running app in other cases.
 */
int already_running(const char *pid_file, const char *app_name);

#endif /* __PID_H__ */
