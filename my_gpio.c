#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include "my_gpio.h"

#define BCM2708_PERI_BASE   0x20000000
#define BCM2709_PERI_BASE   0x3f000000
#define GPIO_OFFSET         0x200000

#define GPIO_LENGTH 4096

int gpio_init(void)
{
    int mem_fd;
    uint32_t peri_base_addr = 0;
    uint32_t gpio_base_addr = 0;
    unsigned char buf[4];
    FILE *fp;
    char buffer[1024];
    char hardware[1024];
	
	// Inspired by code in the RPi.GPIO library at:
    // http://sourceforge.net/p/raspberry-gpio-python/
    // try /dev/gpiomem first since doesn't require root
    if ((mem_fd = open("/dev/gpiomem", O_RDWR|O_SYNC)) > 0)
    {
        gpio_mapped_addr = (uint32_t *)mmap(NULL, GPIO_LENGTH, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, 0);
        if ((uint32_t)gpio_mapped_addr < 0) {
            return MY_GPIO_FAILURE;
        } else {
            return MY_GPIO_SUCCESS;
        }
    }

    // fallback to /dev/mem method... sadly it requires root
    if ((fp = fopen("/proc/device-tree/soc/ranges", "rb")) != NULL) {
        fseek(fp, 4, SEEK_SET);
        if (fread(buf, 1, sizeof buf, fp) == sizeof buf) {
            peri_base_addr = buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3] << 0;
        }
        fclose(fp);
    } else {
        if ((fp = fopen("/proc/cpuinfo", "r")) == NULL)
            return MY_GPIO_FAILURE;

        while(!feof(fp) && peri_base_addr == 0) {
            fgets(buffer, sizeof(buffer), fp);
            sscanf(buffer, "Hardware	: %s", hardware);
            if (strcmp(hardware, "BCM2708") == 0 || strcmp(hardware, "BCM2835") == 0) {
                // RPi 1
                peri_base_addr = BCM2708_PERI_BASE;
				break;
            } else if (strcmp(hardware, "BCM2709") == 0 || strcmp(hardware, "BCM2836") == 0) {
                // RP2 2
                peri_base_addr = BCM2709_PERI_BASE;
				break;
            }
        }
        fclose(fp);
        if (peri_base_addr == 0)
            return MY_GPIO_FAILURE;
    }

    gpio_base_addr = peri_base_addr + GPIO_OFFSET;

    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
        return MY_GPIO_FAILURE;

    gpio_mapped_addr = (uint32_t *)mmap(NULL, GPIO_LENGTH, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, gpio_base_addr);

    if ((uint32_t)gpio_mapped_addr < 0)
        return MY_GPIO_FAILURE;

    return MY_GPIO_SUCCESS;
}

void gpio_deinit(void) {
	munmap(gpio_mapped_addr, GPIO_LENGTH);
}

void gpio_set_mode(int gpio_port, int mode) {
	*(gpio_mapped_addr+((gpio_port)/10)) &= ~(7<<(((gpio_port)%10)*3));
	if (mode) // mode == OUTPUT
		*(gpio_mapped_addr+((gpio_port)/10)) |=  (1<<(((gpio_port)%10)*3));
}
