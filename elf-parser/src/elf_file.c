#include <elf.h>
#include <elfparser/elf_file.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

elf_result elf_open(const char *path, elf_context *ctx) {


    FILE *fp = fopen(path, "rb");
    if (fp == NULL) return ELF_ERR_OPEN;

    fseek(fp, 0, SEEK_END);
    i32 size = ftell(fp);
    if (size < 0) return ELF_ERR_OPEN;
    rewind(fp);

    void *ptr = malloc((usize)size);
    if (ptr == NULL) return ELF_ERR_OPEN;

    i32 bytes_read = fread(ptr, 1, size, fp);
    if (bytes_read < size) return ELF_ERR_OPEN;


    fclose(fp);
    

    ctx->base = ptr;
    ctx->size = (size_t)size;

    ctx->ehdr = (const Elf64_Ehdr*)ptr;




    if (ctx->size < sizeof(Elf64_Ehdr)) return ELF_ERR_RANGE;
    i32 result = memcmp((const void *)ctx->ehdr->e_ident, ELFMAG, SELFMAG);
    if (result) return ELF_ERR_MAGIC;

    // only supporting 64 bit for now
    if (ctx->ehdr->e_ident[EI_CLASS] != ELFCLASS64) return ELF_ERR_MACHINE;
    // only supporting little endian for now
    if (ctx->ehdr->e_ident[EI_DATA] != ELFDATA2LSB) return ELF_ERR_MAGIC;
    // only supporting SystemV/Unix-Gnu
    u8 abi = ctx->ehdr->e_ident[EI_OSABI];
    if (abi != ELFOSABI_SYSV && abi != ELFOSABI_GNU) return ELF_ERR_MAGIC;
    // only supporting current version
    if (ctx->ehdr->e_ident[EI_VERSION] != EV_CURRENT) return ELF_ERR_MAGIC;
    // make sure all padding is 0
    usize i = EI_PAD - 1;
    for (; i < EI_NIDENT; i++) {
        if (ctx->ehdr->e_ident[i] != 0) return ELF_ERR_MAGIC;
    }
    // only supporting EXEC for now (nopie)
    if (ctx->ehdr->e_type != ET_EXEC) return ELF_ERR_TYPE;
    

    return ELF_OK;
}

void elf_close(elf_context *ctx) {
    free((void*)ctx->base); // free the base pointer
    memset((void*)ctx, 0, sizeof(elf_context)); // set all context to 0
}
