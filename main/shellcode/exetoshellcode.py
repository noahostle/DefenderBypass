import donut
import os
cdir=os.getcwd()
if "shellcode" not in cdir.split("/")[-1]: cdir=os.path.join(cdir,"shellcode")
execs = os.path.join(cdir,"executables/")
for fn in os.listdir(execs):
    shellcode = donut.create(file=os.path.join(execs, fn))
    os.system(f"mv {cdir}/../loader.bin {cdir}/loaders/{fn.split('.')[0]}.bin")
