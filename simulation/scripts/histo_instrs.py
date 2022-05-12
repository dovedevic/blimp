#!/usr/bin/env python3
import json
import sys
import re

if len(sys.argv) <= 1:
  print(f"Usage: {sys.argv[0]} [riscvOVPsim .trace file]")
  exit()

instrs = dict()
with open(sys.argv[1], "r") as fp:
  for line in fp:
    match = re.match(r"Info 'riscvOVPsim\/cpu', 0[xX][0-9a-fA-F]+\(.*\): [0-9a-fA-F]+\s+(.*)\n", line)
    if not match:
      continue
    instruction = match.groups()[0]
    opcode = instruction.split(' ')[0]
    
    if opcode not in instrs:
      instrs[opcode] = 0
    instrs[opcode] += 1

instrs = dict(sorted(instrs.items(), key=lambda item: item[1], reverse=True))

with open(sys.argv[1] + ".histo", "w") as wp:
  json.dump(instrs, wp, indent=2)
for instr in instrs:
  print(f"{instr}\t{instrs[instr]}")
