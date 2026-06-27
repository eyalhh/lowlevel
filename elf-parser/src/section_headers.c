#include <elf.h>
#include <elfparser/section_headers.h>
#include <mytypes.h>
#include <string.h>
#include <stdio.h>

const char *safe_string_blob_access(const char *blob, u64 size, u64 offset) {
    if (offset >= size) return NULL;
    return (const char *)(blob+offset);
}
b8 validate_shdrs(const elf_context *ctx, u64 shoff, u64 shnum) {
    const Elf64_Shdr *shdrs = (const Elf64_Shdr*)(ctx->base + shoff);
    for (u32 i = 0; i < shnum; i++) {
        const Elf64_Shdr current = shdrs[i];
        if (current.sh_offset > ctx->size) return 0;
        if (current.sh_size > ctx->size - current.sh_offset) return 0;
        const char *name = elf_shdr_name(ctx, &current);
        if (name == NULL) {
            return 0;
        }
    }
    return 1;


}
elf_result elf_shdrs_parse(elf_context *ctx) {
    u64 shoff = ctx->ehdr->e_shoff;
    u64 shnum = ctx->ehdr->e_shnum;
    u64 shstrndx = ctx->ehdr->e_shstrndx;
    if (shoff > ctx->size) return ELF_ERR_RANGE;
    if (shnum * sizeof(Elf64_Shdr) > ctx->size - shoff) return ELF_ERR_RANGE;

    ctx->shdrs = (const Elf64_Shdr*)(ctx->base + shoff);
    ctx->shnum = shnum;
    // validate shstrtab 
    if ( shstrndx >= shnum ) return ELF_ERR_RANGE;
    Elf64_Shdr shstrtab_sh = ctx->shdrs[shstrndx];

    
    ctx->shstrtab = (const char *)(ctx->base + shstrtab_sh.sh_offset);
    ctx->shstrtab_size = shstrtab_sh.sh_size;

    b8 result = validate_shdrs(ctx, shoff, shnum);
    if (!result) return ELF_ERR_RANGE;

    return ELF_OK;
}

void elf_shdrs_print(const elf_context *ctx) {
    printf("Trying to find .symtab section\n");
    const Elf64_Shdr *shsymbol = elf_shdr_find(ctx, ".symtab");
    if (shsymbol == NULL) {
        printf("didn't find it!\n");
        return;
    }
    printf("Found it! Section name: %s, Offset into file: %" PRIu64 ", Size of section: %" PRIu64 "\n", elf_shdr_name(ctx, shsymbol), shsymbol->sh_offset, shsymbol->sh_size);
    printf("Data of section (hexdump):\n");
    const u8 *base_ptr = (const u8 *)(ctx->base+shsymbol->sh_offset);
    for (u64 i = 0; i < shsymbol->sh_size; i++) {
        printf("%02X ", base_ptr[i]);
    }
}

const char *elf_shdr_name(const elf_context *ctx, const Elf64_Shdr* sh) {
    return safe_string_blob_access(ctx->shstrtab, ctx->shstrtab_size, sh->sh_name);
}

const Elf64_Shdr *elf_shdr_find(const elf_context *ctx, const char *name) {
    for (u32 i = 0; i < ctx->shnum; i++) {
        const Elf64_Shdr *current = &ctx->shdrs[i];
         const char *current_name = elf_shdr_name(ctx, current);
        if (!strcmp(name, current_name)) {
            return current;
        }
    }
    return NULL;
}

const char *elf_shdr_type_str(u32 sh_type);
