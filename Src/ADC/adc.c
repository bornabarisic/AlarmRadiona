#include "stm32f446xx.h"
#include "stm32f4xx_hal.h"

#include "adc.h"

/* Globalne varijable */
static ADC_HandleTypeDef adc_handle;
static DMA_HandleTypeDef dma_handle;

void initialize_adc(void)
{
   __HAL_RCC_ADC1_CLK_ENABLE();
   
   adc_handle.Instance                    = ADC1;
   adc_handle.Init.ClockPrescaler         = ADC_CLOCK_SYNC_PCLK_DIV2;
   adc_handle.Init.Resolution             = ADC_RESOLUTION_12B;
   adc_handle.Init.DataAlign              = ADC_DATAALIGN_RIGHT;
   adc_handle.Init.ScanConvMode           = DISABLE;
   adc_handle.Init.EOCSelection           = ADC_EOC_SINGLE_CONV;
   adc_handle.Init.ContinuousConvMode     = DISABLE;
   adc_handle.Init.NbrOfConversion        = 1;
   adc_handle.Init.DiscontinuousConvMode  = DISABLE;
   adc_handle.Init.NbrOfDiscConversion    = 0;
   adc_handle.Init.ExternalTrigConv       = ADC_EXTERNALTRIGCONV_T3_TRGO;
   adc_handle.Init.ExternalTrigConvEdge   = ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING;
   adc_handle.Init.DMAContinuousRequests  = ENABLE;

   if (HAL_ADC_Init(&adc_handle) != HAL_OK)
   {
      while(1);
   }
   
   /* Potrebno konfigurirati DMA, prije svegqa treba vidjeti koji DMA je povezan sa ADC 1 */
   
}
