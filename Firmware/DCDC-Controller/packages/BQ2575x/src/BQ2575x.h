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
    constexpr void setValue(int32_t value);

    template <unsigned F>
    constexpr int32_t getValue();

    void setIoutReg(int32_t val);

    typedef struct RegisterField {
        uint8_t fieldId;
        uint8_t regAddress;
        // const char *regName;
        uint8_t startBit : 4;       // bit position of value
        uint8_t endBit : 4;         // inclusive
        uint8_t bqSeries;           // 
        int32_t factor;
        // Type?
        // uint8_t unitId : 3;
    } regField_t;
      
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



    protected:
    static const uint8_t I2C_ADDR = 0x6B;   // 7-bit device address
    int (*_i2c_read)(uint16_t devAddr, uint8_t regStart, uint8_t *buf, size_t numBytes);
    int (*_i2c_write)(uint16_t devAddr, uint8_t regStart, const uint8_t *buf, size_t numBytes);

    uint16_t _i2c_read_reg(uint8_t reg, bool isShort = false, uint16_t devAddr = I2C_ADDR) {
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

    int _i2c_write_reg(uint8_t reg, uint16_t value, bool isShort = false, uint16_t devAddr = I2C_ADDR) {
        return _i2c_write(devAddr, reg, (uint8_t*)&value, isShort + 1);
    }

};
