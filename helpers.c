#include "helpers.h"


uint8_t uint32_to_str(uint32_t n, uint8_t *s) { 
    
    if (s == NULL) return 0;

    uint8_t *p = s; 
    uint8_t len=0;
    // Extraer digitos (se guardan en orden inverso) 

    do { 
        *p++ = (uint8_t)(n % 10) + '0'; //coloca el valor y despues incrementa la direccion
        len++;
    } while ((n /= 10) > 0);   
    
    *p = '\0'; // Terminador de cadena
    
    // Invertir los digitos para que queden en el orden correcto
    uint8_t *fin_digitos = p - 1; 
    while (s < fin_digitos) { 
        uint8_t temp = *s; 
        *s++ = *fin_digitos; 
        *fin_digitos-- = temp; 
    }
    return len;
}


bool simple_atoi_uint32(const uint8_t *s, uint32_t *valor) {
    if (s==NULL || valor==NULL) return false;
    if (*s < '0' || *s > '9') return false;

    uint32_t res = 0;
    while (*s >= '0' && *s <= '9') { 
        res = res * 10 + (*s - '0'); 
        s++; 
    }
    *valor=res;
    
    return true;  
}


int8_t parse_buffer(const uint8_t *ptr, uint32_t *v1, uint32_t *v2, uint32_t *v3) { 
    if (!ptr) return -1;

    if (!simple_atoi_uint32(ptr, v1)) return -1;
    while (*ptr != ',' && *ptr != '\0') ptr++;  
    if (*ptr != ',') return -2;
    ptr++; 
 
    if (!simple_atoi_uint32(ptr, v2)) return -3; 
    while (*ptr != ',' && *ptr != '\0') ptr++;  
    if (*ptr != ',') return -4;
    ptr++;  
 
    if (!simple_atoi_uint32(ptr, v3)) return -5;
 
    return 3;
}


/*
int8_t parse_buffer_n(const uint8_t *ptr, uint32_t *valores, uint8_t n) { 
    if (!ptr || !valores || n == 0) return -1;

    for (uint8_t i = 0; i < n; i++) {
        if (!simple_atoi_uint32(ptr, &valores[i])) return -2;
        
        if (i < n - 1) {  // entre valores esperás coma, después del último no
            while (*ptr != ',' && *ptr != '\0') ptr++;
            if (*ptr != ',') return -3;
            ptr++;
        }
    }

    return 0;
}
*/
