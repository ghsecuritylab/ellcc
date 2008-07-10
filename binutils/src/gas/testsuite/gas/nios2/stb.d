#objdump: -dr --prefix-addresses
#name: NIOS2 stb

# Test the ld instruction

.*: +file format elf32-littlenios2

Disassembly of section .text:
0+0000 <[^>]*> stb	r4,0\(zero\)
0+0004 <[^>]*> stb	r4,4\(zero\)
0+0008 <[^>]*> stb	r4,32764\(zero\)
0+000c <[^>]*> stb	r4,-32768\(zero\)
0+0010 <[^>]*> stb	r4,0\(r5\)
0+0014 <[^>]*> stb	r4,4\(r5\)
0+0018 <[^>]*> stb	r4,32764\(r5\)
0+001c <[^>]*> stb	r4,-32768\(r5\)
0+0020 <[^>]*> stb	r4,0\(zero\)
[	]*20: R_NIOS2_S16	.data
0+0024 <[^>]*> stb	r4,0\(zero\)
[	]*24: R_NIOS2_S16	big_external_data_label
0+0028 <[^>]*> stb	r4,0\(zero\)
[	]*28: R_NIOS2_S16	small_external_data_label
0+002c <[^>]*> stb	r4,0\(zero\)
[	]*2c: R_NIOS2_S16	big_external_common
0+0030 <[^>]*> stb	r4,0\(zero\)
[	]*30: R_NIOS2_S16	small_external_common
0+0034 <[^>]*> stb	r4,0\(zero\)
[	]*34: R_NIOS2_S16	.bss
0+0038 <[^>]*> stb	r4,0\(zero\)
[	]*38: R_NIOS2_S16	.bss\+0x4000
0+003c <[^>]*> stb	r4,0\(zero\)
[	]*3c: R_NIOS2_S16	.data\+0x4
0+0040 <[^>]*> stb	r4,0\(zero\)
[	]*40: R_NIOS2_S16	big_external_data_label\+0x4
0+0044 <[^>]*> stb	r4,0\(zero\)
[	]*44: R_NIOS2_S16	small_external_data_label\+0x4
0+0048 <[^>]*> stb	r4,0\(zero\)
[	]*48: R_NIOS2_S16	big_external_common\+0x4
0+004c <[^>]*> stb	r4,0\(zero\)
[	]*4c: R_NIOS2_S16	small_external_common\+0x4
0+0050 <[^>]*> stb	r4,0\(zero\)
[	]*50: R_NIOS2_S16	.bss\+0x4
0+0054 <[^>]*> stb	r4,0\(zero\)
[	]*54: R_NIOS2_S16	.bss\+0x4004
0+0058 <[^>]*> stb	r4,0\(zero\)
[	]*58: R_NIOS2_S16	.data\+0xffff8000
0+005c <[^>]*> stb	r4,0\(zero\)
[	]*5c: R_NIOS2_S16	big_external_data_label\+0xffff8000
0+0060 <[^>]*> stb	r4,0\(zero\)
[	]*60: R_NIOS2_S16	small_external_data_label\+0xffff8000
0+0064 <[^>]*> stb	r4,0\(zero\)
[	]*64: R_NIOS2_S16	big_external_common\+0xffff8000
0+0068 <[^>]*> stb	r4,0\(zero\)
[	]*68: R_NIOS2_S16	small_external_common\+0xffff8000
0+006c <[^>]*> stb	r4,0\(zero\)
[	]*6c: R_NIOS2_S16	.bss\+0xffff8000
0+0070 <[^>]*> stb	r4,0\(zero\)
[	]*70: R_NIOS2_S16	.bss\+0xffffc000
0+0074 <[^>]*> stb	r4,0\(zero\)
[	]*74: R_NIOS2_S16	.data\+0x10000
0+0078 <[^>]*> stb	r4,0\(r5\)
[	]*78: R_NIOS2_S16	.data
0+007c <[^>]*> stb	r4,0\(r5\)
[	]*7c: R_NIOS2_S16	big_external_data_label
0+0080 <[^>]*> stb	r4,0\(r5\)
[	]*80: R_NIOS2_S16	small_external_data_label
0+0084 <[^>]*> stb	r4,0\(r5\)
[	]*84: R_NIOS2_S16	big_external_common
0+0088 <[^>]*> stb	r4,0\(r5\)
[	]*88: R_NIOS2_S16	small_external_common
0+008c <[^>]*> stb	r4,0\(r5\)
[	]*8c: R_NIOS2_S16	.bss
0+0090 <[^>]*> stb	r4,0\(r5\)
[	]*90: R_NIOS2_S16	.bss\+0x4000
0+0094 <[^>]*> stb	r4,0\(r5\)
[	]*94: R_NIOS2_S16	.data\+0x4
0+0098 <[^>]*> stb	r4,0\(r5\)
[	]*98: R_NIOS2_S16	big_external_data_label\+0x4
0+009c <[^>]*> stb	r4,0\(r5\)
[	]*9c: R_NIOS2_S16	small_external_data_label\+0x4
0+00a0 <[^>]*> stb	r4,0\(r5\)
[	]*a0: R_NIOS2_S16	big_external_common\+0x4
0+00a4 <[^>]*> stb	r4,0\(r5\)
[	]*a4: R_NIOS2_S16	small_external_common\+0x4
0+00a8 <[^>]*> stb	r4,0\(r5\)
[	]*a8: R_NIOS2_S16	.bss\+0x4
0+00ac <[^>]*> stb	r4,0\(r5\)
[	]*ac: R_NIOS2_S16	.bss\+0x4004
0+00b0 <[^>]*> stb	r4,0\(r5\)
[	]*b0: R_NIOS2_S16	.data\+0xffff8000
0+00b4 <[^>]*> stb	r4,0\(r5\)
[	]*b4: R_NIOS2_S16	big_external_data_label\+0xffff8000
0+00b8 <[^>]*> stb	r4,0\(r5\)
[	]*b8: R_NIOS2_S16	small_external_data_label\+0xffff8000
0+00bc <[^>]*> stb	r4,0\(r5\)
[	]*bc: R_NIOS2_S16	big_external_common\+0xffff8000
0+00c0 <[^>]*> stb	r4,0\(r5\)
[	]*c0: R_NIOS2_S16	small_external_common\+0xffff8000
0+00c4 <[^>]*> stb	r4,0\(r5\)
[	]*c4: R_NIOS2_S16	.bss\+0xffff8000
0+00c8 <[^>]*> stb	r4,0\(r5\)
[	]*c8: R_NIOS2_S16	.bss\+0xffffc000
0+00cc <[^>]*> stbio	r4,0\(zero\)
0+00d0 <[^>]*> stbio	r4,4\(zero\)
0+00d4 <[^>]*> stbio	r4,32764\(zero\)
0+00d8 <[^>]*> stbio	r4,-32768\(zero\)
0+00dc <[^>]*> stbio	r4,0\(r5\)
0+00e0 <[^>]*> stbio	r4,4\(r5\)
0+00e4 <[^>]*> stbio	r4,32764\(r5\)
0+00e8 <[^>]*> stbio	r4,-32768\(r5\)
0+00ec <[^>]*> stbio	r4,0\(zero\)
[	]*ec: R_NIOS2_S16	.data
0+00f0 <[^>]*> stbio	r4,0\(zero\)
[	]*f0: R_NIOS2_S16	big_external_data_label
0+00f4 <[^>]*> stbio	r4,0\(zero\)
[	]*f4: R_NIOS2_S16	small_external_data_label
0+00f8 <[^>]*> stbio	r4,0\(zero\)
[	]*f8: R_NIOS2_S16	big_external_common
0+00fc <[^>]*> stbio	r4,0\(zero\)
[	]*fc: R_NIOS2_S16	small_external_common
0+0100 <[^>]*> stbio	r4,0\(zero\)
[	]*100: R_NIOS2_S16	.bss
0+0104 <[^>]*> stbio	r4,0\(zero\)
[	]*104: R_NIOS2_S16	.bss\+0x4000
0+0108 <[^>]*> stbio	r4,0\(zero\)
[	]*108: R_NIOS2_S16	.data\+0x4
0+010c <[^>]*> stbio	r4,0\(zero\)
[	]*10c: R_NIOS2_S16	big_external_data_label\+0x4
0+0110 <[^>]*> stbio	r4,0\(zero\)
[	]*110: R_NIOS2_S16	small_external_data_label\+0x4
0+0114 <[^>]*> stbio	r4,0\(zero\)
[	]*114: R_NIOS2_S16	big_external_common\+0x4
0+0118 <[^>]*> stbio	r4,0\(zero\)
[	]*118: R_NIOS2_S16	small_external_common\+0x4
0+011c <[^>]*> stbio	r4,0\(zero\)
[	]*11c: R_NIOS2_S16	.bss\+0x4
0+0120 <[^>]*> stbio	r4,0\(zero\)
[	]*120: R_NIOS2_S16	.bss\+0x4004
0+0124 <[^>]*> stbio	r4,0\(zero\)
[	]*124: R_NIOS2_S16	.data\+0xffff8000
0+0128 <[^>]*> stbio	r4,0\(zero\)
[	]*128: R_NIOS2_S16	big_external_data_label\+0xffff8000
0+012c <[^>]*> stbio	r4,0\(zero\)
[	]*12c: R_NIOS2_S16	small_external_data_label\+0xffff8000
0+0130 <[^>]*> stbio	r4,0\(zero\)
[	]*130: R_NIOS2_S16	big_external_common\+0xffff8000
0+0134 <[^>]*> stbio	r4,0\(zero\)
[	]*134: R_NIOS2_S16	small_external_common\+0xffff8000
0+0138 <[^>]*> stbio	r4,0\(zero\)
[	]*138: R_NIOS2_S16	.bss\+0xffff8000
0+013c <[^>]*> stbio	r4,0\(zero\)
[	]*13c: R_NIOS2_S16	.bss\+0xffffc000
0+0140 <[^>]*> stbio	r4,0\(zero\)
[	]*140: R_NIOS2_S16	.data\+0x10000
0+0144 <[^>]*> stbio	r4,0\(r5\)
[	]*144: R_NIOS2_S16	.data
0+0148 <[^>]*> stbio	r4,0\(r5\)
[	]*148: R_NIOS2_S16	big_external_data_label
0+014c <[^>]*> stbio	r4,0\(r5\)
[	]*14c: R_NIOS2_S16	small_external_data_label
0+0150 <[^>]*> stbio	r4,0\(r5\)
[	]*150: R_NIOS2_S16	big_external_common
0+0154 <[^>]*> stbio	r4,0\(r5\)
[	]*154: R_NIOS2_S16	small_external_common
0+0158 <[^>]*> stbio	r4,0\(r5\)
[	]*158: R_NIOS2_S16	.bss
0+015c <[^>]*> stbio	r4,0\(r5\)
[	]*15c: R_NIOS2_S16	.bss\+0x4000
0+0160 <[^>]*> stbio	r4,0\(r5\)
[	]*160: R_NIOS2_S16	.data\+0x4
0+0164 <[^>]*> stbio	r4,0\(r5\)
[	]*164: R_NIOS2_S16	big_external_data_label\+0x4
0+0168 <[^>]*> stbio	r4,0\(r5\)
[	]*168: R_NIOS2_S16	small_external_data_label\+0x4
0+016c <[^>]*> stbio	r4,0\(r5\)
[	]*16c: R_NIOS2_S16	big_external_common\+0x4
0+0170 <[^>]*> stbio	r4,0\(r5\)
[	]*170: R_NIOS2_S16	small_external_common\+0x4
0+0174 <[^>]*> stbio	r4,0\(r5\)
[	]*174: R_NIOS2_S16	.bss\+0x4
0+0178 <[^>]*> stbio	r4,0\(r5\)
[	]*178: R_NIOS2_S16	.bss\+0x4004
0+017c <[^>]*> stbio	r4,0\(r5\)
[	]*17c: R_NIOS2_S16	.data\+0xffff8000
0+0180 <[^>]*> stbio	r4,0\(r5\)
[	]*180: R_NIOS2_S16	big_external_data_label\+0xffff8000
0+0184 <[^>]*> stbio	r4,0\(r5\)
[	]*184: R_NIOS2_S16	small_external_data_label\+0xffff8000
0+0188 <[^>]*> stbio	r4,0\(r5\)
[	]*188: R_NIOS2_S16	big_external_common\+0xffff8000
0+018c <[^>]*> stbio	r4,0\(r5\)
[	]*18c: R_NIOS2_S16	small_external_common\+0xffff8000
0+0190 <[^>]*> stbio	r4,0\(r5\)
[	]*190: R_NIOS2_S16	.bss\+0xffff8000
0+0194 <[^>]*> stbio	r4,0\(r5\)
[	]*194: R_NIOS2_S16	.bss\+0xffffc000
