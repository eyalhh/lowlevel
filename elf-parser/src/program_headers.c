#include "elfparser/elf_file.h"
#include <elf.h>
#include <elfparser/program_headers.h>
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
