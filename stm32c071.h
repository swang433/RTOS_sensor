#ifndef STM32C071_H
#define STM32C071_H

#include <stdint.h>

/* Register definitions for STM32C071 (RM0490). ADC and I2C are not defined here. */

#define REG32(addr)     (*(volatile uint32_t *)(addr))

/* ------------------------------------------------------------------ */
/* RCC                                                                 */
/* ------------------------------------------------------------------ */
#define RCC_BASE        0x40021000U
#define RCC_CR          REG32(RCC_BASE + 0x00)
#define RCC_ICSCR       REG32(RCC_BASE + 0x04)
#define RCC_CFGR        REG32(RCC_BASE + 0x08)
#define RCC_CIER        REG32(RCC_BASE + 0x18)
#define RCC_CIFR        REG32(RCC_BASE + 0x1C)
#define RCC_CICR        REG32(RCC_BASE + 0x20)
#define RCC_IOPRSTR     REG32(RCC_BASE + 0x24)
#define RCC_AHBRSTR     REG32(RCC_BASE + 0x28)
#define RCC_APBRSTR1    REG32(RCC_BASE + 0x2C)
#define RCC_APBRSTR2    REG32(RCC_BASE + 0x30)
#define RCC_IOPENR      REG32(RCC_BASE + 0x34)
#define RCC_AHBENR      REG32(RCC_BASE + 0x38)
#define RCC_APBENR1     REG32(RCC_BASE + 0x3C)
#define RCC_APBENR2     REG32(RCC_BASE + 0x40)
#define RCC_CCIPR       REG32(RCC_BASE + 0x54)
#define RCC_CSR1        REG32(RCC_BASE + 0x5C)
#define RCC_CSR2        REG32(RCC_BASE + 0x60)

/* RCC_IOPENR */
#define RCC_IOPENR_GPIOAEN      (1U << 0)
#define RCC_IOPENR_GPIOBEN      (1U << 1)
#define RCC_IOPENR_GPIOCEN      (1U << 2)
#define RCC_IOPENR_GPIODEN      (1U << 3)
#define RCC_IOPENR_GPIOFEN      (1U << 5)

/* RCC_APBENR1 */
#define RCC_APBENR1_TIM2EN      (1U << 0)
#define RCC_APBENR1_TIM3EN      (1U << 1)
#define RCC_APBENR1_RTCAPBEN    (1U << 10)
#define RCC_APBENR1_WWDGEN      (1U << 11)
#define RCC_APBENR1_USART2EN    (1U << 17)
#define RCC_APBENR1_DBGEN       (1U << 27)
#define RCC_APBENR1_PWREN       (1U << 28)

/* RCC_APBENR2 */
#define RCC_APBENR2_SYSCFGEN    (1U << 0)

/* ------------------------------------------------------------------ */
/* GPIO (same layout for every port)                                   */
/* ------------------------------------------------------------------ */
#define GPIOA_BASE      0x50000000U
#define GPIOB_BASE      0x50000400U
#define GPIOC_BASE      0x50000800U
#define GPIOD_BASE      0x50000C00U
#define GPIOF_BASE      0x50001400U

#define GPIO_MODER(p)   REG32((p) + 0x00)
#define GPIO_OTYPER(p)  REG32((p) + 0x04)
#define GPIO_OSPEEDR(p) REG32((p) + 0x08)
#define GPIO_PUPDR(p)   REG32((p) + 0x0C)
#define GPIO_IDR(p)     REG32((p) + 0x10)
#define GPIO_ODR(p)     REG32((p) + 0x14)
#define GPIO_BSRR(p)    REG32((p) + 0x18)
#define GPIO_LCKR(p)    REG32((p) + 0x1C)
#define GPIO_AFRL(p)    REG32((p) + 0x20)
#define GPIO_AFRH(p)    REG32((p) + 0x24)
#define GPIO_BRR(p)     REG32((p) + 0x28)

#define GPIOA_MODER     GPIO_MODER(GPIOA_BASE)
#define GPIOA_OTYPER    GPIO_OTYPER(GPIOA_BASE)
#define GPIOA_OSPEEDR   GPIO_OSPEEDR(GPIOA_BASE)
#define GPIOA_PUPDR     GPIO_PUPDR(GPIOA_BASE)
#define GPIOA_IDR       GPIO_IDR(GPIOA_BASE)
#define GPIOA_ODR       GPIO_ODR(GPIOA_BASE)
#define GPIOA_BSRR      GPIO_BSRR(GPIOA_BASE)
#define GPIOA_AFRL      GPIO_AFRL(GPIOA_BASE)
#define GPIOA_AFRH      GPIO_AFRH(GPIOA_BASE)

