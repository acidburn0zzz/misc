#ifndef __PID_H__
#define __PID_H__

void write_pid(const char *pid_file);

/*
 * Check if application is running at pid specified in file
 * Returns : 0 if application not running, -1 if pid file exists but can't be read
 *           pid of running app in other cases.
 */
int is_app_running(const char *app_name, const char *pid_file);

#endif /* __PID_H__ */
