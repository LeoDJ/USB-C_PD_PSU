// TODO: Fix Currently need to manually override pushBlock in TFT_eSPI_Generic.c


#include <TFT_eSPI.h>
// #include <Processors/TFT_eSPI_Generic.h>
// #include <Arduino.h>


extern TFT_eSPI tft;


// class TFT_eSPI_CH32 : public TFT_eSPI {
class CH32_DMA {
    public:

    static void DMA_Tx_Init(DMA_Channel_TypeDef *DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize, bool memInc = true) {
        DMA_InitTypeDef DMA_InitStructure = {0};

        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

        DMA_DeInit(DMA_CHx);

        DMA_InitStructure.DMA_PeripheralBaseAddr = ppadr;
        DMA_InitStructure.DMA_MemoryBaseAddr = memadr;
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
        DMA_InitStructure.DMA_BufferSize = bufsize;
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc = memInc ? DMA_MemoryInc_Enable : DMA_MemoryInc_Disable;    
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
        DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
        DMA_Init(DMA_CHx, &DMA_InitStructure);
    }


    static void pushBlock(uint16_t color, uint32_t len) {
        const uint32_t CHUNK_SIZE = UINT16_MAX;
        int chunks = (len / CHUNK_SIZE) + 1;
        for (int i = 0; i < chunks ; i++) {
            uint16_t toTransfer = (i != chunks - 1) ? CHUNK_SIZE : len % CHUNK_SIZE;
            initDMA(0);
            DMA_Tx_Init(DMA1_Channel3, (uint32_t)&SPI1->DATAR, (uint32_t)&color, toTransfer, false);
            DMA_Cmd(DMA1_Channel3, ENABLE);
            dmaWait();
            deInitDMA();
        }
    }

    // static void pushPixels(const void* data_in, uint32_t len) {
    //     uint16_t *data = (uint16_t*)data_in;

    // }

    static void initDMA(bool ctrl_cs) {
        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
        SPI_DataSizeConfig(SPI1, SPI_DataSize_16b); // need to set to 16 bit transfer mode, because Arduino obv only uses 8
    }

    static void deInitDMA(void) {
        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
        SPI_DataSizeConfig(SPI1, SPI_DataSize_8b);
    }

    static bool dmaBusy(void) {
        return !DMA_GetFlagStatus(DMA1_FLAG_TC3) || SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY);
    }


    static void dmaWait(void) {
        // block until dma transfer done
        pinMode(PA0, OUTPUT);   // TODO: remove after testing
        digitalWrite(PA0, 0);
        while(dmaBusy());
        digitalWrite(PA0, 1);
        delayMicroseconds(100);
    }

};