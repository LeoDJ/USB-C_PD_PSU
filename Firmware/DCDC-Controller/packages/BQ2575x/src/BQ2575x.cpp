#include "BQ2575x.h"

#include <stdint.h>
#include <string.h>


// const char *unitName[] = {
//     nullptr,
//     "",
//     "mA",
//     "uA",
//     "mV",
// };

// constexpr uint8_t uID(const char *str) {
//     int i = 0;
//     for (auto &u : unitName) {
//         if (str /*&& strcmp(u, str) == 0*/) {
//             return i;
//         }
//         i++;
//     }
//     return 0;
// }


static inline constexpr int checkStructArrayEnumParity() {    // returns -1 on success
    int i = 0;
    for (auto &regField : BQ2575x::regFields) {
        if (regField.fieldId != i) return i;
        i++;
    }
    return -1;
}
static constexpr int _mismatchIdx = checkStructArrayEnumParity();  // <- with intellisense, hover over "_mismatchIdx" to find the index
static_assert(_mismatchIdx == -1, "enum / struct array mismatch");



void BQ2575x::setIoutReg(int32_t val) {
    setValue<IOUT_REG>(val);
}

// void BQ2575x::setVoutReg(int32_t val) {
//     setValue<BQ2575x::VOUT_REG>(val);
// }


