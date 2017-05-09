#ifndef __BOARD_CONF_H__
#define __BOARD_CONF_H__

#if defined(ccs)
extern void (*const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

typedef enum {
  NO_SW,
  SW1 = 0x1, /* SW1/Reset Button */
  SW2 = 0x2, /* SW2/GP22/Pin15 */
  SW3 = 0x4  /* SW3/GP13/Pin4 */

} eSwNum;

void BoardInit(void);

void GPIO_IF_Init(void (*SW2IntHandler)(void), void (*SW3IntHandler)(void));
void GPIO_IF_EnableInterrupt(unsigned char ucSwitch);
void GPIO_IF_DisableInterrupt(unsigned char ucSwitch);

#endif /* __BOARD_CONF_H__ */
