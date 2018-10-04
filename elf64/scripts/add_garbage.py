from pycool import *
import re
from math import ceil
from random import randint

args = parse('add to a gcc ASM file some garbage code',[
        ('file',{'help':'the gcc asm file'}),
        ('-l', '--labels', {'nargs':'+','help':'the labels (or function) where the scrip should add garbage', 'required':True}),
        ('-p', '--percent', {'help':'how much adding garbage codes, in percent, default is 50p (so if a function have 20 lines, it will add 10 lines)', 'default':50})
        ])
(lines, labels, p) = [ fget(args.file).split('\n'), dict(zip(args.labels,[True]*len(args.labels))), float(args.percent)/100.0 ]


###
# PART 1: detect label
###

def simple_parse(l):
	r = {'kind':'unknown'}

	j = re.search(r'(.*):.*', l)
	if j:
		name = j.group(1)
		kind = 'label' if not name.startswith('.') else 'sublabel'
		r = {'kind':kind, 'name':name}
	return r

zones = [] #[(l,r), ...]
l,r = -1,-1
i = 0
for line in lines:
	x = simple_parse(line.strip())
	if l >= 0 and (x['kind'] == 'label' or i + 1 == len(lines)):
		r = i
		zones.append((l+1,r-1))
		l,r = -1,-1
	if x['kind'] is 'label' and x['name'] in labels:
		l = i
	i += 1

if len(zones) == 0:
	print("Warning: No labels found, exit")
	exit()


###
# PART 2: add garbage codes
###

bef = '\t'
aft = '\t;junk'
garbage_inst_samples = [
	'nop',
	'cmp rax, rax',
]

off = 0
for z in zones:
	(l,r) = z
	to_add = int(ceil( r-l )*p)
	for offl in range(to_add):
		pos = randint(off + l, off + offl + r)
		choosed = randint(0, len(garbage_inst_samples)-1)
		lines.insert(pos, bef + garbage_inst_samples[choosed] + aft)
	off += offl

fput(args.file, '\n'.join(lines))
print("Success! added garbage ({} lines) to \"{}\"".format(off, args.file))
