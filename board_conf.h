#ifndef __BOARD_CONF_H__
#define __BOARD_CONF_H__

#if defined(ccs)
extern void (*const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

void BoardInit(void);

#endif /* __BOARD_CONF_H__ */
