// Test points handling

#include <processor_include.h>
#include <sysreg.h>
#include <sys/platform.h>
#include <stdio.h>
#include "macro.h"

/* TP0 : FLG2 - PB_03 -> LED7 (yellow)
 * - conflicts with rotary encoder (IOEXP_PIN_nTHUMBWHEEL_OE must be 1)
 * - conflicts with push button 1 (IOEXP_PIN_nPUSHBUTTON1_EN must be 1)
 */

/* TP1: FLG3 - PB_02 */
#define FLG2_PIN        3
#define FLG3_PIN        2

#define MUX_C0_FLG2     1
#define MUX_C0_FLG3     1

void tp_init(void)
{
    /* Enable flags */
    sysreg_bit_set(sysreg_FLAGS, BITM_REGF_FLAGS_FLG2O);
    sysreg_bit_set(sysreg_FLAGS, BITM_REGF_FLAGS_FLG3O);

    /* Port MUX and FER (FLG2 and FLG are both on PORT B) */
    *pREG_PORTB_MUX &= ~(0x3 << (2 * FLG2_PIN));
    *pREG_PORTB_MUX |= MUX_C0_FLG2 << (2 * FLG2_PIN);

    *pREG_PORTB_MUX &= ~(0x3 << (2 * FLG3_PIN));
    *pREG_PORTB_MUX |= MUX_C0_FLG2 << (2 * FLG3_PIN);

    *pREG_PORTB_FER_SET = BIT(FLG2_PIN);
    *pREG_PORTB_FER_SET = BIT(FLG3_PIN);
}

void tp0_set(void)
{
    sysreg_bit_set(sysreg_FLAGS, BITM_REGF_FLAGS_FLG2);
}

void tp0_clr(void)
{
    sysreg_bit_clr(sysreg_FLAGS, BITM_REGF_FLAGS_FLG2);
}

void tp1_set(void)
{
    sysreg_bit_set(sysreg_FLAGS, BITM_REGF_FLAGS_FLG3);
}

void tp1_clr(void)
{
    sysreg_bit_clr(sysreg_FLAGS, BITM_REGF_FLAGS_FLG3);
}
