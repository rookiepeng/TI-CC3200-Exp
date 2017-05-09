#include <hw_types.h>
#include <hw_memmap.h>
#include <hw_common_reg.h>
#include <hw_ints.h>
#include <hw_mcspi.h>
#include <rom_map.h>
#include <udma.h>
#include <udma_if.h>
#include <utils.h>
#include <interrupt.h>
#include <prcm.h>

#include "pinmux.h"
#include "board_conf.h"

int main(void) {

    BoardInit();// Initialize Board configurations
    PinMuxConfig();
	
	return 0;
}
