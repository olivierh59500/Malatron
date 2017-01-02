#!/usr/bin/python

import os
import sys
import r2pipe

def get_functions(r2):
    afl = r2.cmd('afl')
    afl = afl.split('\n')
    funcs = []
    for line in afl:
        gu = line.split(' ')
        addr = int(gu[0], 16)
        name = gu[-1]
        funcs.append((addr, name))

    return funcs

def disass_funcs(r2, funcs):
    out = []
    for f in funcs:
        name = f[1]
        addr = f[0]
        disass = r2.cmd('pdr @ ' + name)
        out.append((addr, name, disass))

    return out

# full disass dump
if __name__ == '__main__':
    if len(sys.argv) < 2:
        raise # TODO: better error handling

    r2 = r2pipe.open(sys.argv[1])
    r2.cmd('aaa')
    r2.cmd('af')
    outfile = open(sys.argv[2], 'w')
    funcs = get_functions(r2)
    result = disass_funcs(r2, funcs)
    for func in result:
        outfile.write('Address ' + str(func[0]) + '\n')
        outfile.write('Function ' + func[1] + '\n')
        outfile.write(func[2] + '\n') # the disassembly
        outfile.write('DONE\n')
