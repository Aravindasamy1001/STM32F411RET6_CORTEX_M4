#include <stdint.h>

#define RCC_BASE        0x40023800UL
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))

#define GPIOA_BASE      0x40020000UL
#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_BSRR      (*(volatile uint32_t *)(GPIOA_BASE + 0x18))

#define GPIOB_BASE      0x40020400UL
#define GPIOB_MODER     (*(volatile uint32_t *)(GPIOB_BASE + 0x00))
#define GPIOB_BSRR      (*(volatile uint32_t *)(GPIOB_BASE + 0x18))

#define SYSTICK_BASE    0xE000E010UL
#define SYSTICK_CTRL    (*(volatile uint32_t *)(SYSTICK_BASE + 0x00))
#define SYSTICK_LOAD    (*(volatile uint32_t *)(SYSTICK_BASE + 0x04))
#define SYSTICK_VAL     (*(volatile uint32_t *)(SYSTICK_BASE + 0x08))

#define RCC_AHB1ENR_GPIOAEN    (1U << 0)
#define RCC_AHB1ENR_GPIOBEN    (1U << 1)

#define PA5_SET     (1U << 5)
#define PA5_RESET   (1U << (5 + 16))

#define PB1_SET     (1U << 1)
#define PB1_RESET   (1U << (1 + 16))

#define SYSTICK_CTRL_ENABLE     (1U << 0)
#define SYSTICK_CTRL_TICKINT    (1U << 1)
#define SYSTICK_CTRL_CLKSRC     (1U << 2)

static volatile uint32_t g_tick = 0;
static volatile uint8_t g_current_task = 0;

#define NUM_TASKS 2

typedef enum
{
    LED_STATE_ON = 0,
    LED_STATE_OFF = 1
} led_state_t;

typedef struct
{
    uint32_t last_tick;
    uint32_t on_period_ms;
    uint32_t off_period_ms;
    led_state_t state;
} task_cb_t;

static task_cb_t tasks[NUM_TASKS];

static inline void LED1_On(void)
{
    GPIOA_BSRR = PA5_SET;
}

static inline void LED1_Off(void)
{
    GPIOA_BSRR = PA5_RESET;
}

static inline void LED2_On(void)
{
    GPIOB_BSRR = PB1_SET;
}

static inline void LED2_Off(void)
{
    GPIOB_BSRR = PB1_RESET;
}
static void task_LED1(task_cb_t *t)
{
    uint32_t elapsed = g_tick - t->last_tick;

    if(t->state == LED_STATE_ON)
    {
        if(elapsed >= t->on_period_ms)
        {
            LED1_Off();
            t->state = LED_STATE_OFF;
            t->last_tick = g_tick;
        }
    }
    else
    {
        if(elapsed >= t->off_period_ms)
        {
            LED1_On();
            t->state = LED_STATE_ON;
            t->last_tick = g_tick;
        }
    }
}

static void task_LED2(task_cb_t *t)
{
    uint32_t elapsed = g_tick - t->last_tick;

    if(t->state == LED_STATE_ON)
    {
        if(elapsed >= t->on_period_ms)
        {
            LED2_Off();
            t->state = LED_STATE_OFF;
            t->last_tick = g_tick;
        }
    }
    else
    {
        if(elapsed >= t->off_period_ms)
        {
            LED2_On();
            t->state = LED_STATE_ON;
            t->last_tick = g_tick;
        }
    }
}

typedef void (*task_fn_t)(task_cb_t *);

static const task_fn_t task_table[NUM_TASKS] =
{
    task_LED1,
    task_LED2
};

void SysTick_Handler(void)
{
    g_tick++;

    task_table[g_current_task](&tasks[g_current_task]);

    g_current_task++;

    if(g_current_task >= NUM_TASKS)
    {
        g_current_task = 0;
    }
}

static void gpio_init(void)
{
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    (void)RCC_AHB1ENR;

    GPIOA_MODER &= ~(0x3U << 10);
    GPIOA_MODER |=  (0x1U << 10);

    GPIOB_MODER &= ~(0x3U << 2);
    GPIOB_MODER |=  (0x1U << 2);

    LED1_Off();
    LED2_Off();
}

static void systick_init(void)
{
    SYSTICK_VAL = 0;
    SYSTICK_LOAD = 15999;

    SYSTICK_CTRL =
        SYSTICK_CTRL_ENABLE |
        SYSTICK_CTRL_TICKINT |
        SYSTICK_CTRL_CLKSRC;
}

static void tasks_init(void)
{
    tasks[0].last_tick = 0;
    tasks[0].on_period_ms = 200;
    tasks[0].off_period_ms = 200;
    tasks[0].state = LED_STATE_OFF;

    tasks[1].last_tick = 0;
    tasks[1].on_period_ms = 500;
    tasks[1].off_period_ms = 500;
    tasks[1].state = LED_STATE_OFF;
}

int main(void)
{
    gpio_init();
    tasks_init();
    systick_init();

    while(1)
    {
        __asm volatile("wfi");
    }
}
