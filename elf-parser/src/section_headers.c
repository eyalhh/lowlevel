#include <elf.h>
#include <elfparser/section_headers.h>
#include <elfparser/colors.h>
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

const char *safe_print_str(const char *s) {
    if (s == NULL) return "<null>";
    if (s[0] == 0) return "<null>";
    return s;
}

void print_indented_index(u32 index) {
    char result[10] = {0};
    snprintf(result, 9, "%u", index);
    printf("%-2s", result);
}
void shdrs_row_print(const elf_context *ctx, const Elf64_Shdr *sh, u32 index) {
    print_indented_index(index);
    printf(" ");
    printf("%-18s", safe_print_str(elf_shdr_name(ctx, sh)));
    printf(" ");
    printf("%-12s", elf_shdr_type_str(sh->sh_type));

    printf("\n");

}

void elf_shdrs_print(const elf_context *ctx) {
    printf("%s%-2s %-18s %-12s %8s %12s %14s %5s%s\n", c_dim(), "#", "name", "type", "addr", "offset", "size", "flg", c_reset());
    printf("%s", c_dim());
    for (u32 i = 0; i < 77; i++) {
        printf("\u2500");
    }
    printf("%s", c_reset());
    printf("\n");

    for (u32 i = 0; i < ctx->shnum; i++) {
        shdrs_row_print(ctx, (const Elf64_Shdr *)&ctx->shdrs[i], i);
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

const char *elf_shdr_type_str(u32 sh_type) {
    switch(sh_type) {
        case SHT_NULL:
        return "NULL";
        case SHT_PROGBITS:
        return "PROGBITS";
        case SHT_SYMTAB:
        return "SYMTAB";
        case SHT_STRTAB:
        return "STRTAB";
        case SHT_RELA:
        return "RELA";
        case SHT_HASH:
        return "HASH";
        case SHT_DYNAMIC:
        return "DYNAMIC";
        case SHT_NOTE:
        return "NOTE";
        case SHT_NOBITS:
        return "NOBITS";
        case SHT_REL:
        return "REL";
        case SHT_SHLIB:
        return "SHLIB";
        case SHT_DYNSYM:
        return "DYNSYM";
        case SHT_INIT_ARRAY:
        return "INIT_ARRAY";
        case SHT_FINI_ARRAY:
        return "FINI_ARRAY";
        case SHT_PREINIT_ARRAY:
        return "PREINIT_ARRAY";
        case SHT_GROUP:
        return "GROUP";
        case SHT_SYMTAB_SHNDX:
        return "SYMTAB_SHNDX";
        case SHT_RELR:
        return "RELR";
        default:
        return "UNKNOWN";



    }
}
#define SHT_NULL	  0		/* Section header table entry unused */
#define SHT_PROGBITS	  1		/* Program data */
#define SHT_SYMTAB	  2		/* Symbol table */
#define SHT_STRTAB	  3		/* String table */
#define SHT_RELA	  4		/* Relocation entries with addends */
#define SHT_HASH	  5		/* Symbol hash table */
#define SHT_DYNAMIC	  6		/* Dynamic linking information */
#define SHT_NOTE	  7		/* Notes */
#define SHT_NOBITS	  8		/* Program space with no data (bss) */
#define SHT_REL		  9		/* Relocation entries, no addends */
#define SHT_SHLIB	  10		/* Reserved */
#define SHT_DYNSYM	  11		/* Dynamic linker symbol table */
#define SHT_INIT_ARRAY	  14		/* Array of constructors */
#define SHT_FINI_ARRAY	  15		/* Array of destructors */
#define SHT_PREINIT_ARRAY 16		/* Array of pre-constructors */
#define SHT_GROUP	  17		/* Section group */
#define SHT_SYMTAB_SHNDX  18		/* Extended section indices */
#define SHT_RELR	  19            /* RELR relative relocations */
#define	SHT_NUM		  20		/* Number of defined types.  */
