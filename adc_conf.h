#ifndef ADC_CONF_H_
#define ADC_CONF_H_

#define ADC_BUFF_SIZE 1024

void InitAdcDma(unsigned long *g_ucADCBuffPRI, unsigned long *g_ucADCBuffALT, void (*ADCIntHandler)(void));

#endif /* ADC_CONF_H_ */
