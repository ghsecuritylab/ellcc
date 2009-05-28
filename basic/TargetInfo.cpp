//===--- TargetInfo.cpp - Information about Target machine ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file implements the TargetInfo and TargetInfoImpl interfaces.
//
//===----------------------------------------------------------------------===//

#include "TargetInfo.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include <cstdlib>
#include <sstream>
using namespace ellcc;

// TargetInfo Constructor.
TargetInfo::TargetInfo(const std::string &T) : Triple(T) {
  // Set defaults.  Defaults are set for a 32-bit RISC platform,
  // like PPC or SPARC.
  // These should be overridden by concrete targets as needed.
  BigEndian = false;
  CharIsSigned = true;
  CharWidth = CharAlign = 8;
  PrefCharAlign = 0;
  WCharWidth = WCharAlign = 32;
  PrefWCharAlign = 0;
  BoolWidth = BoolAlign = 8;
  PrefBoolAlign = 0;
  ShortWidth = ShortAlign = 16;
  PrefShortAlign = 0;
  IntWidth = IntAlign = 32;
  PrefIntAlign = 0;
  LongWidth = LongAlign = 32;
  PrefLongAlign = 0;
  LongLongWidth = LongLongAlign = 64;
  PrefLongLongAlign = 0;
  IntMaxTWidth = 64;
  FloatWidth = FloatAlign = 32;
  PrefFloatAlign = 0;
  FloatFormat = &llvm::APFloat::IEEEsingle;

  DoubleWidth = DoubleAlign = 64;
  PrefDoubleAlign = 0;
  DoubleFormat = &llvm::APFloat::IEEEdouble;

  LongDoubleWidth = LongDoubleAlign = 128;
  PrefLongDoubleAlign = 0;
  LongDoubleFormat = &llvm::APFloat::IEEEquad;

  PointerWidth = PointerAlign = 32;
  PrefPointerAlign = 0;
  VectorWidth = VectorAlign = 64;
  PrefVectorAlign = 64;
  LongVectorWidth = LongVectorAlign = 128;
  PrefLongVectorAlign = 128;
  AggregateWidth = 0;
  AggregateAlign = 0;
  PrefAggregateAlign = 0;

  SizeType = UnsignedLong;
  PtrDiffType = SignedLong;
  IntMaxType = SignedLongLong;
  UIntMaxType = UnsignedLongLong;
  IntPtrType = SignedLong;
  WCharType = SignedInt;

  UserLabelPrefix = "_";
}

// Out of line virtual dtor for TargetInfo.
TargetInfo::~TargetInfo() {}

// Get the target description string, e.g.:
// "E-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64"

#define xstr(x) #x
#define str(x) xstr(x)
#define DATA(type, name)                \
    str << "-" str(type);               \
    str << (int)name##Width;            \
    str << ":";                         \
    str << (int)name##Align;            \
    if (Pref##name##Align) {            \
        str << ":";                     \
        str << (int)Pref##name##Align;  \
    }

#define DATAP(type, name)               \
    str << "-" str(type);               \
    str << ":";                         \
    str << (int)name##Width;            \
    str << ":";                         \
    str << (int)name##Align;            \
    if (Pref##name##Align) {            \
        str << ":";                     \
        str << (int)Pref##name##Align;  \
    }

#define DATAN(type, name)               \
    str << "-" str(type);               \
    str << ":";                         \
    str << (int)name##Align;            \
    if (Pref##name##Align) {            \
        str << ":";                     \
        str << (int)Pref##name##Align;  \
    }

void TargetInfo::getTargetDescription(std::string& res)
{
    std::stringstream str;
    if (BigEndian) {
        str << "E";
    } else {
        str << "e";
    }
    DATAP(p, Pointer);
    DATAN(i1, Bool);
    DATA(i, Char);
    DATA(i, Short);
    DATA(i, Int);
    DATA(i, Long);
    DATA(i, LongLong);
    DATA(f, Float);
    DATA(f, Double);
    DATA(f, LongDouble);
    DATA(v, Vector);
    DATA(v, LongVector);
    DATAN(a0, Aggregate);
    str >> res;
}
#undef xstr
#undef str
#undef DATA
#undef DATAP
#undef DATAN

