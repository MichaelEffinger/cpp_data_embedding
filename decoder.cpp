#include <array>
#include <algorithm>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <print>
#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>

#include <fcntl.h>
#include <unistd.h>
#include <elf.h>

#include "decoder_strings.h"

constexpr std::byte ParseKeyPartImple(const auto& arr, std::string_view& str) {
    auto cond = [str](std::string_view s) { return str.starts_with(s); };
        
    /* Search for a matching string. */
    auto iter = std::ranges::find_if(arr, cond);

    /* Calculate what index it is. */
    auto num = iter - arr.cbegin();

    if (num == arr.size())
        throw std::invalid_argument("invalid key piece");

    /* Skip ahead in the string. */
    str = str.data() + iter->length();

    return static_cast<std::byte>(num);
}

struct KeyInfo {
    int order;
    std::array<std::byte, 3> data;

    constexpr KeyInfo(std::string_view key) {
        int ordering;
        try {
            ordering = int(ParseKeyPartImple(g_ordering_prefix, key));
        }
        catch (std::invalid_argument& excpt) {
            ordering = -1;
        }

        /* Determine proper ordering. */
        bool special_do = false;
        if (ordering == -1)
            ordering = 8;
        else if (ordering == 4)
            special_do = true;
        else if (ordering >= 5)
            ordering += 1;
        
        /* Type prefix and coolness prefix form the first byte. */
        {
            /* Find the type prefix for the first nyble. */
            std::byte n1 = ParseKeyPartImple(g_type_prefix, key);

            /* Coolness prefix for second nyble. */
            std::byte n2 = ParseKeyPartImple(g_coolness_prefix, key);

            data[0] = (n1 << 4) | n2;
        }

        /* Unit strings form the second byte. */
        {
            /* Find the first unit for the first nyble. */
            std::byte n1 = ParseKeyPartImple(g_first_unit_strings, key);

            /* We also need to check the direction at this point. */
            //ordering += int(ParseKeyPartImple(g_direction_strings, key)) * 8;

            /* Second unit for second nyble. */
            std::byte n2 = ParseKeyPartImple(g_second_unit_strings, key);

            data[1] = (n1 << 4) | n2;
        }

        /* Verb and coolness suffix form the third byte. */
        {
            /* Find the verb for the first nyble. */
            std::byte n1 = ParseKeyPartImple(g_verb_strings, key);

            /* Coolness suffix for second nyble. */
            std::byte n2 = ParseKeyPartImple(g_coolness_suffix, key);

            data[2] = (n1 << 4) | n2;
        }

        /* If this was a do function, check if it ends with 'to' or 'from'. */
        /* to is earlier in order than from. */
        if (special_do)
            ordering += int(ParseKeyPartImple(g_direction_strings, key));

        /* Assign our order value. */
        this->order = ordering;
    }
}; // struct KeyInfo

void PrintUsage(const char* name) {
    std::print("{} <executable_path>\n", name);
    std::exit(-1);
}

