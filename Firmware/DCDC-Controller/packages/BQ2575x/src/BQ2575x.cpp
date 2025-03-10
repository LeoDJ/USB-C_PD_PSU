#include "BQ2575x.h"

#include <stdint.h>
#include <string.h>

enum BQ_SERIES {
    BQ_ALL = 0xFF,
    BQ_6 = 0x01,    // BQ25756
    BQ_8 = 0x02,    // BQ25758
};


typedef struct RegisterField {
    uint8_t fieldId;
    uint8_t regAddress;
    // const char *regName;
    uint8_t startBit : 4;       // bit position of value
    uint8_t endBit : 4;         // inclusive
    uint8_t bqSeries;           // 
    int16_t factor;
    // Type?
    // uint8_t unitId : 3;
} regField_t;

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


enum REG_FIELDS_ID {
    VFB_REG,                    // 0x00
    IOUT_REG,                   // 0x02
    VOUT_REG,                   // 0x04
    IAC_DPM,                    // 0x06
    VAC_DPM,                    // 0x08
    IAC_REV,                    // 0x0A
    VAC_REV,                    // 0x0C
    IPRECHG,                    // 0x10
    ITERM,                      // 0x12
    EN_TERM,                    // 0x14
    VBAT_LOWV, 
    EN_PRECHG, 
    TOPOFF_TMR,                 // 0x15
    WATCHDOG,      
    EN_CHG_TMR,
    CHG_TMR,   
    EN_TMR2X,  
    CV_TMR,    
    VRECHG,                     // 0x17
    WD_RST,                     
    DIS_CE_PIN,                 
    EN_CHG_BIT_RESET_BEHAVIOR,  
    EN_HIZ,
    EN_IBAT_LOAD,
    EN_CHG,
    EN_IOUT_PIN,                // 0x18
    EN_IIN_PIN,
    DIS_PG_PIN,
    DIS_STAT_PIN,
    FORCE_STAT4_ON,
    FORCE_STAT3_ON,
    FORCE_STAT2_ON,
    FORCE_STAT_ON,
    REG_RST,                    // 0x19
    EN_IAC_LOAD,
    EN_PFM,
    PWRPATH_REDUCE_VDRV,
    EN_REV,
    FORCE_SWEEP,                // 0x1A
    P_AND_O_TMR,   
    FULL_SWEEP_TMR,
    EN_MPPT,       
    TS_T5,                      // 0x1B
    TS_T3,
    TS_T2,
    TS_T1,
    JEITA_VSET,                 // 0x1C
    JEITA_ISETH,  
    JEITA_ISETC,  
    EN_JEITA,     
    EN_TS,        
    BHOT,                       // 0x1D
    BCOLD,
    TOVLD_SET,                  // 0x1E
    SYSREV_UV,
    EN_BYPASS,
    EN_OVLD_TMAX,
    EN_OVLD_3L,
    OVLD_ILIM2,
    EN_OVLD,
    VAC_MPP,                    // 0x1F
    ADC_DONE_STAT,              // 0x21
    IAC_DPM_STAT,
    VAC_DPM_STAT,
    WD_STAT,
    CHARGE_STAT,
    PG_STAT,                    // 0x22
    TS_STAT,
    MPPT_STAT,
    FSW_SYNC_STAT,              // 0x23
    CV_TMR_STAT,
    REVERSE_STAT,
    VAC_UV_STAT,                // 0x24
    VAC_OV_STAT,
    IBAT_OCP_STAT,
    VBAT_OV_STAT,
    TSHUT_STAT,
    CHG_TMR_STAT,
    DRV_OKZ_STAT,
    ADC_DONE_FLAG,              // 0x25
    IAC_DPM_FLAG,
    VAC_DPM_FLAG,
    WD_FLAG,
    CV_TMR_FLAG,
    CHARGE_FLAG,
    PG_FLAG,                    // 0x26
    TS_FLAG,
    REVERSE_FLAG,
    FSW_SYNC_FLAG,
    MPPT_FLAG,
    VAC_UV_FLAG,                // 0x27
    VAC_OV_FLAG,
    IBAT_OCP_FLAG,
    VBAT_OV_FLAG,
    TSHUT_FLAG,
    CHG_TMR_FLAG,
    DRV_OKZ_FLAG,
    ADC_DONE_MASK,              // 0x28
    IAC_DPM_MASK,
    VAC_DPM_MASK,
    WD_MASK,
    CV_TMR_MASK,
    CHARGE_MASK,
    PG_MASK,                    // 0x29
    TS_MASK,
    REVERSE_MASK,
    FSW_SYNC_MASK,
    MPPT_MASK,
    VAC_UV_MASK,                // 0x2A
    VAC_OV_MASK,
    IBAT_OCP_MASK,
    VBAT_OV_MASK,
    TSHUT_MASK,
    CHG_TMR_MASK,
    DRV_OKZ_MASK,
    ADC_EN,                     // 0x2B
    ADC_RATE,
    ADC_SAMPLE,
    ADC_AVG,
    ADC_AVG_INIT,
    IAC_ADC_DIS,                // 0x2C
    IOUT_ADC_DIS,
    VAC_ADC_DIS,
    VOUT_ADC_DIS,
    TS_ADC_DIS,
    VFB_ADC_DIS,
    IAC_ADC,                    // 0x2D
    IOUT_ADC,                   // 0x2F
    VAC_ADC,                    // 0x31
    VOUT_ADC,                   // 0x33
    TS_ADC,                     // 0x37
    VFB_ADC,                    // 0x39
    BOOST_HS_DRV,               // 0x3B
    BUCK_HS_DRV,
    BOOST_LS_DRV,
    BUCK_LS_DRV,
    BOOST_DEAD_TIME,            // 0x3C
    BUCK_DEAD_TIME,
    PART_NUM,                   // 0x3D
    DEV_REV,
    IBAT_REV,                   // 0x62
    EN_CONV_FAST_TRANSIENT,
    REG_FIELDS_ID_NUM,          // Last entry in enum

