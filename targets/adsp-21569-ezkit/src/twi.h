/* TWI API. */

#ifndef TWI_H_
#define TWI_H_

int twi_init(uint16_t dev_addr);
int twi_write_reg8(uint8_t reg, uint8_t data);
int twi_write_verify_reg8(uint8_t reg, uint8_t data);
int twi_read_reg8(uint8_t reg, uint8_t *p_data);
int twi_deinit(void);

#endif /* TWI_H_ */
