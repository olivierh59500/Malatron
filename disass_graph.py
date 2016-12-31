import capstone
import pefile
import os

def split_functions(file_path):
    pe = pefile.PE(file_path)
    entry = pe.OPTIONAL_HEADER.AddressOfEntryPoint
    code_section = pe.get_section_by_rva(entry)
    data = code_section.get_data()
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    func_sig = '\x55\x89\xE5' # push ebp and mov ebp, esp. should deet most functions (prob all)
    pos = 0
    start_addr = code_section.VirtualAddress
    funcs = []
    while True:
        pos = data.find(func_sig, pos)
        if pos == -1:
            break

        func = []
        for i in md.disasm(data[pos:], start_addr + pos):
            addr = int(i.address)
            opcode = str(i.mnemonic)
            func.append((addr, opcode, i.op_str))

        funcs.append(func)
        pos += len(func_sig)

if __name__ == '__main__':
    # Gu's love
