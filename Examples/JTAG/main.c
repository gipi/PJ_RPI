/*
 * Simple program to set the alternate function correctly to use
 * the JTAG on rpi.
 */
#include "PJ_RPI.h"
#include <stdio.h>

void set_alt_for(int g, int alt) {
    short value_old = GET_GPIO_ALT(g);

    INP_GPIO(g);
    SET_GPIO_ALT(g, alt);

    printf("gpio %d %d -> %d\n", g, value_old, alt);
}

int main() {
	if(map_peripheral(&gpio) == -1) {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        return -1;
    }

    printf("Setting pins for TCK, TDO, TDI, TMS\n");

    set_alt_for(24, 4); /* TDO */
    set_alt_for(25, 4); /* TCK */
    set_alt_for(26, 4); /* TDI */
    set_alt_for(27, 4); /* TMS */

    return 0;	
}
