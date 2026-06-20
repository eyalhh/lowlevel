#ifndef ELFPARSER_PROGRAM_HEADERS_H
#define ELFPARSER_PROGRAM_HEADERS_H

#include <mytypes.h>
#include "elf_file.h"

elf_result elf_phdrs_parse(elf_context *ctx);

void elf_phdrs_print(const elf_context *ctx);

const char *elf_phdr_type_str(u32 p_type);

#endif
