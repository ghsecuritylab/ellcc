# REQUIRES: x86
# RUN: llvm-mc -filetype=obj -triple=x86_64-unknown-linux %s -o %t
# RUN: echo "PHDRS {all PT_LOAD FILEHDR PHDRS ;} \
# RUN:       SECTIONS { \
# RUN:           . = 0x10000200; \
# RUN:           .text : {*(.text*)} :all \
# RUN:           .foo : {*(.foo.*)} :all \
# RUN:           .data : {*(.data.*)} :all}" > %t.script
# RUN: ld.lld -o %t1 --script %t.script %t
# RUN: llvm-readobj -program-headers %t1 | FileCheck %s

## Check the AT(expr)
# RUN: echo "PHDRS {all PT_LOAD FILEHDR PHDRS AT(0x500 + 0x500) ;} \
# RUN:       SECTIONS { \
# RUN:           . = 0x10000200; \
# RUN:           .text : {*(.text*)} :all \
# RUN:           .foo : {*(.foo.*)} :all \
# RUN:           .data : {*(.data.*)} :all}" > %t.script
# RUN: ld.lld -o %t1 --script %t.script %t
# RUN: llvm-readobj -program-headers %t1 | FileCheck --check-prefix=AT %s

# CHECK:     ProgramHeaders [
# CHECK-NEXT:  ProgramHeader {
# CHECK-NEXT:    Type: PT_LOAD (0x1)
# CHECK-NEXT:    Offset: 0x0
# CHECK-NEXT:    VirtualAddress: 0x10000000
# CHECK-NEXT:    PhysicalAddress: 0x10000000
# CHECK-NEXT:    FileSize: 521
# CHECK-NEXT:    MemSize: 521
# CHECK-NEXT:    Flags [ (0x7)
# CHECK-NEXT:      PF_R (0x4)
# CHECK-NEXT:      PF_W (0x2)
# CHECK-NEXT:      PF_X (0x1)
# CHECK-NEXT:    ]

# AT:       ProgramHeaders [
# AT-NEXT:    ProgramHeader {
# AT-NEXT:      Type: PT_LOAD (0x1)
# AT-NEXT:      Offset: 0x0
# AT-NEXT:      VirtualAddress: 0x10000000
# AT-NEXT:      PhysicalAddress: 0xA00
# AT-NEXT:      FileSize: 521
# AT-NEXT:      MemSize: 521
# AT-NEXT:      Flags [ (0x7)
# AT-NEXT:        PF_R (0x4)
# AT-NEXT:        PF_W (0x2)
# AT-NEXT:        PF_X (0x1)
# AT-NEXT:      ]

.global _start
_start:
 nop

.section .foo.1,"a"
foo1:
 .long 0

.section .foo.2,"aw"
foo2:
 .long 0
