#include <elf.h>
#include <elfparser/elf_header.h>
#include <elfparser/colors.h>
#include <mytypes.h>
#include <stdio.h>


void elf_header_print(const elf_context *ctx) {
    printf("ELF Header:\n");
    printf("  Magic:   ");
    for (u32 i = 0; i < EI_NIDENT; i++) {
        printf("%s%02x%s ", c_yellow(), (unsigned char)ctx->ehdr->e_ident[i], c_reset());
    }
    printf("\n");
    printf("  Class:                             %s\n", elf_class_str(ctx));
    printf("  Data:                              %s\n", elf_data_str(ctx));
    printf("  Version:                           %d (current)\n", EV_CURRENT);
    printf("  OS/ABI:                            %s\n", elf_abi_str(ctx));
    printf("  ABI Version:                       %s\n", "0");
    printf("  Type:                              %s\n", elf_type_str(ctx->ehdr->e_type));
    printf("  Machine:                           %s\n", elf_machine_str(ctx->ehdr->e_machine));
    printf("  Version:                           %#x\n", EV_CURRENT);
    printf("  Entry point address:               %s%p%s\n", c_green(), (void *)ctx->ehdr->e_entry, c_reset());
    printf("  Start of program headers:          %d (bytes into file)\n", (int)ctx->ehdr->e_phoff);
    printf("  Start of section headers:          %d (bytes into file)\n", (int)ctx->ehdr->e_shoff);
    printf("  Flags:                             0x%x\n", ctx->ehdr->e_flags);
    printf("  Size of this header:               %zu (bytes)\n", (usize)ctx->ehdr->e_ehsize);
    printf("  Size of program headers:           %zu (bytes)\n", (usize)ctx->ehdr->e_phentsize);
    printf("  Number of program headers:         %zu\n", (usize)ctx->ehdr->e_phnum);
    printf("  Size of section headers:           %zu (bytes)\n", (usize)ctx->ehdr->e_shentsize);
    printf("  Number of section headers:         %zu\n", (usize)ctx->ehdr->e_shnum);
    printf("  Section header string table index: %hu\n", ctx->ehdr->e_shstrndx);

    
}

const char *elf_type_str(u16 e_type) {
    switch(e_type) {
        case ET_NONE:
        return "NONE (no file type)";
        case ET_REL:
        return "REL (Relocatable file)";
        case ET_EXEC:
        return "EXEC (Executable file)";
        case ET_DYN:
        return "DYN (Shared object file)";
        case ET_CORE:
        return "CORE (Core file)";
        default:
        return "UNKNOWN";
    }
}

const char *elf_machine_str(u16 e_machine) {
    switch(e_machine) {
        case EM_NONE:
        return "NONE";
        case EM_X86_64:
        return "Advanced Micro Devices X86-64";
        default:
        return "UNKNOWN";
    }
}

const char *elf_class_str(const elf_context *ctx) {
    u8 arch = (u8)ctx->ehdr->e_ident[EI_CLASS];
    if (arch == ELFCLASS32) return "ELF32";
    if (arch == ELFCLASS64) return "ELF64";
    return "UNKNOWN";
}

const char *elf_data_str(const elf_context *ctx) {
    u8 data_type = (u8)ctx->ehdr->e_ident[EI_DATA];
    if (data_type == ELFDATA2LSB) return "2's complement, little endian";
    if (data_type == ELFDATA2MSB) return "2's complement, big endian";
    return "UNKNOWN";

}

const char *elf_abi_str(const elf_context *ctx) {
    u8 abi = (u8)ctx->ehdr->e_ident[EI_OSABI];
    if (abi == ELFOSABI_SYSV) return "System V";
    if (abi == ELFOSABI_GNU) return "UNIX - GNU";
    return "UNKNOWN";
}

