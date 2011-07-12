#ifndef __HEXDUMP_H__
#define __HEXDUMP_H__

/* Dumps size bytes of *data to stdout starting at "offset". Looks like:
 * [0000] 75 6E 6B 6E 6F 77 6E 20   30 FF 00 00 00 00 39 00 unknown 0.....9.
 *
 * Source: http://sws.dett.de/mini/hexdump-c/
 */
void hexdump(const void *data, unsigned int offset, unsigned int size);

#endif /* __HEXDUMP_H__ */
