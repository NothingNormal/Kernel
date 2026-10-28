#ifndef __ONEWIRE_GPIO_H__
#define __ONEWIRE_GPIO_H__

/* Lock complete transactions in process context, not individual bit slots. */
void onewire_bus_lock(void);
void onewire_bus_unlock(void);

unsigned char ow_reset(void);
unsigned char read_byte(void);
void write_byte(char val);

#endif
