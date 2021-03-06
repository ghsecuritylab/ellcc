// RUN: %clangxx_asan -O0 %s -Fe%t
// FIXME: 'cat' is needed due to PR19744.
// RUN: not %run %t 2>&1 | cat | FileCheck %s

#include <malloc.h>

int main() {
  char *buffer = (char*)malloc(42);
  buffer[42] = 42;
// CHECK: AddressSanitizer: heap-buffer-overflow on address [[ADDR:0x[0-9a-f]+]]
// CHECK: WRITE of size 1 at [[ADDR]] thread T0
// CHECK-NEXT: {{#0 .* main .*malloc_right_oob.cc}}:[[@LINE-3]]
// CHECK: [[ADDR]] is located 0 bytes to the right of 42-byte region
// CHECK: allocated by thread T0 here:
// CHECK-NEXT: {{#0 .* malloc }}
// CHECK-NEXT: {{#1 .* main .*malloc_right_oob.cc}}:[[@LINE-8]]
  free(buffer);
}
