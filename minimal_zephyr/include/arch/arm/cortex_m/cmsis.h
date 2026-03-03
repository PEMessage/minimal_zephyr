/*
 * Copyright (c) 2024
 * Minimal Zephyr - ARM Cortex-M CMSIS Header
 */

#ifndef MINIMAL_CMSIS_H
#define MINIMAL_CMSIS_H

#include <stdint.h>

/* IRQ numbers for Cortex-M3 */
#define NonMaskableInt_IRQn     (-14)
#define MemoryManagement_IRQn   (-12)
#define BusFault_IRQn           (-11)
#define UsageFault_IRQn         (-10)
#define SVCall_IRQn             (-5)
#define DebugMonitor_IRQn       (-4)
#define PendSV_IRQn             (-2)
#define SysTick_IRQn            (-1)

/* NVIC registers */
typedef struct {
    volatile uint32_t ISER[8];      /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register */
    uint32_t RESERVED0[24];
    volatile uint32_t ICER[8];      /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register */
    uint32_t RSERVED1[24];
    volatile uint32_t ISPR[8];      /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register */
    uint32_t RESERVED2[24];
    volatile uint32_t ICPR[8];      /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register */
    uint32_t RESERVED3[24];
    volatile uint32_t IABR[8];      /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register */
    uint32_t RESERVED4[56];
    volatile uint8_t  IP[240];      /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
    uint32_t RESERVED5[644];
    volatile  uint32_t STIR;        /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register */
}  NVIC_Type;

/* SCB registers */
typedef struct {
    volatile const uint32_t CPUID;  /*!< Offset: 0x00 (R/ )  CPUID Base Register */
    volatile uint32_t ICSR;         /*!< Offset: 0x04 (R/W)  Interrupt Control and State Register */
    volatile uint32_t VTOR;         /*!< Offset: 0x08 (R/W)  Vector Table Offset Register */
    volatile uint32_t AIRCR;        /*!< Offset: 0x0C (R/W)  Application Interrupt and Reset Control Register */
    volatile uint32_t SCR;          /*!< Offset: 0x10 (R/W)  System Control Register */
    volatile uint32_t CCR;          /*!< Offset: 0x14 (R/W)  Configuration and Control Register */
    volatile uint8_t  SHP[12];      /*!< Offset: 0x18 (R/W)  System Handler Priority Register (4-7, 8-11, 12-15) */
    volatile uint32_t SHCSR;        /*!< Offset: 0x24 (R/W)  System Handler Control and State Register */
    volatile uint32_t CFSR;         /*!< Offset: 0x28 (R/W)  Configurable Fault Status Register */
    volatile uint32_t HFSR;         /*!< Offset: 0x2C (R/W)  HardFault Status Register */
    volatile uint32_t DFSR;         /*!< Offset: 0x30 (R/W)  Debug Fault Status Register */
    volatile uint32_t MMFAR;        /*!< Offset: 0x34 (R/W)  MemManage Fault Address Register */
    volatile uint32_t BFAR;         /*!< Offset: 0x38 (R/W)  BusFault Address Register */
    volatile uint32_t AFSR;         /*!< Offset: 0x3C (R/W)  Auxiliary Fault Status Register */
    volatile const uint32_t PFR[2]; /*!< Offset: 0x40 (R/ )  Processor Feature Register */
    volatile const uint32_t DFR;    /*!< Offset: 0x48 (R/ )  Debug Feature Register */
    volatile const uint32_t ADR;    /*!< Offset: 0x4C (R/ )  Auxiliary Feature Register */
    volatile const uint32_t MMFR[4];/*!< Offset: 0x50 (R/ )  Memory Model Feature Register */
    volatile const uint32_t ISAR[5];/*!< Offset: 0x60 (R/ )  Instruction Set Attributes Register */
    uint32_t RESERVED0[5];
    volatile uint32_t CPACR;        /*!< Offset: 0x88 (R/W)  Coprocessor Access Control Register */
} SCB_Type;

/* SysTick registers */
typedef struct {
    volatile uint32_t CTRL;         /*!< Offset: 0x00 (R/W)  SysTick Control and Status Register */
    volatile uint32_t LOAD;         /*!< Offset: 0x04 (R/W)  SysTick Reload Value Register */
    volatile uint32_t VAL;          /*!< Offset: 0x08 (R/W)  SysTick Current Value Register */
    volatile const uint32_t CALIB;  /*!< Offset: 0x0C (R/ )  SysTick Calibration Register */
} SysTick_Type;

