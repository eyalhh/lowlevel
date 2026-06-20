#ifndef ELFPARSER_SYMBOLS_H
#define ELFPARSER_SYMBOLS_H

#include <elf.h>
#include <mytypes.h>
#include "elf_file.h"

elf_result elf_symbols_parse(elf_context *ctx);

void elf_symbols_print(const elf_context *ctx);

const char *elf_sym_name(const elf_context *ctx, const Elf64_Sym *sym);

const char *elf_sym_type_str(u8 st_info);
const char *elf_sym_bind_str(u8 st_info);

#endif
