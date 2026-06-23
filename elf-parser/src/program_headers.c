#include "elfparser/elf_file.h"
#include <elf.h>
#include <stdio.h>
#include <elfparser/program_headers.h>
#include <elfparser/elf_header.h>
#include <elfparser/colors.h>
#include <mytypes.h>



elf_result elf_phdrs_parse(elf_context *ctx) {

    u64 phoff = ctx->ehdr->e_phoff;
    usize phnum = ctx->ehdr->e_phnum;
    usize phentsize = ctx->ehdr->e_phentsize;
    
    // check that phentsize is exactly sizeof Elf64_Phdr
    if (phentsize != sizeof(Elf64_Phdr)) return ELF_ERR_RANGE;

    if (phnum == 0) {
        ctx->phdrs = NULL;
        ctx->phnum = 0;
        return ELF_OK;
    }

    if ((usize)phoff > ctx->size) return ELF_ERR_RANGE;


    if ( phnum * phentsize > ctx->size - phoff ) return ELF_ERR_RANGE; 

    ctx->phdrs = (const Elf64_Phdr *)(ctx->base + phoff);
    ctx->phnum = phnum;



    return ELF_OK;
}

void ph_perm_print(u32 p_flags) {
    char buffer[4] = {'-', '-', '-', 0};
    if (p_flags & PF_R) buffer[0] = 'r';
    if (p_flags & PF_W) buffer[1] = 'w';
    if (p_flags & PF_X) buffer[2] = 'x';
    printf("%s%c%s%s%c%s%s%c%s   ", c_green(), buffer[0], c_reset(), c_yellow(), buffer[1], c_reset(), c_red(), buffer[2], c_reset());

}

void ph_print_size(u64 size, b8 align) {
    double frac = (double)size;
    char *buffer[4] = {"B", "KB", "MB", "GB"};
    u32 counter = 0;
    while (frac > 1000) {
        counter+=1;
        frac /= 1000;
    }
    char result[0x20] = {0};
    snprintf(result, 0x1f, frac < 10 && counter != 0 && !align ? "%.1f %s": "%.0f %s", frac, buffer[counter]);
    printf(align ? "%6s" : "%10s ", result);
    return;
}
void ph_row_print(const Elf64_Phdr* ph) {
    const char *type_name = elf_phdr_type_str(ph->p_type);
    printf("%s%-14s%s ", c_cyan(), type_name, c_reset());
    ph_perm_print(ph->p_flags);
    printf("   0x%06" PRIx32 " ", (u32)ph->p_offset);
    printf("    0x%06" PRIx64 " ", (u64)ph->p_vaddr);
    ph_print_size(ph->p_filesz, false);
    ph_print_size(ph->p_memsz, false);
    ph_print_size(ph->p_align, true);
    printf("\n");


}
void elf_phdrs_print(const elf_context *ctx) {
    printf("Elf file type is %s\n", elf_type_str(ctx->ehdr->e_type));
    printf("Entry point %" PRIx64 "\n", ctx->ehdr->e_entry);
    printf("There are %" PRIu16 " program headers, starting at offset %" PRIu64 "\n\n", ctx->ehdr->e_phnum, ctx->ehdr->e_phoff);

    // print the columns of the table
    printf("%s%-14s %-6s %10s %12s %10s %10s %6s %s\n", c_dim(), "type", "perm", "offset", "vaddr", "filesz", "memsz", "align", c_reset());
    printf("%s", c_dim());
    for (int i = 0; i < 74; i++) fputs("\u2500", stdout);
    printf("%s\n", c_reset());

    // print the table itself
    for (int i = 0; i < ctx->ehdr->e_phnum; i++) {
        ph_row_print(&ctx->phdrs[i]);
    }



}

const char *elf_phdr_type_str(u32 p_type) {
    switch(p_type) {
        case PT_NULL:
        return "NULL";
        case PT_LOAD:
        return "LOAD";
        case PT_DYNAMIC:
        return "DYNAMIC";
        case PT_INTERP:
        return "INTERP";
        case PT_NOTE:
        return "NOTE";
        case PT_SHLIB:
        return "SHLIB";
        case PT_PHDR:
        return "PHDR";
        case PT_TLS:
        return "TLS";
        case PT_GNU_EH_FRAME:
        return "GNU_EH_FRAME";
        case PT_GNU_STACK:
        return "GNU_STACK";
        case PT_GNU_RELRO:
        return "GNU_RELRO";
        case PT_GNU_PROPERTY:
        return "GNU_PROPERTY";
        case PT_GNU_SFRAME:
        return "GNU_SFRAME";
        default:
        return "UNKNOWN";

    }

}