#define GPIOC_MODER     GPIO_MODER(GPIOC_BASE)
#define GPIOC_PUPDR     GPIO_PUPDR(GPIOC_BASE)
#define GPIOC_IDR       GPIO_IDR(GPIOC_BASE)

/* MODER: 2 bits per pin */
#define GPIO_MODE_INPUT     0x0U
#define GPIO_MODE_OUTPUT    0x1U
#define GPIO_MODE_AF        0x2U
#define GPIO_MODE_ANALOG    0x3U    /* reset state of most pins */

/* PUPDR: 2 bits per pin */
#define GPIO_PULL_NONE      0x0U
#define GPIO_PULL_UP        0x1U
#define GPIO_PULL_DOWN      0x2U

/* Alternate functions used by this project */
#define GPIO_AF1_USART2     0x1U    /* PA2 = USART2_TX, PA3 = USART2_RX */

/* NUCLEO-C071RB pins (check the board user manual for your revision) */
#define LED_PIN         5U      /* LD1 on PA5 */
#define BUTTON_PIN      13U     /* B1 on PC13, active low */
#define UART_TX_PIN     2U      /* PA2 */
#define UART_RX_PIN     3U      /* PA3 */

/* ------------------------------------------------------------------ */
/* USART2                                                              */
/* ------------------------------------------------------------------ */
#define USART2_BASE     0x40004400U
#define USART2_CR1      REG32(USART2_BASE + 0x00)
#define USART2_CR2      REG32(USART2_BASE + 0x04)
#define USART2_CR3      REG32(USART2_BASE + 0x08)
#define USART2_BRR      REG32(USART2_BASE + 0x0C)
#define USART2_GTPR     REG32(USART2_BASE + 0x10)
#define USART2_RTOR     REG32(USART2_BASE + 0x14)
#define USART2_RQR      REG32(USART2_BASE + 0x18)
#define USART2_ISR      REG32(USART2_BASE + 0x1C)
#define USART2_ICR      REG32(USART2_BASE + 0x20)
#define USART2_RDR      REG32(USART2_BASE + 0x24)
#define USART2_TDR      REG32(USART2_BASE + 0x28)
#define USART2_PRESC    REG32(USART2_BASE + 0x2C)

/* USART_CR1 */
#define USART_CR1_UE        (1U << 0)
#define USART_CR1_RE        (1U << 2)
#define USART_CR1_TE        (1U << 3)
#define USART_CR1_IDLEIE    (1U << 4)
#define USART_CR1_RXNEIE    (1U << 5)
#define USART_CR1_TCIE      (1U << 6)
#define USART_CR1_TXEIE     (1U << 7)
#define USART_CR1_FIFOEN    (1U << 29)

/* USART_ISR */
#define USART_ISR_PE        (1U << 0)
#define USART_ISR_FE        (1U << 1)
#define USART_ISR_NE        (1U << 2)
#define USART_ISR_ORE       (1U << 3)
#define USART_ISR_IDLE      (1U << 4)
#define USART_ISR_RXNE      (1U << 5)
#define USART_ISR_TC        (1U << 6)
#define USART_ISR_TXE       (1U << 7)

/* USART_ICR (write 1 to clear) */
#define USART_ICR_PECF      (1U << 0)
#define USART_ICR_FECF      (1U << 1)
#define USART_ICR_NECF      (1U << 2)
#define USART_ICR_ORECF     (1U << 3)
#define USART_ICR_IDLECF    (1U << 4)
#define USART_ICR_TCCF      (1U << 6)

/* 12 MHz / 115200 = 104 (about 0.16% error) */
#define USART2_BRR_115200   104U

/* UART functions */
void uart_init();
char uart_read(void); 
void uart_write(char c); 
void uart_write_chunk(const char* st); 