    // aliases
    ICHG_REG =          IOUT_REG,
    EN_ICHG_PIN =       EN_IOUT_PIN,
    EN_ILIM_HIZ_PIN =   EN_IOUT_PIN,
    IBAT_ADC_DIS =      IOUT_ADC_DIS,
    VBAT_ADC_DIS =      VOUT_ADC_DIS,
    IBAT_ADC =          IOUT_ADC,
    VBAT_ADC =          VOUT_ADC,
};

#pragma pack(push, 1)
constexpr regField_t regFields[] = {
    { VFB_REG,                      0x00,   0,  4,  BQ_6,                   },
    { IOUT_REG,                     0x02,   2, 10,  BQ_ALL, 50,             }, // ICHG_REG
    { VOUT_REG,                     0x04,   2, 13,  BQ_8,                   }, 
    { IAC_DPM,                      0x06,   2, 10,  BQ_ALL, 50,             },
    { VAC_DPM,                      0x08,   2, 13,  BQ_ALL, 20,             },
    { IAC_REV,                      0x0A,   2, 10,  BQ_ALL, 50,             },
    { VAC_REV,                      0x0C,   2, 13,  BQ_ALL, 20,             },
    { IPRECHG,                      0x10,   2,  9,  BQ_6,   50,             },
    { ITERM,                        0x12,   2,  9,  BQ_6,   50,             },
    { EN_TERM,                      0x14,   3,  3,  BQ_6,                   },
    { VBAT_LOWV,                    0x14,   1,  2,  BQ_6,                   },
    { EN_PRECHG,                    0x14,   0,  0,  BQ_6,                   },
    { TOPOFF_TMR,                   0x15,   6,  7,  BQ_6,                   },
    { WATCHDOG,                     0x15,   4,  5,  BQ_ALL,                 },
    { EN_CHG_TMR,                   0x15,   3,  3,  BQ_6,                   },
    { CHG_TMR,                      0x15,   1,  2,  BQ_6,                   },
    { EN_TMR2X,                     0x15,   0,  0,  BQ_6,                   },
    { CV_TMR,                       0x16,   0,  3,  BQ_6,                   },
    { VRECHG,                       0x17,   6,  7,  BQ_6,                   },
    { WD_RST,                       0x17,   5,  5,  BQ_ALL,                 },
    { DIS_CE_PIN,                   0x17,   4,  4,  BQ_ALL,                 },
    { EN_CHG_BIT_RESET_BEHAVIOR,    0x17,   3,  3,  BQ_ALL,                 },
    { EN_HIZ,                       0x17,   2,  2,  BQ_ALL,                 },
    { EN_IBAT_LOAD,                 0x17,   1,  1,  BQ_ALL,                 },
    { EN_CHG,                       0x17,   0,  0,  BQ_ALL,                 },
    { EN_IOUT_PIN,                  0x18,   7,  7,  BQ_ALL,                 }, // EN_ICHG_PIN
    { EN_IIN_PIN,                   0x18,   6,  6,  BQ_ALL,                 }, // EN_ILIM_HIZ_PIN
    { DIS_PG_PIN,                   0x18,   5,  5,  BQ_ALL,                 },
    { DIS_STAT_PIN,                 0x18,   4,  4,  BQ_ALL,                 },
    { FORCE_STAT4_ON,               0x18,   3,  3,  BQ_ALL,                 },
    { FORCE_STAT3_ON,               0x18,   2,  2,  BQ_ALL,                 },
    { FORCE_STAT2_ON,               0x18,   1,  1,  BQ_6,                   },
    { FORCE_STAT_ON,                0x18,   0,  0,  BQ_ALL,                 },
    { REG_RST,                      0x19,   7,  7,  BQ_ALL,                 },
    { EN_IAC_LOAD,                  0x19,   6,  6,  BQ_ALL,                 },
    { EN_PFM,                       0x19,   5,  5,  BQ_ALL,                 },
    { PWRPATH_REDUCE_VDRV,          0x19,   3,  3,  BQ_8,                   },
    { EN_REV,                       0x19,   0,  0,  BQ_ALL,                 },
    { FORCE_SWEEP,                  0x1A,   7,  7,  BQ_6,                   },
    { P_AND_O_TMR,                  0x1A,   5,  6,  BQ_6,                   },
    { FULL_SWEEP_TMR,               0x1A,   1,  2,  BQ_6,                   },
    { EN_MPPT,                      0x1A,   0,  0,  BQ_6,                   },
    { TS_T5,                        0x1B,   6,  7,  BQ_ALL,                 },
    { TS_T3,                        0x1B,   4,  5,  BQ_6,                   },
    { TS_T2,                        0x1B,   2,  3,  BQ_6,                   },
    { TS_T1,                        0x1B,   0,  1,  BQ_6,                   },
    { JEITA_VSET,                   0x1C,   5,  6,  BQ_6,                   },
    { JEITA_ISETH,                  0x1C,   4,  4,  BQ_6,                   },
    { JEITA_ISETC,                  0x1C,   2,  3,  BQ_6,                   },
    { EN_JEITA,                     0x1C,   1,  1,  BQ_6,                   },
    { EN_TS,                        0x1C,   0,  0,  BQ_6,                   },
    { BHOT,                         0x1D,   6,  7,  BQ_ALL,                 },
    { BCOLD,                        0x1D,   5,  5,  BQ_ALL,                 },
    { TOVLD_SET,                    0x1E,   6,  6,  BQ_8,                   },
    { SYSREV_UV,                    0x1E,   5,  5,  BQ_ALL,                 },
    { EN_BYPASS,                    0x1E,   4,  4,  BQ_8,                   },
    { EN_OVLD_TMAX,                 0x1E,   3,  3,  BQ_8,                   },
    { EN_OVLD_3L,                   0x1E,   2,  2,  BQ_8,                   },
    { OVLD_ILIM2,                   0x1E,   1,  1,  BQ_8,                   },
    { EN_OVLD,                      0x1E,   0,  0,  BQ_8,                   },
    { VAC_MPP,                      0x1F,   2, 13,  BQ_6,   20,             },
    { ADC_DONE_STAT,                0x21,   7,  7,  BQ_ALL,                 },
    { IAC_DPM_STAT,                 0x21,   6,  6,  BQ_ALL,                 },
    { VAC_DPM_STAT,                 0x21,   5,  5,  BQ_ALL,                 },
    { WD_STAT,                      0x21,   3,  3,  BQ_ALL,                 },
    { CHARGE_STAT,                  0x21,   0,  2,  BQ_ALL,                 },
    { PG_STAT,                      0x22,   7,  7,  BQ_ALL,                 },
    { TS_STAT,                      0x22,   4,  6,  BQ_ALL,                 },
    { MPPT_STAT,                    0x22,   0,  1,  BQ_6,                   },
    { FSW_SYNC_STAT,                0x23,   4,  5,  BQ_ALL,                 },
    { CV_TMR_STAT,                  0x23,   3,  3,  BQ_6,                   },
    { REVERSE_STAT,                 0x23,   2,  2,  BQ_ALL,                 },
    { VAC_UV_STAT,                  0x24,   7,  7,  BQ_ALL,                 },
    { VAC_OV_STAT,                  0x24,   6,  6,  BQ_ALL,                 },
    { IBAT_OCP_STAT,                0x24,   5,  5,  BQ_ALL,                 },
    { VBAT_OV_STAT,                 0x24,   4,  4,  BQ_ALL,                 },
    { TSHUT_STAT,                   0x24,   3,  3,  BQ_ALL,                 },
    { CHG_TMR_STAT,                 0x24,   2,  2,  BQ_6,                   },
    { DRV_OKZ_STAT,                 0x24,   1,  1,  BQ_ALL,                 },
    { ADC_DONE_FLAG,                0x25,   7,  7,  BQ_ALL,                 },
    { IAC_DPM_FLAG,                 0x25,   6,  6,  BQ_ALL,                 },
    { VAC_DPM_FLAG,                 0x25,   5,  5,  BQ_ALL,                 },
    { WD_FLAG,                      0x25,   3,  3,  BQ_ALL,                 },
    { CV_TMR_FLAG,                  0x25,   1,  1,  BQ_6,                   },
    { CHARGE_FLAG,                  0x25,   0,  0,  BQ_ALL,                 },
    { PG_FLAG,                      0x26,   7,  7,  BQ_ALL,                 },
    { TS_FLAG,                      0x26,   4,  4,  BQ_ALL,                 },
    { REVERSE_FLAG,                 0x26,   3,  3,  BQ_ALL,                 },
    { FSW_SYNC_FLAG,                0x26,   1,  1,  BQ_ALL,                 },
    { MPPT_FLAG,                    0x26,   0,  0,  BQ_6,                   },
    { VAC_UV_FLAG,                  0x27,   7,  7,  BQ_ALL,                 },
    { VAC_OV_FLAG,                  0x27,   6,  6,  BQ_ALL,                 },
    { IBAT_OCP_FLAG,                0x27,   5,  5,  BQ_ALL,                 },
    { VBAT_OV_FLAG,                 0x27,   4,  4,  BQ_ALL,                 },
    { TSHUT_FLAG,                   0x27,   3,  3,  BQ_ALL,                 },
    { CHG_TMR_FLAG,                 0x27,   2,  2,  BQ_6,                   },
    { DRV_OKZ_FLAG,                 0x27,   1,  1,  BQ_ALL,                 },
    { ADC_DONE_MASK,                0x28,   7,  7,  BQ_ALL,                 },
    { IAC_DPM_MASK,                 0x28,   6,  6,  BQ_ALL,                 },
    { VAC_DPM_MASK,                 0x28,   5,  5,  BQ_ALL,                 },
    { WD_MASK,                      0x28,   3,  3,  BQ_ALL,                 },
    { CV_TMR_MASK,                  0x28,   1,  1,  BQ_6,                   },
    { CHARGE_MASK,                  0x28,   0,  0,  BQ_ALL,                 },
    { PG_MASK,                      0x29,   7,  7,  BQ_ALL,                 },
    { TS_MASK,                      0x29,   4,  4,  BQ_ALL,                 },
    { REVERSE_MASK,                 0x29,   3,  3,  BQ_ALL,                 },
    { FSW_SYNC_MASK,                0x29,   1,  1,  BQ_ALL,                 },
    { MPPT_MASK,                    0x29,   0,  0,  BQ_6,                   },
    { VAC_UV_MASK,                  0x2A,   7,  7,  BQ_ALL,                 },
    { VAC_OV_MASK,                  0x2A,   6,  6,  BQ_ALL,                 },
    { IBAT_OCP_MASK,                0x2A,   5,  5,  BQ_ALL,                 },
    { VBAT_OV_MASK,                 0x2A,   4,  4,  BQ_ALL,                 },
    { TSHUT_MASK,                   0x2A,   3,  3,  BQ_ALL,                 },
    { CHG_TMR_MASK,                 0x2A,   2,  2,  BQ_6,                   },
    { DRV_OKZ_MASK,                 0x2A,   1,  1,  BQ_ALL,                 },
    { ADC_EN,                       0x2B,   7,  7,  BQ_ALL,                 },
    { ADC_RATE,                     0x2B,   6,  6,  BQ_ALL,                 },
    { ADC_SAMPLE,                   0x2B,   4,  5,  BQ_ALL,                 },
    { ADC_AVG,                      0x2B,   3,  3,  BQ_ALL,                 },
    { ADC_AVG_INIT,                 0x2B,   2,  2,  BQ_ALL,                 },
    { IAC_ADC_DIS,                  0x2C,   7,  7,  BQ_ALL,                 },
    { IOUT_ADC_DIS,                 0x2C,   6,  6,  BQ_ALL,                 }, // IBAT_ADC_DIS
    { VAC_ADC_DIS,                  0x2C,   5,  5,  BQ_ALL,                 },
    { VOUT_ADC_DIS,                 0x2C,   4,  4,  BQ_ALL,                 }, // VBAT_ADC_DIS
    { TS_ADC_DIS,                   0x2C,   2,  2,  BQ_ALL,                 },
    { VFB_ADC_DIS,                  0x2C,   1,  1,  BQ_6,                   },
    { IAC_ADC,                      0x2D,   0, 15,  BQ_ALL, 800,            }, // uA
    { IOUT_ADC,                     0x2F,   0, 15,  BQ_ALL, 2,              }, // IBAT_ADC
    { VAC_ADC,                      0x31,   0, 15,  BQ_ALL, 2,              },
    { VOUT_ADC,                     0x33,   0, 15,  BQ_ALL, 2,              }, // VBAT_ADC
    { TS_ADC,                       0x37,   0, 15,  BQ_ALL,                 },
    { VFB_ADC,                      0x39,   0, 15,  BQ_6,   1,              },
    { BOOST_HS_DRV,                 0x3B,   6,  7,  BQ_ALL,                 },
    { BUCK_HS_DRV,                  0x3B,   4,  5,  BQ_ALL,                 },
    { BOOST_LS_DRV,                 0x3B,   2,  3,  BQ_ALL,                 },
    { BUCK_LS_DRV,                  0x3B,   0,  1,  BQ_ALL,                 },
    { BOOST_DEAD_TIME,              0x3C,   2,  3,  BQ_ALL,                 },
    { BUCK_DEAD_TIME,               0x3C,   0,  1,  BQ_ALL,                 },
    { PART_NUM,                     0x3D,   3,  5,  BQ_ALL,                 },
    { DEV_REV,                      0x3D,   0,  2,  BQ_ALL,                 },
    { IBAT_REV,                     0x62,   6,  7,  BQ_ALL,                 },
    { EN_CONV_FAST_TRANSIENT,       0x62,   1,  1,  BQ_ALL,                 },
};
#pragma pack(pop)

