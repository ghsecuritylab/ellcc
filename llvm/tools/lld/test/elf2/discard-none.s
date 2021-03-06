// RUN: llvm-mc -filetype=obj -triple=x86_64-pc-linux -save-temp-labels %s -o %t
// RUN: ld.lld2 -discard-none -shared %t -o %t2
// RUN: llvm-readobj -s -sd -t %t2 | FileCheck %s
// REQUIRES: x86

.text
.Lmyvar:
.Lmyothervar:

// CHECK:   Section {
// CHECK:     Name: .strtab
// CHECK-NEXT:     Type: SHT_STRTAB
// CHECK-NEXT:     Flags [
// CHECK-NEXT:     ]
// CHECK-NEXT:     Address:
// CHECK-NEXT:     Offset:
// CHECK-NEXT:     Size:
// CHECK-NEXT:     Link:
// CHECK-NEXT:     Info:
// CHECK-NEXT:     AddressAlignment:
// CHECK-NEXT:     EntrySize:
// CHECK-NEXT:     SectionData (
// CHECK-NEXT:       0000: 002E4C6D 79766172 002E4C6D 796F7468  |..Lmyvar..Lmyoth|
// CHECK-NEXT:       0010: 65727661 7200                        |ervar.|
// CHECK-NEXT:     )
// CHECK-NEXT:   }

// CHECK:   Symbol {
// CHECK-NEXT:     Name:
// CHECK-NEXT:     Value: 0x0
// CHECK-NEXT:     Size: 0
// CHECK-NEXT:     Binding: Local
// CHECK-NEXT:     Type: None
// CHECK-NEXT:     Other: 0
// CHECK-NEXT:     Section: Undefined
// CHECK-NEXT:   }
// CHECK-NEXT:   Symbol {
// CHECK-NEXT:     Name: .Lmyothervar
// CHECK-NEXT:     Value: 0x1000
// CHECK-NEXT:     Size: 0
// CHECK-NEXT:     Binding: Local
// CHECK-NEXT:     Type: None
// CHECK-NEXT:     Other: 0
// CHECK-NEXT:     Section: .text
// CHECK-NEXT:   }
// CHECK-NEXT:   Symbol {
// CHECK-NEXT:     Name: .Lmyvar
// CHECK-NEXT:     Value: 0x1000
// CHECK-NEXT:     Size: 0
// CHECK-NEXT:     Binding: Local
// CHECK-NEXT:     Type: None
// CHECK-NEXT:     Other: 0
// CHECK-NEXT:     Section: .text
// CHECK-NEXT:   }
