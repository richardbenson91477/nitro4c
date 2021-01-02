#!/usr/bin/env python3
import os, sys

begin_tag = '// enums_map_begin\n'
end_tag = '// enums_map_end\n'

def main ():
    if len(sys.argv) < 2:
        print ("usage: " + sys.argv [0] + " [files]")
        return -1

    data_s = sys.argv [1]

    f_in = open (data_s + '.h', 'r')
    if not f_in:
        return -2

    f_out = open (data_s + '_enums_map.h', 'w')
    if not f_out:
        return -3

    lines = f_in.readlines ()
    f_in.close ()

    if not lines.count (begin_tag):
        return -4
    elif not lines.count (end_tag):
        return -5

    f_out.write ('enums_map _' + data_s + '_enums_map [] = {\n');

    e_id1 = lines.index (begin_tag) + 1
    e_id2 = lines.index (end_tag)
    for _enum in lines [e_id1 : e_id2]:
        if -1 != _enum.rfind ('='):
            split = _enum.split ('=')
            _id = int(split [1].rstrip (',\n'))
            _enum = split [0].strip()
        else:
            _id += 1
            _enum = _enum.strip().rstrip (',\n')

        f_out.write ('    {\"' + _enum + '\", ' + str(_id) + '},\n')

    f_out.write ('    {NULL, 0}};\n\n')

if __name__ == '__main__':
    res = main()
    if res:
        print("error " + str(res))
    sys.exit (res)

