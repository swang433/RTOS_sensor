#include "stm32c071.h"

#define GPIOA_EN        (1U << 0)
#define USART2_EN       (1U << 17)
#define RT_TX_BIT       ((1U << 2) | (1U << 3))
#define RX_TX_EN        RT_TX_BIT
#define UART_MODE       ((1U << 4) | (1U << 6))
#define UART_AF_MASK    ((1U << 8) | (1U << 12))

void nvic_set_priority(uint32_t irqn, uint32_t priority);{
    uint32_t reg_num = irqn >> 2; 
    uint32_t byte_pos = irqn & (3U); 
    uint32_t shift     = (byte_pos * 8) + 6; 

    uint32_t val = NVIC_IPR[reg_num];     
    val &= ~(0xFFUL << (byte_pos * 8));           // Clear the 8-bit field
    val |= ((priority & 0x03UL) << shift);        // Set the 2 priority bits
    NVIC_IPR[reg_idx] = val;
}

void uart_init(void){
    //TODO
    RCC_IOPENR  |= RCC_IOPENR_GPIOAEN;
    RCC_APBENR1 |= RCC_APBENR1_USART2EN;

    GPIOA_MODER &= ~(0xFU << 4);                 // PA2, PA3 fields
    GPIOA_MODER |=  (0xAU << 4);                 // 10 10 = AF
    GPIOA_AFRL  &= ~(0xFFU << 8);
    GPIOA_AFRL  |=  (0x11U << 8);                // AF1, AF1

    USART2_BRR = USART2_BRR_115200;
    USART2_CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;
    USART2_CR1 |= USART_CR1_UE;

    nvic_set_priority(USART2_IRQn, 3);
    NVIC_ISER = 1U << USART2_IRQn;
}

char uart_read(void){
    //TODO
}

void uart_write(void){
    //TODO
}

void uart_write_chunk(const char* s){
    //TODO
}