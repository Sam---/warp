#!/usr/bin/python3
import sys, os, json

WARPFILE = os.path.expanduser(os.getenv("WARPFILE", default="~/.warp"))
USAGE = """Usage:
    warp {i}warp-point{z}
    warp [--set | -s] {i}warp-point{z} {i}directory{z}
    warp -d {i}warp-point{z}
""".format(i='\x1b[4m', z='\x1b[0m')

warpdata = None
if os.path.isfile(WARPFILE):
    with open(WARPFILE, 'r', encoding='utf-8') as warpfile:
        warpdata = json.load(warpfile)
else:
    warpdata = {}

if len(sys.argv) > 1:
    if sys.argv[1] in ['--set', '-s']:
        if len(sys.argv) > 3:
            warpdata[sys.argv[2]] = os.path.abspath(sys.argv[3])
            with open(WARPFILE, 'w', encoding='utf-8') as wwarpfile:
                json.dump(warpdata, wwarpfile)
            print('.')
        else:
            print("warp: Not enough arguments", file=sys.stderr)
            sys.stderr.write(USAGE)
            print('.')
    elif sys.argv[1] in ['-d', '--remove']:
        if len(sys.argv) > 2:
            if sys.argv[2] in warpdata:
                del warpdata[sys.argv[2]]
                with open(WARPFILE, 'w', encoding='utf-8') as wwarpfile:
                    json.dump(warpdata, wwarpfile)
            else:
                print("{wp}: No such warp point".format(wp=sys.argv[2]),
                        file=sys.stderr)
            print('.')
        else:
            print("warp: Not enough arguments", file=sys.stderr)
            print('.')
    else:
        if sys.argv[1] in warpdata:
            print(warpdata[sys.argv[1]])
        else:
            print("{wp}: No such warp point".format(wp=sys.argv[1]),
                    file=sys.stderr)
            print('.')
else:
    sys.stderr.write(USAGE)
    print('.')
