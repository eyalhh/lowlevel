#ifndef ELFPARSER_ELF_HEADER_H
#define ELFPARSER_ELF_HEADER_H

#include <mytypes.h>
#include <elfparser/elf_file.h>

void elf_header_print(const elf_context *ctx);

const char *elf_type_str(u16 e_type);
const char *elf_machine_str(u16 e_machine);

const char *elf_class_str(const elf_context *ctx);

const char *elf_data_str(const elf_context *ctx);

const char *elf_abi_str(const elf_context *ctx);


#endif
