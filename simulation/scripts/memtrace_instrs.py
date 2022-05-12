#!/usr/bin/env python3
import sys
import re

if len(sys.argv) <= 1:
  print(f"Usage: {sys.argv[0]} [riscvOVPsim .trace file]")
  exit()

mem_trace = ""
i = 0
with open(sys.argv[1], "r") as fp:
  for line in fp:
    match = re.match(r"Info[ ]+(MEM(R|W) 0[xX][0-9a-fA-F]+ 0[xX][0-9a-fA-F]+ [0-9a-fA-F]+ [0-9a-fA-F]+)\n", line)
    if not match:
      continue
    operation = match.groups()[0]
    is_read = "MEMR" in operation
    address = operation.split(' ')[-1]
    
    mem_trace += "0x" + str(address) + (" P_MEM_RD " if is_read else " P_MEM_WR ") + str(i) + "\n"
    i += 1

with open(sys.argv[1] + ".memtrace", "w") as wp:
  wp.write(mem_trace)
# print(mem_trace)
