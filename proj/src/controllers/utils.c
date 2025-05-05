#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief Extracts the LSB from a 16-bit value
 * @param val 16-bit value from which to extract the LSB
 * @param lsb Pointer to store the resulting LSB
 * @return 0 on success, non-zero otherwise
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  
  // Check for null pointer
  if (lsb == NULL) { return 1; }

  // Get LSB
  *lsb = val & 0xFF;
  return 0;
}

/**
 * @brief Extracts the MSB from a 16-bit value.
 * @param val 16-bit value from which to extract the MSB
 * @param msb Pointer to store the resulting MSB
 * @return 0 on success, non-zero otherwise
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  
  // Check for null pointer
  if (msb == NULL) { return 1; }

  // Get MSB
  *msb = val >> 8;
  return 0;
}
/**
 * @brief Reads a byte of data from the I/O port
 * @param port Port to read the data from
 * @param value Pointer to store the read byte
 * @return 0 on success, non-zero otherwise
 */
int (util_sys_inb)(int port, uint8_t *value) {
  
  // Check for null pointer
  if (value==NULL){return 1;}

  // Read data 
  uint32_t val;
  int res = sys_inb(port, &val);

  // Get lower 8 bits
  *value = val & 0xFF;
  return res;
}
