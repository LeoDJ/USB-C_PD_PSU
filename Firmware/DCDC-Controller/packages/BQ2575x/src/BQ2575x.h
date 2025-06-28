#pragma once
#include <stdint.h>
#include <stddef.h>



class BQ2575x {
    public:
    BQ2575x(int (*i2c_read)(uint16_t devAddr, uint8_t regStart, uint8_t* buf, size_t numBytes),
            int (*i2c_write)(uint16_t devAddr, uint8_t regStart, const uint8_t* buf, size_t numBytes))
          : _i2c_read(i2c_read), _i2c_write(i2c_write) {
      }

    template <unsigned F>
    constexpr uint16_t getMask() const {
        constexpr int len = regFields[F].endBit - regFields[F].startBit + 1;
        return ((1ULL << len) - 1) << regFields[F].startBit;
    }

    template <unsigned F>
    constexpr void setValue(int32_t value) const {
        constexpr regField_t field = regFields[F];
        constexpr bool isShort = (field.startBit > 7 || field.endBit > 7);   // 2 byte register?
        uint16_t registerValue = _i2c_read_reg(field.regAddress, isShort);
        registerValue &= ~getMask<F>();
        if constexpr (field.factor != 0) {
            value /= field.factor;
        }
        registerValue |= value << field.startBit;

        // writeVal(regFields[F].regAddress, registerValue);
        _i2c_write_reg(field.regAddress, registerValue, isShort);
    }

    template <unsigned F>
    constexpr int32_t getValue() const {
        constexpr regField_t field = regFields[F];
        constexpr bool isShort = (field.startBit > 7 || field.endBit > 7);   // 2 byte register?
        uint16_t registerValue = _i2c_read_reg(field.regAddress, isShort);
        registerValue &= ~getMask<F>();
        int32_t value = registerValue >> field.startBit;
        if constexpr (field.factor != 0) {
            value *= field.factor;
        }
        return value;
    }

    void setIoutReg(int32_t val);

    typedef struct RegisterField {
        uint8_t fieldId;
        uint8_t regAddress;
        // const char *regName;
        uint8_t startBit : 4;       // bit position of value
        uint8_t endBit : 4;         // inclusive
        uint8_t bqSeries;           
        int32_t factor;
        // Type?
        // uint8_t unitId : 3;
    } regField_t;

