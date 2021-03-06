//===-- xray_arm.cc ---------------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file is a part of XRay, a dynamic runtime instrumentation system.
//
// Implementation of ARM-specific routines (32-bit).
//
//===----------------------------------------------------------------------===//
#include "xray_interface_internal.h"
#include "sanitizer_common/sanitizer_common.h"
#include <atomic>
#include <cassert>

namespace __xray {

// The machine codes for some instructions used in runtime patching.
enum class PatchOpcodes : uint32_t
{
  PO_PushR0Lr = 0xE92D4001, // PUSH {r0, lr}
  PO_BlxIp = 0xE12FFF3C, // BLX ip
  PO_PopR0Lr = 0xE8BD4001, // POP {r0, lr}
  PO_B20 = 0xEA000005 // B #20
};

// 0xUUUUWXYZ -> 0x000W0XYZ
inline static uint32_t getMovwMask(const uint32_t Value) {
  return (Value & 0xfff) | ((Value & 0xf000) << 4);
}

// 0xWXYZUUUU -> 0x000W0XYZ
inline static uint32_t getMovtMask(const uint32_t Value) {
  return getMovwMask(Value >> 16);
}

// Writes the following instructions:
//   MOVW R<regNo>, #<lower 16 bits of the |Value|>
//   MOVT R<regNo>, #<higher 16 bits of the |Value|>
inline static uint32_t* write32bitLoadReg(uint8_t regNo, uint32_t* Address,
    const uint32_t Value) {
  //This is a fatal error: we cannot just report it and continue execution.
  assert(regNo <= 15 && "Register number must be 0 to 15.");
  // MOVW R, #0xWXYZ in machine code is 0xE30WRXYZ
  *Address = (0xE3000000 | (uint32_t(regNo)<<12) | getMovwMask(Value));
  Address++;
  // MOVT R, #0xWXYZ in machine code is 0xE34WRXYZ
  *Address = (0xE3400000 | (uint32_t(regNo)<<12) | getMovtMask(Value));
  return Address + 1;
}

// Writes the following instructions:
//   MOVW r0, #<lower 16 bits of the |Value|>
//   MOVT r0, #<higher 16 bits of the |Value|>
inline static uint32_t *Write32bitLoadR0(uint32_t *Address,
                                         const uint32_t Value) {
  return write32bitLoadReg(0, Address, Value);
}

// Writes the following instructions:
//   MOVW ip, #<lower 16 bits of the |Value|>
//   MOVT ip, #<higher 16 bits of the |Value|>
inline static uint32_t *Write32bitLoadIP(uint32_t *Address,
                                         const uint32_t Value) {
  return write32bitLoadReg(12, Address, Value);
}

inline static bool patchSled(const bool Enable, const uint32_t FuncId,
                             const XRaySledEntry &Sled, void (*TracingHook)()) {
  // When |Enable| == true,
  // We replace the following compile-time stub (sled):
  //
  // xray_sled_n:
  //   B #20
  //   6 NOPs (24 bytes)
  //
  // With the following runtime patch:
  //
  // xray_sled_n:
  //   PUSH {r0, lr}
  //   MOVW r0, #<lower 16 bits of function ID>
  //   MOVT r0, #<higher 16 bits of function ID>
  //   MOVW ip, #<lower 16 bits of address of TracingHook>
  //   MOVT ip, #<higher 16 bits of address of TracingHook>
  //   BLX ip
  //   POP {r0, lr}
  //
  // Replacement of the first 4-byte instruction should be the last and atomic
  // operation, so that the user code which reaches the sled concurrently
  // either jumps over the whole sled, or executes the whole sled when the
  // latter is ready.
  //
  // When |Enable|==false, we set back the first instruction in the sled to be
  //   B #20

  uint32_t *FirstAddress = reinterpret_cast<uint32_t *>(Sled.Address);
  if (Enable) {
    uint32_t *CurAddress = FirstAddress + 1;
    CurAddress =
        Write32bitLoadR0(CurAddress, reinterpret_cast<uint32_t>(FuncId));
    CurAddress =
        Write32bitLoadIP(CurAddress, reinterpret_cast<uint32_t>(TracingHook));
    *CurAddress = uint32_t(PatchOpcodes::PO_BlxIp);
    CurAddress++;
    *CurAddress = uint32_t(PatchOpcodes::PO_PopR0Lr);
    std::atomic_store_explicit(
        reinterpret_cast<std::atomic<uint32_t> *>(FirstAddress),
        uint32_t(PatchOpcodes::PO_PushR0Lr), std::memory_order_release);
  } else {
    std::atomic_store_explicit(
        reinterpret_cast<std::atomic<uint32_t> *>(FirstAddress),
        uint32_t(PatchOpcodes::PO_B20), std::memory_order_release);
  }
  return true;
}

bool patchFunctionEntry(const bool Enable, const uint32_t FuncId,
                        const XRaySledEntry &Sled) {
  return patchSled(Enable, FuncId, Sled, __xray_FunctionEntry);
}

bool patchFunctionExit(const bool Enable, const uint32_t FuncId,
                       const XRaySledEntry &Sled) {
  return patchSled(Enable, FuncId, Sled, __xray_FunctionExit);
}

} // namespace __xray
