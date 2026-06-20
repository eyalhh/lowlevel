#ifndef ELFPARSER_ELF_FILE_H
#define ELFPARSER_ELF_FILE_H

#include <elf.h>
#include <mytypes.h>

typedef struct {
    const u8 *base;
    usize size;

    /* elf headers */
    const Elf64_Ehdr *ehdr;
    b8 is_64; // for now its always true

    /* program headers */
    const Elf64_Phdr *phdrs;
    usize phnum;

    /* section headers */
    const Elf64_Shdr *shdrs;
    usize shnum;

    /* section names (string table) */
    const char *shstrtab;
    usize shstrtab_size;

    /* symbol table (.symtab) + string values (.strtab) */
    const Elf64_Sym *symbols;
    usize sym_count;
    const char *strtab;
    usize strtab_size;


} elf_context;


typedef enum {
    ELF_OK = 0,
    ELF_ERR_OPEN,
    ELF_ERR_TRUNCATED,
    ELF_ERR_MAGIC,
    ELF_ERR_MACHINE,
    ELF_ERR_TYPE,
    ELF_ERR_RANGE,
    ELF_ERR_CTX,
} elf_result;


elf_result elf_open(const char *path, elf_context *ctx);

void elf_close(elf_context *ctx);

const char *elf_strerror(elf_result r);


#endif
