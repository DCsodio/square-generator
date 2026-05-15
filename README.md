# ⚡ Generador de Pulsos Personalizados por UART

Este proyecto implementa un generador de pulsos cuadrados paramétricos para microcontroladores PIC de 8 bits. Permite configurar dinámicamente el tiempo en estado alto, el tiempo en estado bajo y la cantidad total de ciclos a ejecutar mediante comandos enviados por comunicación serial (UART).

Está diseñado bajo una arquitectura de máquina de estados no bloqueante, optimizado para ejecutarse en entornos con recursos limitados (sin usar librerías estándar pesadas como `stdio.h` para el parsing).

## 🚀 Características Principales

* **Comunicación UART Asíncrona:** Implementación robusta con buffers circulares (Ring Buffers) gestionados por interrupciones tanto para RX como para TX, evitando bloqueos en la ejecución.
* **Base de Tiempo Exacta:** Uso de interrupciones de Timer2 para generar una base de tiempo estricta de 1 ms.
* **Parsing Optimizado para MCU:** Lógica de conversión de strings a enteros (y viceversa) escrita desde cero sin depender de `malloc`, `sprintf` o `atoi` de la librería estándar, ahorrando memoria flash y RAM.
* **Protocolo de Trama Sencillo:** Los comandos se envían encapsulados entre caracteres de inicio y fin (`>...<`).
* **Máquina de Estados Finita (FSM):** Ejecución de la señal de salida de forma secuencial y predecible (IDLE -> ESTADO_HIGH -> ESTADO_LOW).

## 🛠️ Tecnologías y Entorno

* **Microcontrolador:** Familia PIC (Configurado para usar oscilador interno a 8MHz).
* **Compilador:** XC8 Compiler.
* **Lenguaje:** C (C99).

## 📡 Protocolo de Comunicación

Para configurar y lanzar una secuencia de pulsos, debes enviar una trama a través del puerto serial con los siguientes parámetros:

* **Baudrate:** 38400 bps
* **Formato de trama:** `>TH,TL,CC<`

Donde:
* `TH`: Tiempo en alto (ms) - (Up)
* `TL`: Tiempo en bajo (ms) - (Down)
* `CC`: Cantidad de ciclos a ejecutar.

**Ejemplo de uso:**
Si envías por la terminal serial el texto:
```text
>1000,500,5<
