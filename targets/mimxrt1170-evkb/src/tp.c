// Test points handling

#include "fsl_common.h"
#include "fsl_gpio.h"

// TP_0 = Green LED
// TP_1 = Red LED
#define TP_0_GPIO_BASE GPIO9
#define TP_0_GPIO_PIN (3U)
#define TP_0_OFF_VALUE (0U)
#define TP_0_ON_VALUE (1U)

#define TP_1_GPIO_BASE GPIO9
#define TP_1_GPIO_PIN (25U)
#define TP_1_OFF_VALUE (0U)
#define TP_1_ON_VALUE (1U)

void tp_init(void)
{
    gpio_pin_config_t led_config = { kGPIO_DigitalOutput, 0, kGPIO_NoIntmode };

    GPIO_PinInit(TP_0_GPIO_BASE, TP_0_GPIO_PIN, &led_config);
    GPIO_PinWrite(TP_0_GPIO_BASE, TP_0_GPIO_PIN, TP_0_OFF_VALUE);

    GPIO_PinInit(TP_1_GPIO_BASE, TP_1_GPIO_PIN, &led_config);
    GPIO_PinWrite(TP_1_GPIO_BASE, TP_1_GPIO_PIN, TP_1_OFF_VALUE);
}

void tp0_set(void)
{
    GPIO_PinWrite(TP_0_GPIO_BASE, TP_0_GPIO_PIN, TP_0_ON_VALUE);
}

void tp0_clr(void)
{
    GPIO_PinWrite(TP_0_GPIO_BASE, TP_0_GPIO_PIN, TP_0_OFF_VALUE);
}

void tp1_set(void)
{
    GPIO_PinWrite(TP_1_GPIO_BASE, TP_1_GPIO_PIN, TP_1_ON_VALUE);
}

void tp1_clr(void)
{
    GPIO_PinWrite(TP_1_GPIO_BASE, TP_1_GPIO_PIN, TP_1_OFF_VALUE);
}