    enum BQ_SERIES {    // bitfield
        BQ_ALL = 0xFF,
        BQ_6 = 0x01,    // BQ25756
        BQ_8 = 0x02,    // BQ25758
    };
      
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
    static constexpr regField_t regFields[] = {
        { BQ2575x::VFB_REG,                     0x00,   0,  4,  BQ_6,                   },
        { BQ2575x::IOUT_REG,                    0x02,   2, 10,  BQ_ALL, 50000,          }, // ICHG_REG
        { BQ2575x::VOUT_REG,                    0x04,   2, 13,  BQ_8,   20000,          }, 
        { BQ2575x::IAC_DPM,                     0x06,   2, 10,  BQ_ALL, 50000,          },
        { BQ2575x::VAC_DPM,                     0x08,   2, 13,  BQ_ALL, 20000,          },
        { BQ2575x::IAC_REV,                     0x0A,   2, 10,  BQ_ALL, 50000,          },
        { BQ2575x::VAC_REV,                     0x0C,   2, 13,  BQ_ALL, 20000,          },
        { BQ2575x::IPRECHG,                     0x10,   2,  9,  BQ_6,   50000,          },
        { BQ2575x::ITERM,                       0x12,   2,  9,  BQ_6,   50000,          },
        { BQ2575x::EN_TERM,                     0x14,   3,  3,  BQ_6,                   },
        { BQ2575x::VBAT_LOWV,                   0x14,   1,  2,  BQ_6,                   },
        { BQ2575x::EN_PRECHG,                   0x14,   0,  0,  BQ_6,                   },
        { BQ2575x::TOPOFF_TMR,                  0x15,   6,  7,  BQ_6,                   },
        { BQ2575x::WATCHDOG,                    0x15,   4,  5,  BQ_ALL,                 },
        { BQ2575x::EN_CHG_TMR,                  0x15,   3,  3,  BQ_6,                   },
        { BQ2575x::CHG_TMR,                     0x15,   1,  2,  BQ_6,                   },
        { BQ2575x::EN_TMR2X,                    0x15,   0,  0,  BQ_6,                   },
        { BQ2575x::CV_TMR,                      0x16,   0,  3,  BQ_6,                   },
        { BQ2575x::VRECHG,                      0x17,   6,  7,  BQ_6,                   },
        { BQ2575x::WD_RST,                      0x17,   5,  5,  BQ_ALL,                 },
        { BQ2575x::DIS_CE_PIN,                  0x17,   4,  4,  BQ_ALL,                 },
        { BQ2575x::EN_CHG_BIT_RESET_BEHAVIOR,   0x17,   3,  3,  BQ_ALL,                 },
        { BQ2575x::EN_HIZ,                      0x17,   2,  2,  BQ_ALL,                 },
        { BQ2575x::EN_IBAT_LOAD,                0x17,   1,  1,  BQ_ALL,                 },
        { BQ2575x::EN_CHG,                      0x17,   0,  0,  BQ_ALL,                 },
        { BQ2575x::EN_IOUT_PIN,                 0x18,   7,  7,  BQ_ALL,                 }, // EN_ICHG_PIN
        { BQ2575x::EN_IIN_PIN,                  0x18,   6,  6,  BQ_ALL,                 }, // EN_ILIM_HIZ_PIN
        { BQ2575x::DIS_PG_PIN,                  0x18,   5,  5,  BQ_ALL,                 },
        { BQ2575x::DIS_STAT_PIN,                0x18,   4,  4,  BQ_ALL,                 },
        { BQ2575x::FORCE_STAT4_ON,              0x18,   3,  3,  BQ_ALL,                 },
        { BQ2575x::FORCE_STAT3_ON,              0x18,   2,  2,  BQ_ALL,                 },
        { BQ2575x::FORCE_STAT2_ON,              0x18,   1,  1,  BQ_6,                   },
        { BQ2575x::FORCE_STAT_ON,               0x18,   0,  0,  BQ_ALL,                 },
        { BQ2575x::REG_RST,                     0x19,   7,  7,  BQ_ALL,                 },
        { BQ2575x::EN_IAC_LOAD,                 0x19,   6,  6,  BQ_ALL,                 },
        { BQ2575x::EN_PFM,                      0x19,   5,  5,  BQ_ALL,                 },
        { BQ2575x::PWRPATH_REDUCE_VDRV,         0x19,   3,  3,  BQ_8,                   },
        { BQ2575x::EN_REV,                      0x19,   0,  0,  BQ_ALL,                 },
        { BQ2575x::FORCE_SWEEP,                 0x1A,   7,  7,  BQ_6,                   },
        { BQ2575x::P_AND_O_TMR,                 0x1A,   5,  6,  BQ_6,                   },
        { BQ2575x::FULL_SWEEP_TMR,              0x1A,   1,  2,  BQ_6,                   },
        { BQ2575x::EN_MPPT,                     0x1A,   0,  0,  BQ_6,                   },
        { BQ2575x::TS_T5,                       0x1B,   6,  7,  BQ_ALL,                 },
        { BQ2575x::TS_T3,                       0x1B,   4,  5,  BQ_6,                   },
        { BQ2575x::TS_T2,                       0x1B,   2,  3,  BQ_6,                   },
        { BQ2575x::TS_T1,                       0x1B,   0,  1,  BQ_6,                   },
        { BQ2575x::JEITA_VSET,                  0x1C,   5,  6,  BQ_6,                   },
        { BQ2575x::JEITA_ISETH,                 0x1C,   4,  4,  BQ_6,                   },
        { BQ2575x::JEITA_ISETC,                 0x1C,   2,  3,  BQ_6,                   },
        { BQ2575x::EN_JEITA,                    0x1C,   1,  1,  BQ_6,                   },
        { BQ2575x::EN_TS,                       0x1C,   0,  0,  BQ_6,                   },
        { BQ2575x::BHOT,                        0x1D,   6,  7,  BQ_ALL,                 },
        { BQ2575x::BCOLD,                       0x1D,   5,  5,  BQ_ALL,                 },
        { BQ2575x::TOVLD_SET,                   0x1E,   6,  6,  BQ_8,                   },
        { BQ2575x::SYSREV_UV,                   0x1E,   5,  5,  BQ_ALL,                 },
        { BQ2575x::EN_BYPASS,                   0x1E,   4,  4,  BQ_8,                   },
        { BQ2575x::EN_OVLD_TMAX,                0x1E,   3,  3,  BQ_8,                   },
        { BQ2575x::EN_OVLD_3L,                  0x1E,   2,  2,  BQ_8,                   },
        { BQ2575x::OVLD_ILIM2,                  0x1E,   1,  1,  BQ_8,                   },
        { BQ2575x::EN_OVLD,                     0x1E,   0,  0,  BQ_8,                   },
        { BQ2575x::VAC_MPP,                     0x1F,   2, 13,  BQ_6,   20000,          },
        { BQ2575x::ADC_DONE_STAT,               0x21,   7,  7,  BQ_ALL,                 },
        { BQ2575x::IAC_DPM_STAT,                0x21,   6,  6,  BQ_ALL,                 },
        { BQ2575x::VAC_DPM_STAT,                0x21,   5,  5,  BQ_ALL,                 },
        { BQ2575x::WD_STAT,                     0x21,   3,  3,  BQ_ALL,                 },
        { BQ2575x::CHARGE_STAT,                 0x21,   0,  2,  BQ_ALL,                 },
        { BQ2575x::PG_STAT,                     0x22,   7,  7,  BQ_ALL,                 },
        { BQ2575x::TS_STAT,                     0x22,   4,  6,  BQ_ALL,                 },
        { BQ2575x::MPPT_STAT,                   0x22,   0,  1,  BQ_6,                   },
        { BQ2575x::FSW_SYNC_STAT,               0x23,   4,  5,  BQ_ALL,                 },
        { BQ2575x::CV_TMR_STAT,                 0x23,   3,  3,  BQ_6,                   },
        { BQ2575x::REVERSE_STAT,                0x23,   2,  2,  BQ_ALL,                 },
        { BQ2575x::VAC_UV_STAT,                 0x24,   7,  7,  BQ_ALL,                 },
        { BQ2575x::VAC_OV_STAT,                 0x24,   6,  6,  BQ_ALL,                 },
        { BQ2575x::IBAT_OCP_STAT,               0x24,   5,  5,  BQ_ALL,                 },
        { BQ2575x::VBAT_OV_STAT,                0x24,   4,  4,  BQ_ALL,                 },
        { BQ2575x::TSHUT_STAT,                  0x24,   3,  3,  BQ_ALL,                 },
        { BQ2575x::CHG_TMR_STAT,                0x24,   2,  2,  BQ_6,                   },
        { BQ2575x::DRV_OKZ_STAT,                0x24,   1,  1,  BQ_ALL,                 },
        { BQ2575x::ADC_DONE_FLAG,               0x25,   7,  7,  BQ_ALL,                 },
        { BQ2575x::IAC_DPM_FLAG,                0x25,   6,  6,  BQ_ALL,                 },
        { BQ2575x::VAC_DPM_FLAG,                0x25,   5,  5,  BQ_ALL,                 },
        { BQ2575x::WD_FLAG,                     0x25,   3,  3,  BQ_ALL,                 },
        { BQ2575x::CV_TMR_FLAG,                 0x25,   1,  1,  BQ_6,                   },
        { BQ2575x::CHARGE_FLAG,                 0x25,   0,  0,  BQ_ALL,                 },
        { BQ2575x::PG_FLAG,                     0x26,   7,  7,  BQ_ALL,                 },
        { BQ2575x::TS_FLAG,                     0x26,   4,  4,  BQ_ALL,                 },
        { BQ2575x::REVERSE_FLAG,                0x26,   3,  3,  BQ_ALL,                 },
        { BQ2575x::FSW_SYNC_FLAG,               0x26,   1,  1,  BQ_ALL,                 },
        { BQ2575x::MPPT_FLAG,                   0x26,   0,  0,  BQ_6,                   },
        { BQ2575x::VAC_UV_FLAG,                 0x27,   7,  7,  BQ_ALL,                 },
        { BQ2575x::VAC_OV_FLAG,                 0x27,   6,  6,  BQ_ALL,                 },
        { BQ2575x::IBAT_OCP_FLAG,               0x27,   5,  5,  BQ_ALL,                 },
        { BQ2575x::VBAT_OV_FLAG,                0x27,   4,  4,  BQ_ALL,                 },
        { BQ2575x::TSHUT_FLAG,                  0x27,   3,  3,  BQ_ALL,                 },
        { BQ2575x::CHG_TMR_FLAG,                0x27,   2,  2,  BQ_6,                   },
        { BQ2575x::DRV_OKZ_FLAG,                0x27,   1,  1,  BQ_ALL,                 },
        { BQ2575x::ADC_DONE_MASK,               0x28,   7,  7,  BQ_ALL,                 },
        { BQ2575x::IAC_DPM_MASK,                0x28,   6,  6,  BQ_ALL,                 },
        { BQ2575x::VAC_DPM_MASK,                0x28,   5,  5,  BQ_ALL,                 },
        { BQ2575x::WD_MASK,                     0x28,   3,  3,  BQ_ALL,                 },
        { BQ2575x::CV_TMR_MASK,                 0x28,   1,  1,  BQ_6,                   },
        { BQ2575x::CHARGE_MASK,                 0x28,   0,  0,  BQ_ALL,                 },
        { BQ2575x::PG_MASK,                     0x29,   7,  7,  BQ_ALL,                 },
        { BQ2575x::TS_MASK,                     0x29,   4,  4,  BQ_ALL,                 },
        { BQ2575x::REVERSE_MASK,                0x29,   3,  3,  BQ_ALL,                 },
        { BQ2575x::FSW_SYNC_MASK,               0x29,   1,  1,  BQ_ALL,                 },
        { BQ2575x::MPPT_MASK,                   0x29,   0,  0,  BQ_6,                   },
        { BQ2575x::VAC_UV_MASK,                 0x2A,   7,  7,  BQ_ALL,                 },
        { BQ2575x::VAC_OV_MASK,                 0x2A,   6,  6,  BQ_ALL,                 },
        { BQ2575x::IBAT_OCP_MASK,               0x2A,   5,  5,  BQ_ALL,                 },
        { BQ2575x::VBAT_OV_MASK,                0x2A,   4,  4,  BQ_ALL,                 },
        { BQ2575x::TSHUT_MASK,                  0x2A,   3,  3,  BQ_ALL,                 },
        { BQ2575x::CHG_TMR_MASK,                0x2A,   2,  2,  BQ_6,                   },
        { BQ2575x::DRV_OKZ_MASK,                0x2A,   1,  1,  BQ_ALL,                 },
        { BQ2575x::ADC_EN,                      0x2B,   7,  7,  BQ_ALL,                 },
        { BQ2575x::ADC_RATE,                    0x2B,   6,  6,  BQ_ALL,                 },
        { BQ2575x::ADC_SAMPLE,                  0x2B,   4,  5,  BQ_ALL,                 },
        { BQ2575x::ADC_AVG,                     0x2B,   3,  3,  BQ_ALL,                 },
        { BQ2575x::ADC_AVG_INIT,                0x2B,   2,  2,  BQ_ALL,                 },
        { BQ2575x::IAC_ADC_DIS,                 0x2C,   7,  7,  BQ_ALL,                 },
        { BQ2575x::IOUT_ADC_DIS,                0x2C,   6,  6,  BQ_ALL,                 }, // IBAT_ADC_DIS
        { BQ2575x::VAC_ADC_DIS,                 0x2C,   5,  5,  BQ_ALL,                 },
        { BQ2575x::VOUT_ADC_DIS,                0x2C,   4,  4,  BQ_ALL,                 }, // VBAT_ADC_DIS
        { BQ2575x::TS_ADC_DIS,                  0x2C,   2,  2,  BQ_ALL,                 },
        { BQ2575x::VFB_ADC_DIS,                 0x2C,   1,  1,  BQ_6,                   },
        { BQ2575x::IAC_ADC,                     0x2D,   0, 15,  BQ_ALL, 800,            }, // uA
        { BQ2575x::IOUT_ADC,                    0x2F,   0, 15,  BQ_ALL, 2000,           }, // IBAT_ADC
        { BQ2575x::VAC_ADC,                     0x31,   0, 15,  BQ_ALL, 2000,           },
        { BQ2575x::VOUT_ADC,                    0x33,   0, 15,  BQ_ALL, 2000,           }, // VBAT_ADC
        { BQ2575x::TS_ADC,                      0x37,   0, 15,  BQ_ALL,                 },
        { BQ2575x::VFB_ADC,                     0x39,   0, 15,  BQ_6,   1000,           },
        { BQ2575x::BOOST_HS_DRV,                0x3B,   6,  7,  BQ_ALL,                 },
        { BQ2575x::BUCK_HS_DRV,                 0x3B,   4,  5,  BQ_ALL,                 },
        { BQ2575x::BOOST_LS_DRV,                0x3B,   2,  3,  BQ_ALL,                 },
        { BQ2575x::BUCK_LS_DRV,                 0x3B,   0,  1,  BQ_ALL,                 },
        { BQ2575x::BOOST_DEAD_TIME,             0x3C,   2,  3,  BQ_ALL,                 },
        { BQ2575x::BUCK_DEAD_TIME,              0x3C,   0,  1,  BQ_ALL,                 },
        { BQ2575x::PART_NUM,                    0x3D,   3,  5,  BQ_ALL,                 },
        { BQ2575x::DEV_REV,                     0x3D,   0,  2,  BQ_ALL,                 },
        { BQ2575x::IBAT_REV,                    0x62,   6,  7,  BQ_ALL,                 },
        { BQ2575x::EN_CONV_FAST_TRANSIENT,      0x62,   1,  1,  BQ_ALL,                 },
    };
    #pragma pack(pop)

    // static constexpr int len = sizeof(regFields);
    static_assert(REG_FIELDS_ID_NUM == sizeof(regFields) / sizeof(regFields[0]), "Length mismatch between struct array and enum, check association. (Index of array must equal ID enum value)");
    // extra checks had to be left in the .cpp



    protected:
    static const uint8_t I2C_ADDR = 0x6B;   // 7-bit device address

    int (*_i2c_read)(uint16_t devAddr, uint8_t regStart, uint8_t *buf, size_t numBytes);
    int (*_i2c_write)(uint16_t devAddr, uint8_t regStart, const uint8_t *buf, size_t numBytes);

    uint16_t _i2c_read_reg(uint8_t reg, bool isShort = false, uint16_t devAddr = I2C_ADDR) const {
        uint16_t buf = 0;
        int ret = _i2c_read(devAddr, reg, (uint8_t*)&buf, isShort + 1);
        if (ret == 0) {
            return buf;
        }
        else {
            // TODO: error
            return 0xFF;
        }
    }

    int _i2c_write_reg(uint8_t reg, uint16_t value, bool isShort = false, uint16_t devAddr = I2C_ADDR) const {
        return _i2c_write(devAddr, reg, (uint8_t*)&value, isShort + 1);
    }

};
