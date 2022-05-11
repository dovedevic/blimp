import sys
import os

row_size = int(sys.argv[1])
regions = int(sys.argv[2])
size = int(sys.argv[3])

data_regions = [f"data_{chr(ord('a') + r)}" for r in range(regions)]
data_values = [str(b) for b in sys.argv[4:]]
data_values += ["0x00" for _ in range(regions - len(data_values))]

memory_region = ""
for region in range(regions):
  region_size = 0
  memory_region += data_regions[region] + ":\n"
  while region_size != size:
    if region_size + row_size > size:
      # pad
      padded = [data_values[region]] * (size - region_size)
      padded += ["0x00"] * (row_size - len(padded))
      memory_region += "    .byte " + ", ".join(padded) + "\n"
      break
    else:
      region_size += row_size
      memory_region += "    .byte " + ", ".join([data_values[region]] * row_size) + "\n"

if os.name == 'nt':
  import pyperclip
  pyperclip.copy(memory_region)
else:
  print(memory_region)
