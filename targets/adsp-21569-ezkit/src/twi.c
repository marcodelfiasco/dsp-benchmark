/* TWI API. */

#include "log.h"
#include "macro.h"
#include "twi.h"
#include <drivers/twi/adi_twi.h>

#define TWI_DEV                     2u      /* TWI device number */
#define TWI_RATE_KILOHERTZ          100u    /* kHz */
#define TWI_DUTYCYCLE               50u     /* percent */
#define TWI_PRESCALER               12u     /* fSCLK/10MHz (112.5 sclk0_0) */
#define TWI_BUFFER_SIZE             8u

/* Memory required for TWI */
static uint8_t _twi_driver_memory[ADI_TWI_MEMORY_SIZE];

/* Dev buffer for configuring ADC-DAC through TWI*/
static uint8_t _twi_buffer[TWI_BUFFER_SIZE];

/* TWI driver handle */
static ADI_TWI_HANDLE _twi_handle;

int twi_init(uint16_t dev_addr)
{
    ADI_TWI_RESULT result;

    result = adi_twi_Open(TWI_DEV, ADI_TWI_MASTER, &_twi_driver_memory[0], ADI_TWI_MEMORY_SIZE, &_twi_handle);
    if (result != ADI_TWI_SUCCESS)
    {
        log_msg("TWI Open failed 0x%08X\n", result);
        return FAILED;
    }

    result = adi_twi_SetPrescale(_twi_handle, TWI_PRESCALER);
    if(result != ADI_TWI_SUCCESS)
    {
        log_msg("TWI Set Prescale failed 0x%08X\n", result);
        return FAILED;
    }

    result = adi_twi_SetBitRate(_twi_handle, TWI_RATE_KILOHERTZ);
    if(result != ADI_TWI_SUCCESS)
    {
        log_msg("TWI Set Bitrate failed 0x%08X\n", result);
        return FAILED;
    }

    result = adi_twi_SetDutyCycle(_twi_handle, TWI_DUTYCYCLE);
    if(result != ADI_TWI_SUCCESS)
    {
        log_msg("TWI Set Duty cycle failed 0x%08X\n", result);
        return FAILED;
    }

    result = adi_twi_SetHardwareAddress(_twi_handle, dev_addr);
    if(result != ADI_TWI_SUCCESS)
    {
        log_msg("TWI Set Hw address failed 0x%08X\n", result);
        return FAILED;
    }

    return SUCCESS;
}

int twi_write_reg8(uint8_t reg, uint8_t data)
{
    ADI_TWI_RESULT result;

    _twi_buffer[0] = reg;
    _twi_buffer[1] = data;
    result = adi_twi_Write(_twi_handle, _twi_buffer, 2u, false);
    if(result != ADI_TWI_SUCCESS)
    {
        log_msg("TWI write failed 0x%08X\n", result);
        return FAILED;
    }

    return SUCCESS;
}

int twi_write_verify_reg8(uint8_t reg, uint8_t data)
{
    int status;
    uint8_t read = ~data;

    status = twi_write_reg8(reg, data);
    CHECK_RESULT_RETURN(status);

    status = twi_read_reg8(reg, &read);
    CHECK_RESULT_RETURN(status);

    if (data != read)
    {
        log_msg("TWI readback failed!\n");
        return FAILED;
    }

    return SUCCESS;
}

int twi_read_reg8(uint8_t reg, uint8_t *p_data)
{
    ADI_TWI_RESULT result;
    uint8_t rx_data;

    /* write register address */
    _twi_buffer[0] = reg;
    result = adi_twi_Write(_twi_handle, _twi_buffer, 1u, true);
    if(result != ADI_TWI_SUCCESS)
    {
        log_msg("TWI write failed 0x%08X\n", result);
        return FAILED;
    }

    /* read register value */
    result = adi_twi_Read(_twi_handle, &rx_data, 1u, false);
    if(result != ADI_TWI_SUCCESS)
    {
        log_msg("TWI Read failed 0x%08X\n", result);
        return FAILED;
    }

    *p_data = rx_data;

    return SUCCESS;
}

int twi_deinit(void)
{
    ADI_TWI_RESULT result;

    result = adi_twi_Close(_twi_handle);
    if(result != ADI_TWI_SUCCESS)
    {
        log_msg("TWI close failed 0x%08X\n", result);
        return FAILED;
    }

    return SUCCESS;
}
