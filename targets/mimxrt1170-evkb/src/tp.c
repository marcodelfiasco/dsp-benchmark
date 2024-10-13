// Test points handling

#include "fsl_common.h"
#include "fsl_gpio.h"

/* GPIO probes: PROBE_0 = Green LED, PROBE_1 = Red LED*/
#define PROBE_0_GPIO GPIO9
#define PROBE_0_GPIO_PIN (3U)
#define PROBE_0_OFF_VALUE (0U)
#define PROBE_0_ON_VALUE (1U)

#define PROBE_1_GPIO GPIO9
#define PROBE_1_GPIO_PIN (25U)
#define PROBE_1_OFF_VALUE (0U)
#define PROBE_1_ON_VALUE (1U)

void tp_init(void)
{
    gpio_pin_config_t led_config = { kGPIO_DigitalOutput, 0, kGPIO_NoIntmode };

    /* Configure GPIO probes */
    GPIO_PinInit(PROBE_0_GPIO, PROBE_0_GPIO_PIN, &led_config);
    GPIO_PinWrite(PROBE_0_GPIO, PROBE_0_GPIO_PIN, PROBE_0_OFF_VALUE);

    GPIO_PinInit(PROBE_1_GPIO, PROBE_1_GPIO_PIN, &led_config);
    GPIO_PinWrite(PROBE_1_GPIO, PROBE_1_GPIO_PIN, PROBE_1_OFF_VALUE);
}

void tp0_set(void)
{
    GPIO_PinWrite(PROBE_0_GPIO, PROBE_0_GPIO_PIN, PROBE_0_ON_VALUE);
}

void tp0_clr(void)
{
    GPIO_PinWrite(PROBE_0_GPIO, PROBE_0_GPIO_PIN, PROBE_0_OFF_VALUE);
}

void tp1_set(void)
{
    GPIO_PinWrite(PROBE_1_GPIO, PROBE_1_GPIO_PIN, PROBE_1_ON_VALUE);
}

void tp1_clr(void)
{
    GPIO_PinWrite(PROBE_1_GPIO, PROBE_1_GPIO_PIN, PROBE_1_OFF_VALUE);
}
