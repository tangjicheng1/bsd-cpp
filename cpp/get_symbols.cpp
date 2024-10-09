#include <cxxabi.h>
#include <elf.h>
#include <fcntl.h>
#include <gelf.h>
#include <libelf.h>
#include <unistd.h>
#include <iostream>
#include <vector>

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

    size_t num_symbols = 0;
    Elf_Scn* section = NULL;

    // Iterate over sections to find symbol tables
    while ((section = elf_nextscn(elf, section)) != NULL) {
        GElf_Shdr shdr;
        gelf_getshdr(section, &shdr);

        if (shdr.sh_type == SHT_SYMTAB || shdr.sh_type == SHT_DYNSYM) {
            Elf_Data* data = elf_getdata(section, NULL);
            num_symbols = shdr.sh_size / shdr.sh_entsize;

            for (size_t i = 0; i < num_symbols; ++i) {
                GElf_Sym sym;
                gelf_getsym(data, i, &sym);

                // Get symbol name
                char* symbol_name = elf_strptr(elf, shdr.sh_link, sym.st_name);
                if (symbol_name) {
                    // Perform name demangling
                    int status;
                    char* demangled_name = abi::__cxa_demangle(symbol_name, NULL, NULL, &status);
                    if (status == 0) {
                        std::cout << demangled_name << std::endl;
                        free(demangled_name);  // Free the demangled name after use
                    } else {
                        std::cout << "mangling failed: " << symbol_name << std::endl;
                    }
                }
            }
        }
    }

    elf_end(elf);
    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <binary_file>" << std::endl;
        return 1;
    }

    print_symbols(argv[1]);
    return 0;
}
