#include <elfparser/elf_file.h>
#include <elfparser/elf_header.h>
#include <elfparser/program_headers.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void raise_for_result(elf_result result) {
    if (result != ELF_OK) {
        switch(result) {
            case ELF_ERR_MAGIC:
            fprintf(stderr, "malformed magic bytes\n");
            exit(EXIT_FAILURE);
            case ELF_ERR_MACHINE:
            fprintf(stderr, "unsupported machine type (currently we only support x86_64)\n");
            exit(EXIT_FAILURE);
            case ELF_ERR_TYPE:
            fprintf(stderr, "unsupported elf type (current only exec, fixed address binary static, no pie)\n");
            exit(EXIT_FAILURE);
            default:
            fprintf(stderr, "error accoured\n");
            exit(EXIT_FAILURE);

        }
    }
}


int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s {file_name}\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const char *file_name = argv[1];

    elf_context ctx; // init the context on the stack
    memset((void *)&ctx, 0, sizeof(ctx)); // memset to 0
    elf_result result = elf_open(file_name, &ctx); // call elf_open
    raise_for_result(result);
    result = elf_phdrs_parse(&ctx);
    raise_for_result(result);
    elf_header_print(&ctx);
    elf_close(&ctx);

    return 0;

}
