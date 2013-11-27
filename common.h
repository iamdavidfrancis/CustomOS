#ifndef COMMON_H
#define COMMON_H

#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default */
#endif
#include <stddef.h>
#include <stdint.h>

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
void memset(uint8_t *dest, uint8_t val, uint32_t len);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(const char *message, const char *file, u32int line);
extern void panic_assert(const char *file, u32int line, const char *desc);


#endif