int main(int argc, char** argv) {
    int result;

    //if (argc != 2 || !std::string_view("--help").compare(argv[1]))
    //    PrintUsage(argv[0]);
    
    /* Open target executable. */
    FILE* exec_fd = fopen("test", "rb");
    if (exec_fd == NULL) {
        perror("failed to open target file");
        return -1;
    }

    /* Read in the ELF header. */
    Elf64_Ehdr ehdr;
    result = fread(&ehdr, sizeof(ehdr), 1, exec_fd);
    if (result == 0) {
        perror("failed to read header");
        return -1;
    }

    /* Make sure this is a 64bit elf. */
    if (ehdr.e_ident[EI_CLASS] != ELFCLASS64) {
        std::print("only 64bit ELFs are supported but a 32bit ELF was provided\n");
        return -1;
    }

    /* Check if we have section headers. */
    if (ehdr.e_phoff == 0) {
        std::print("target executable has no section headers\n");
        return -1;
    }

    /* Log section name sect info. */
    std::print("section headers offset: 0x{:X}\n", ehdr.e_shoff);
    std::print("section header size:    0x{:X}\n", ehdr.e_shentsize);
    std::print("section string table section index: 0x{:X}\n\n", ehdr.e_shstrndx);

    /* Read the section string table section. */
    Elf64_Shdr sect_str_shdr;
    fseek(exec_fd, ehdr.e_shoff + ehdr.e_shstrndx * ehdr.e_shentsize, SEEK_SET);
    result = fread(&sect_str_shdr, std::min(sizeof(sect_str_shdr), static_cast<size_t>(ehdr.e_shentsize)), 1, exec_fd);
    if (result == 0) {
        perror("failed to read section string table section header");
        return -1;
    }

    /* Log some stuffs. */
    std::print("section name string table section size:   0x{:X}\n", sect_str_shdr.sh_size);
    std::print("section name string table section offset: 0x{:X}\n", sect_str_shdr.sh_offset);

    /* Read the entire section name string table. */
    std::vector<char> sect_str_tab(sect_str_shdr.sh_size);
    fseek(exec_fd, sect_str_shdr.sh_offset, SEEK_SET);
    result = fread(sect_str_tab.data(), 1, sect_str_tab.size(), exec_fd);
    if (result == 0) {
        perror("failed to read section string table");
        return -1;
    }

    /* Scan through each section header, searching for ".strtab" section. */
    Elf64_Shdr symtab_shdr;
    bool found = false;
    fseek(exec_fd, ehdr.e_shoff, SEEK_SET);
    for (int i = 0; i < ehdr.e_shnum; i++) {
        /* Read in a header. */
        result = fread(&symtab_shdr, 0x40, 1, exec_fd);
        if (result == 0) {
            std::print("failed to read section header {}: {}\n", i, std::strerror(errno));
            return -1;
        }

        /* Check that we're in bounds. */
        // TODO

        std::print("\t{} ({:X}; {:X})\n", &sect_str_tab[symtab_shdr.sh_name], symtab_shdr.sh_offset, symtab_shdr.sh_size);

        if (!std::string_view(".strtab").compare(&sect_str_tab.at(symtab_shdr.sh_name))) {
            std::print("found symbol table section at index {}\n", i);
            found = true;
            break;
        }
    }

    std::print("symbol table section offset: 0x{:X}\n", symtab_shdr.sh_offset);
    std::print("symbol table section size:   0x{:X}\n", symtab_shdr.sh_size);

    if (!found) {
        std::print("failed to find .symtab section\n");
        return -1;
    }

    /* Read the entire symbol table. */
    std::vector<char> sym_table(symtab_shdr.sh_size);
    fseek(exec_fd, symtab_shdr.sh_offset, SEEK_SET);
    result = fread(sym_table.data(), 1, sym_table.size(), exec_fd);
    if (result == 0) {
        perror("failed to read entire string table");
        return -1;
    }

    std::array<std::byte, 3 * 16> data;
    std::ranges::fill(data, std::byte{});
    size_t sym_table_off = 0;
    while (true) {
        /* This is a disgusting hack, but it works. */
        /* Instead of properly parsing mangled names we're just gonna
         * bruteforce decoding each valid name present within a mangled
         * symbol */
        /* Try to decode it as a name..? */
        while (true) {
            /* Skip until we reach a number or the end. */
            char cur_ch;
            while (cur_ch = sym_table[sym_table_off], !std::isdigit(cur_ch) && cur_ch != '\00') {
                sym_table_off++;
                if (sym_table_off >= sym_table.size())
                    break;
            }

            if (cur_ch == '\00') {
                sym_table_off++;
                break;
            }

            //if (sym_table_off >= 0xDF3B)
            //    std::print("here\n");

            /* Determine current name length. */
            int len = 0;
            while (cur_ch = sym_table[sym_table_off], std::isdigit(cur_ch)) {
                len *= 10;
                len += cur_ch - '0';
                sym_table_off++;
            }
            if (len < 0 || len > 100)
                continue;
            assert(len >= 0);
            
            try {
                /* Get substring. */
                auto subst = std::string_view(sym_table.data() + sym_table_off);
                subst = subst.substr(0, std::min(subst.length(), static_cast<size_t>(len)));

                /* Try to parse as a key. */
                KeyInfo key(subst);

                std::print("key found: {}\n", subst);
                std::print("\torder:  {}\n\tbytes:  ", key.order);
                for (auto b : key.data) std::print("{:02X} ", int(b));
                std::print("\n\toffset: 0x{:X}\n", sym_table_off);

                std::ranges::copy(key.data, data.begin() + key.order * 3);
            }
            catch (std::invalid_argument& excpt) {
                /* Do nothing. */
                /* ... */
            }

            /* Increment offset to the next string part. */
            sym_table_off += len;
        }

        /* Check if we're at the end. */
        if (sym_table_off >= symtab_shdr.sh_size)
            break;
    }

    /* Print out decoded data. */
    for (int i = 0; i < data.size(); i++) {
        std::print("{:02X} ", char(data[i]));
        if (i != 0 && i % 16 == 15)
            std::print("\n");
    }

}
