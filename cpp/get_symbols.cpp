#include <iostream>
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <libelf.h>
#include <gelf.h>

void print_symbols(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    if (elf_version(EV_CURRENT) == EV_NONE) {
        std::cerr << "libelf initialization failed" << std::endl;
        close(fd);
        return;
    }

    Elf* elf = elf_begin(fd, ELF_C_READ, NULL);
    if (!elf) {
        std::cerr << "elf_begin failed" << std::endl;
        close(fd);
        return;
    }

    size_t shnum;
    elf_getshnum(elf, &shnum);

    for (size_t i = 0; i < shnum; ++i) {
        Elf_Scn* scn = elf_getscn(elf, i);
        if (!scn) continue;

        GElf_Shdr shdr;
        gelf_getshdr(scn, &shdr);

        if (shdr.sh_type == SHT_SYMTAB || shdr.sh_type == SHT_DYNSYM) {
            Elf_Data* data = elf_getdata(scn, NULL);
            size_t sym_count = shdr.sh_size / shdr.sh_entsize;

            for (size_t j = 0; j < sym_count; ++j) {
                GElf_Sym sym;
                gelf_getsym(data, j, &sym);
                std::cout << "Symbol: " << elf_strptr(elf, shdr.sh_link, sym.st_name) << std::endl;
            }
        }
    }

    elf_end(elf);
    close(fd);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <binary_file>" << std::endl;
        return 1;
    }

    print_symbols(argv[1]);
    return 0;
}