/* ------------------------------------------------------------------ */
/* EXTI (buttons). On the C0 the port select lives in EXTI, not SYSCFG */
/* ------------------------------------------------------------------ */
#define EXTI_BASE       0x40021800U
#define EXTI_RTSR1      REG32(EXTI_BASE + 0x00)
#define EXTI_FTSR1      REG32(EXTI_BASE + 0x04)
#define EXTI_SWIER1     REG32(EXTI_BASE + 0x08)
#define EXTI_RPR1       REG32(EXTI_BASE + 0x0C)    /* rising pending, write 1 to clear */
#define EXTI_FPR1       REG32(EXTI_BASE + 0x10)    /* falling pending, write 1 to clear */
#define EXTI_EXTICR1    REG32(EXTI_BASE + 0x60)    /* lines 0-3 */
#define EXTI_EXTICR2    REG32(EXTI_BASE + 0x64)    /* lines 4-7 */
#define EXTI_EXTICR3    REG32(EXTI_BASE + 0x68)    /* lines 8-11 */
#define EXTI_EXTICR4    REG32(EXTI_BASE + 0x6C)    /* lines 12-15 */
#define EXTI_IMR1       REG32(EXTI_BASE + 0x80)
#define EXTI_EMR1       REG32(EXTI_BASE + 0x84)

/* EXTICRx: 8 bits per line */
#define EXTI_PORT_A     0x00U
#define EXTI_PORT_B     0x01U
#define EXTI_PORT_C     0x02U
#define EXTI_PORT_D     0x03U
#define EXTI_PORT_F     0x05U

/* ------------------------------------------------------------------ */
/* TIM3 (buzzer PWM)                                                   */
/* ------------------------------------------------------------------ */
#define TIM3_BASE       0x40000400U
#define TIM3_CR1        REG32(TIM3_BASE + 0x00)
#define TIM3_CR2        REG32(TIM3_BASE + 0x04)
#define TIM3_DIER       REG32(TIM3_BASE + 0x0C)
#define TIM3_SR         REG32(TIM3_BASE + 0x10)
#define TIM3_EGR        REG32(TIM3_BASE + 0x14)
#define TIM3_CCMR1      REG32(TIM3_BASE + 0x18)
#define TIM3_CCMR2      REG32(TIM3_BASE + 0x1C)
#define TIM3_CCER       REG32(TIM3_BASE + 0x20)
#define TIM3_CNT        REG32(TIM3_BASE + 0x24)
#define TIM3_PSC        REG32(TIM3_BASE + 0x28)
#define TIM3_ARR        REG32(TIM3_BASE + 0x2C)
#define TIM3_CCR1       REG32(TIM3_BASE + 0x34)
#define TIM3_CCR2       REG32(TIM3_BASE + 0x38)
#define TIM3_CCR3       REG32(TIM3_BASE + 0x3C)
#define TIM3_CCR4       REG32(TIM3_BASE + 0x40)

#define TIM_CR1_CEN     (1U << 0)
#define TIM_CR1_ARPE    (1U << 7)
#define TIM_EGR_UG      (1U << 0)

/* ------------------------------------------------------------------ */
/* RTC                                                                 */
/* ------------------------------------------------------------------ */
#define RTC_BASE        0x40002800U
#define RTC_TR          REG32(RTC_BASE + 0x00)
#define RTC_DR          REG32(RTC_BASE + 0x04)
#define RTC_SSR         REG32(RTC_BASE + 0x08)
#define RTC_ICSR        REG32(RTC_BASE + 0x0C)
#define RTC_PRER        REG32(RTC_BASE + 0x10)
#define RTC_CR          REG32(RTC_BASE + 0x18)
#define RTC_WPR         REG32(RTC_BASE + 0x24)
#define RTC_SR          REG32(RTC_BASE + 0x50)
#define RTC_SCR         REG32(RTC_BASE + 0x5C)

#define RTC_ICSR_RSF    (1U << 5)
#define RTC_ICSR_INITF  (1U << 6)
#define RTC_ICSR_INIT   (1U << 7)
#define RTC_WPR_KEY1    0xCAU
#define RTC_WPR_KEY2    0x53U

/* ------------------------------------------------------------------ */
/* NVIC (Cortex-M0+ core)                                              */
/* ------------------------------------------------------------------ */
#define NVIC_ISER       REG32(0xE000E100U)
#define NVIC_ICER       REG32(0xE000E180U)
#define NVIC_ISPR       REG32(0xE000E200U)
#define NVIC_ICPR       REG32(0xE000E280U)
#define NVIC_IPR(n)     REG32(0xE000E400U + 4U * (n))  /* 4 IRQs per word; M0+ needs word access */

void nvic_set_priority(uint32_t irqn, uint32_t priority); 

/* IRQ numbers, matching the order in startup.s */
#define EXTI0_1_IRQn    5U
#define EXTI2_3_IRQn    6U
#define EXTI4_15_IRQn   7U
#define TIM3_IRQn       16U
#define USART2_IRQn     28U

#endif /* STM32C071_H */
