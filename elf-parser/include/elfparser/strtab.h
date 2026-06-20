#ifndef ELFPARSER_STRTAB_H
#define ELFPARSER_STRTAB_H

#include <mytypes.h>

const char *elf_strtab_get(const char *table, usize size, usize offset);

#endif