// Work around bitwise-OR in initializers.
#define S(x) ((SimpleTypeFlags)(x))
// Info about each simple type.
const TargetInfo::SimpleTypeInfo TargetInfo::simpleTypes[SimpleTypeCount] =
{
  { "<no type>",                S(STF_NONE)                            },
  { "bool",                     S(STF_INTEGER)                         },
  { "char",                     S(STF_INTEGER)                         },
  { "signed char",              S(STF_INTEGER)                         },
  { "unsigned char",            S(STF_INTEGER|STF_UNSIGNED)            },
  { "short",                    S(STF_INTEGER)                         },
  { "unsigned short",           S(STF_INTEGER|STF_UNSIGNED)            },
  { "wchar_t",                  S(STF_INTEGER)                         },
  { "int",                      S(STF_INTEGER)                         },
  { "unsigned int",             S(STF_INTEGER|STF_PROM|STF_UNSIGNED)   },
  { "long",                     S(STF_INTEGER|STF_PROM)                },
  { "unsigned long",            S(STF_INTEGER|STF_PROM|STF_UNSIGNED)   },
  { "long long",                S(STF_INTEGER|STF_PROM)                },
  { "unsigned long long",       S(STF_INTEGER|STF_PROM|STF_UNSIGNED)   },
  { "float",                    S(STF_FLOAT)                           },
  { "double",                   S(STF_FLOAT|STF_PROM)                  },
  { "long double",              S(STF_FLOAT)                           },
  { "float _Complex",           S(STF_FLOAT)                           },
  { "double _Complex",          S(STF_FLOAT)                           },
  { "long double _Complex",     S(STF_FLOAT)                           },
  { "float _Imaginary",         S(STF_FLOAT)                           },
  { "float _Imaginary",         S(STF_FLOAT)                           },
  { "long double _Imaginary",   S(STF_FLOAT)                           },
  { "void",                     S(STF_NONE)                            },
};

/// getTypeName - Return the user string for the specified type enum.
/// For example, SignedShort -> "short".
const char *TargetInfo::getTypeName(SimpleType T) {
  assert(T < SimpleTypeCount && "Invalid type passed in");
  return simpleTypes[T].name;
}

//===----------------------------------------------------------------------===//


static void removeGCCRegisterPrefix(const char *&Name) {
  if (Name[0] == '%' || Name[0] == '#')
    Name++;
}

/// isValidGCCRegisterName - Returns whether the passed in string
/// is a valid register name according to GCC. This is used by Sema for
/// inline asm statements.
bool TargetInfo::isValidGCCRegisterName(const char *Name) const {
  const char * const *Names;
  unsigned NumNames;
  
  // Get rid of any register prefix.
  removeGCCRegisterPrefix(Name);

  
  if (strcmp(Name, "memory") == 0 ||
      strcmp(Name, "cc") == 0)
    return true;
  
  getGCCRegNames(Names, NumNames);
  
  // If we have a number it maps to an entry in the register name array.
  if (isdigit(Name[0])) {
    char *End;
    int n = (int)strtol(Name, &End, 0);
    if (*End == 0)
      return n >= 0 && (unsigned)n < NumNames;
  }

  // Check register names.
  for (unsigned i = 0; i < NumNames; i++) {
    if (strcmp(Name, Names[i]) == 0)
      return true;
  }
  
  // Now check aliases.
  const GCCRegAlias *Aliases;
  unsigned NumAliases;
  
  getGCCRegAliases(Aliases, NumAliases);
  for (unsigned i = 0; i < NumAliases; i++) {
    for (unsigned j = 0 ; j < llvm::array_lengthof(Aliases[i].Aliases); j++) {
      if (!Aliases[i].Aliases[j])
        break;
      if (strcmp(Aliases[i].Aliases[j], Name) == 0)
        return true;
    }
  }
  
  return false;
}

const char *TargetInfo::getNormalizedGCCRegisterName(const char *Name) const {
  assert(isValidGCCRegisterName(Name) && "Invalid register passed in");
  
  removeGCCRegisterPrefix(Name);
    
  const char * const *Names;
  unsigned NumNames;

  getGCCRegNames(Names, NumNames);

  // First, check if we have a number.
  if (isdigit(Name[0])) {
    char *End;
    int n = (int)strtol(Name, &End, 0);
    if (*End == 0) {
      assert(n >= 0 && (unsigned)n < NumNames && 
             "Out of bounds register number!");
      return Names[n];
    }
  }
  
  // Now check aliases.
  const GCCRegAlias *Aliases;
  unsigned NumAliases;
  
  getGCCRegAliases(Aliases, NumAliases);
  for (unsigned i = 0; i < NumAliases; i++) {
    for (unsigned j = 0 ; j < llvm::array_lengthof(Aliases[i].Aliases); j++) {
      if (!Aliases[i].Aliases[j])
        break;
      if (strcmp(Aliases[i].Aliases[j], Name) == 0)
        return Aliases[i].Register;
    }
  }
  
  return Name;
}

