# REQUIRES: x86, shell

# RUN: rm -rf %t.dir
# RUN: mkdir -p %t.dir/build
# RUN: llvm-mc -filetype=obj -triple=x86_64-unknown-linux %s -o %t.dir/build/foo.o
# RUN: echo "INPUT(\"%t.dir/build/foo.o\")" > %t.dir/build/foo.script
# RUN: cd %t.dir
# RUN: ld.lld build/foo.script -o bar --reproduce repro
# RUN: cpio -id < repro.cpio
# RUN: diff build/foo.script repro/%:t.dir/build/foo.script
# RUN: diff build/foo.o repro/%:t.dir/build/foo.o

.globl _start
_start:
  mov $60, %rax
  mov $42, %rdi
  syscall