/* Memory mapping of Cortex-M3 Hardware */
#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address */
#define ITM_BASE            (0xE0000000UL)                            /*!< ITM Base Address */
#define DWT_BASE            (0xE0001000UL)                            /*!< DWT Base Address */
#define TPI_BASE            (0xE0040000UL)                            /*!< TPI Base Address */
#define CoreDebug_BASE      (0xE000EDF0UL)                            /*!< Core Debug Base Address */
#define SysTick_BASE        (SCS_BASE + 0x0010UL)                     /*!< SysTick Base Address */
#define NVIC_BASE           (SCS_BASE + 0x0100UL)                     /*!< NVIC Base Address */
#define SCB_BASE            (SCS_BASE + 0x0D00UL)                     /*!< System Control Block Base Address */

#define SCnSCB              ((SCnSCB_Type *) SCS_BASE)                /*!< System control Register not in SCB */
#define SCB                 ((SCB_Type *) SCB_BASE)                   /*!< SCB configuration struct */
#define SysTick             ((SysTick_Type *) SysTick_BASE)           /*!< SysTick configuration struct */
#define NVIC                ((NVIC_Type *) NVIC_BASE)                 /*!< NVIC configuration struct */
#define ITM                 ((ITM_Type *) ITM_BASE)                   /*!< ITM configuration struct */
#define DWT                 ((DWT_Type *) DWT_BASE)                   /*!< DWT configuration struct */
#define TPI                 ((TPI_Type *) TPI_BASE)                   /*!< TPI configuration struct */
#define CoreDebug           ((CoreDebug_Type *) CoreDebug_BASE)       /*!< Core Debug configuration struct */

/* NVIC functions */
static inline void NVIC_EnableIRQ(uint32_t IRQn)
{
    NVIC->ISER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

static inline void NVIC_DisableIRQ(uint32_t IRQn)
{
    NVIC->ICER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

static inline uint32_t NVIC_GetPendingIRQ(uint32_t IRQn)
{
    return((uint32_t) ((NVIC->ISPR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F))) ? 1 : 0));
}

static inline void NVIC_SetPendingIRQ(uint32_t IRQn)
{
    NVIC->ISPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

static inline void NVIC_ClearPendingIRQ(uint32_t IRQn)
{
    NVIC->ICPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

static inline uint32_t NVIC_GetActive(uint32_t IRQn)
{
    return((uint32_t)((NVIC->IABR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F))) ? 1 : 0));
}

static inline void NVIC_SetPriority(uint32_t IRQn, uint32_t priority)
{
    if (IRQn < 0) {
        SCB->SHP[((uint32_t)(IRQn) & 0xF) - 4] = ((priority << (8 - 4)) & 0xFF);
    } else {
        NVIC->IP[(uint32_t)(IRQn)] = ((priority << (8 - 4)) & 0xFF);
    }
}

static inline uint32_t NVIC_GetPriority(uint32_t IRQn)
{
    if (IRQn < 0) {
        return((uint32_t)(SCB->SHP[((uint32_t)(IRQn) & 0xF) - 4] >> (8 - 4)));
    } else {
        return((uint32_t)(NVIC->IP[(uint32_t)(IRQn)] >> (8 - 4)));
    }
}

/* System core clock */
extern uint32_t SystemCoreClock;

static inline void SystemCoreClockUpdate(void)
{
    /* Default for MPS2 AN385 - 25MHz */
    SystemCoreClock = 25000000;
}

/* SysTick functions */
static inline uint32_t SysTick_Config(uint32_t ticks)
{
    if ((ticks - 1) > 0xFFFFFF) {
        return (1);      /* Reload value impossible */
    }

    SysTick->LOAD  = ticks - 1;                                  /* set reload register */
    NVIC_SetPriority (SysTick_IRQn, (1 << 4) - 1);               /* set Priority for Systick Interrupt */
    SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
    SysTick->CTRL  = 0x07;                                       /* Enable SysTick IRQ and SysTick Timer */
    return (0);                                                  /* Function successful */
}

#endif /* MINIMAL_CMSIS_H */