bool TargetInfo::validateOutputConstraint(const char *Name, 
                                          ConstraintInfo &info) const
{
  info = CI_None;

  // An output constraint must start with '=' or '+'
  if (*Name != '=' && *Name != '+')
    return false;

  if (*Name == '+')
    info = CI_ReadWrite;
  else
    info = CI_None;

  Name++;
  while (*Name) {
    switch (*Name) {
    default:
      if (!validateAsmConstraint(Name, info)) {
        // FIXME: We temporarily return false
        // so we can add more constraints as we hit it.
        // Eventually, an unknown constraint should just be treated as 'g'.
        return false;
      }
    case '&': // early clobber.
      break;
    case 'r': // general register.
      info = (ConstraintInfo)(info|CI_AllowsRegister);
      break;
    case 'm': // memory operand.
      info = (ConstraintInfo)(info|CI_AllowsMemory);
      break;
    case 'g': // general register, memory operand or immediate integer.
    case 'X': // any operand.
      info = (ConstraintInfo)(info|CI_AllowsMemory|CI_AllowsRegister);
      break;
    }
    
    Name++;
  }
  
  return true;
}

bool TargetInfo::resolveSymbolicName(const char *&Name,
                                     const std::string *OutputNamesBegin,
                                     const std::string *OutputNamesEnd,
                                     unsigned &Index) const
{
  assert(*Name == '[' && "Symbolic name did not start with '['");

  Name++;
  const char *Start = Name;
  while (*Name && *Name != ']')
    Name++;
  
  if (!*Name) {
    // Missing ']'
    return false;
  }
  
  std::string SymbolicName(Start, Name - Start);
  
  Index = 0;
  for (const std::string *it = OutputNamesBegin; 
       it != OutputNamesEnd; 
       ++it, Index++) {
    if (SymbolicName == *it)
      return true;
  }

  return false;
}

bool TargetInfo::validateInputConstraint(const char *Name,
                                         const std::string *OutputNamesBegin,
                                         const std::string *OutputNamesEnd,
                                         ConstraintInfo* OutputConstraints,
                                         ConstraintInfo &info) const {
  info = CI_None;

  while (*Name) {
    switch (*Name) {
    default:
      // Check if we have a matching constraint
      if (*Name >= '0' && *Name <= '9') {
        unsigned NumOutputs = OutputNamesEnd - OutputNamesBegin;
        unsigned i = *Name - '0';
  
        // Check if matching constraint is out of bounds.
        if (i >= NumOutputs)
          return false;
        
        // The constraint should have the same info as the respective 
        // output constraint.
        info = (ConstraintInfo)(info|OutputConstraints[i]);
      } else if (!validateAsmConstraint(Name, info)) {
        // FIXME: This error return is in place temporarily so we can
        // add more constraints as we hit it.  Eventually, an unknown
        // constraint should just be treated as 'g'.
        return false;
      }
      break;
    case '[': {
      unsigned Index = 0;
      if (!resolveSymbolicName(Name, OutputNamesBegin, OutputNamesEnd, Index))
        return false;
    
      break;
    }          
    case '%': // commutative
      // FIXME: Fail if % is used with the last operand.
      break;
    case 'i': // immediate integer.
    case 'I':
    case 'n': // immediate integer with a known value.
      break;
    case 'r': // general register.
      info = (ConstraintInfo)(info|CI_AllowsRegister);
      break;
    case 'm': // memory operand.
      info = (ConstraintInfo)(info|CI_AllowsMemory);
      break;
    case 'g': // general register, memory operand or immediate integer.
    case 'X': // any operand.
      info = (ConstraintInfo)(info|CI_AllowsMemory|CI_AllowsRegister);
      break;
    }
    
    Name++;
  }
  
  return true;
}
