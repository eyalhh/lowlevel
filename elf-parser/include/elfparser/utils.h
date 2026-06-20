#ifndef ELF_PARSER_UTILS_H
#define ELF_PARSER_UTILS_H

#include <mytypes.h>

b8 range_ok(usize size, usize offset, usize len);

const void *mem_at(const u8 *base, usize size, usize offset, usize len);

void eprintf(const char *fmt, ...);

void hexdump(const u8 *data, usize len, usize file_off);

#endif
