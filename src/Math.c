#include "Math.h"

int8 Max8(int8 a, int8 b) { return a > b ? a : b; }
uint8 MaxU8(uint8 a, uint8 b) { return a > b ? a : b; }
int16 Max16(int16 a, int16 b) { return a > b ? a : b; }
uint16 MaxU16(uint16 a, uint16 b) { return a > b ? a : b; }
int32 Max32(int32 a, int32 b) { return a > b ? a : b; }
uint32 MaxU32(uint32 a, uint32 b) { return a > b ? a : b; }
int64 Max64(int64 a, int64 b) { return a > b ? a : b; }
uint64 MaxU64(uint64 a, uint64 b) { return a > b ? a : b; }
fpnt32 MaxF32(fpnt32 a, fpnt32 b) { return a > b ? a : b; }
fpnt64 MaxF64(fpnt64 a, fpnt64 b) { return a > b ? a : b; }

int8 Min8(int8 a, int8 b) { return a < b ? a : b; }
uint8 MinU8(uint8 a, uint8 b) { return a < b ? a : b; }
int16 Min16(int16 a, int16 b) { return a < b ? a : b; }
uint16 MinU16(uint16 a, uint16 b) { return a < b ? a : b; }
int32 Min32(int32 a, int32 b) { return a < b ? a : b; }
uint32 MinU32(uint32 a, uint32 b) { return a < b ? a : b; }
int64 Min64(int64 a, int64 b) { return a < b ? a : b; }
uint64 MinU64(uint64 a, uint64 b) { return a < b ? a : b; }
fpnt32 MinF32(fpnt32 a, fpnt32 b) { return a < b ? a : b; }
fpnt64 MinF64(fpnt64 a, fpnt64 b) { return a < b ? a : b; }