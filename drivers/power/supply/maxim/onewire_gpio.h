#ifndef __ONEWIRE_GPIO_H__
#define __ONEWIRE_GPIO_H__

/*
 * Serializes complete bus transactions. The per-slot spinlock in the bit
 * helpers cannot stop two callers from interleaving their bytes.
 */
void onewire_bus_lock(void);
void onewire_bus_unlock(void);

unsigned char ow_reset(void);
unsigned char read_byte(void);
void write_byte(char val);

#endif
