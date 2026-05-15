
/**
 * @file    helper.h
 * @brief   Funciones utilitarias para conversión y parsing de strings.
 * @version 1.0
 */
#ifndef HELPER_H
#define	HELPER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Convierte un entero sin signo de 32 bits a string ASCII.
 *
 * @param n   Valor a convertir.
 * @param s   Buffer de destino. Debe tener al menos 11 bytes (max uint32 + '\0').
 *
 * @return    Cantidad de dígitos escritos, 0 si s es NULL.
 *
 * @note      No usa malloc ni sprintf. Apto para uso en MCU.
 */
uint8_t uint32_to_str(uint32_t n, uint8_t *s);

/**
 * @brief Convierte un string ASCII a uint32_t.
 *
 * @param s       Puntero al string de entrada. Debe comenzar con un dígito válido.
 * @param valor   Puntero donde se almacena el resultado.
 *
 * @return        true si la conversión fue exitosa, false si s o valor son NULL
 *                o si el string no comienza con un dígito válido.
 */
bool simple_atoi_uint32(const uint8_t *s, uint32_t *valor);

/**
 * @brief Parsea un buffer con tres números separados por comas.
 *
 * @param ptr   Puntero al buffer de entrada. Formato esperado: "n1,n2,n3".
 * @param v1    Puntero donde se almacena el primer valor.
 * @param v2    Puntero donde se almacena el segundo valor.
 * @param v3    Puntero donde se almacena el tercer valor.
 *
 * @return       3  si los tres valores fueron leídos correctamente.
 *              -1  falla lectura v1 o ptr NULL.
 *              -2  no se encuentra la primera coma.
 *              -3  falla lectura v2.
 *              -4  no se encuentra la segunda coma.
 *              -5  falla lectura v3.
 *
 * @note        El buffer debe estar terminado en '\0'.
 */
int8_t parse_buffer(const uint8_t *ptr, uint32_t *v1, uint32_t *v2, uint32_t *v3);

#ifdef __cplusplus
}
#endif

#endif	/* HELPER_H */

