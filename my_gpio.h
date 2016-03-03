#ifndef MY_GPIO_H
#define MY_GPIO_H

#include <stdint.h>

#define MY_GPIO_SUCCESS 0
#define MY_GPIO_FAILURE -1
#define LOW 0
#define HIGH 1
#define READ 0
#define WRITE 1

#define FSEL_OFFSET 0
#define SET_OFFSET 7
#define CLR_OFFSET 10 
#define LEV_OFFSET 13

volatile uint32_t *gpio_mapped_addr;

int gpio_init(void);
void gpio_deinit(void);
void gpio_set_mode(int gpio_port, int mode);

static inline uint32_t gpio_read(const int gpio_port) {
	return (*(gpio_mapped_addr+LEV_OFFSET) & (1 << gpio_port)) > LOW;
}

static inline void gpio_write(const int gpio_port, const int value) {
	if (value) { // value == HIGH
		*(gpio_mapped_addr+SET_OFFSET) = 1 << gpio_port;
	} else { // value == LOW
		*(gpio_mapped_addr+CLR_OFFSET) = 1 << gpio_port;
	}
}

#endif