constexpr int len = sizeof(regFields);
static_assert(REG_FIELDS_ID_NUM == sizeof(regFields) / sizeof(regFields[0]), "Length mismatch between struct array and enum, check association. (Index of array must equal ID enum value)");

constexpr int checkStructArrayEnumParity() {    // returns -1 on success
    int i = 0;
    for (auto &regField : regFields) {
        if (regField.fieldId != i) return i;
        i++;
    }
    return -1;
}
static_assert(checkStructArrayEnumParity() == -1, "enum / struct array mismatch");
constexpr int mismatchIdx = checkStructArrayEnumParity();  // <- with intellisense, hover over "mismatchIdx" to find the index


template <unsigned F>
constexpr uint16_t getMask() {
    constexpr int len = regFields[F].endBit - regFields[F].startBit + 1;
    return ((1ULL << len) - 1) << regFields[F].startBit;
}

template <unsigned F>
constexpr void setValue(int32_t value) {
    uint16_t registerValue /* = readVal(regFields[F].regAddress) */;
    registerValue &= ~getMask<F>();
    if constexpr (regFields[F].factor != 0) {
        value /= regFields[F].factor;
    }
    registerValue |= value << regFields[F].startBit;

    // writeVal(regFields[F].regAddress, registerValue);
}

void setIoutReg(int32_t val) {
    setValue<IOUT_REG>(val);
}

void setVoutReg(int32_t val) {
    setValue<VOUT_REG>(val);
}


