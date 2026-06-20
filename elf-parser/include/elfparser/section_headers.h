#ifndef ELFPARSER_SECTION_HEADERS_H
#define ELFPARSER_SECTION_HEADERS_H

#include <elf.h>
#include <mytypes.h>
#include "elf_file.h"

elf_result elf_shdrs_parse(elf_context *ctx);

void elf_shdrs_print(const elf_context *ctx);

const char *elf_shdr_name(const elf_context *ctx, const Elf64_Shdr* sh);

const Elf64_Shdr *elf_shdr_find(const elf_context *ctx, const char *name);

const char *elf_shdr_type_str(u32 sh_type);

#endif
