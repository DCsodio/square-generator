# ⚡ Generador de Pulsos Personalizados (PIC UART)

Este proyecto implementa un sistema embebido para la generación de trenes de pulsos cuadrados con parámetros configurables dinámicamente. La configuración se realiza a través de una interfaz serial (UART), permitiendo definir el tiempo en alto, el tiempo en bajo y la cantidad de ciclos de la señal.

El sistema está diseñado para microcontroladores PIC y utiliza una arquitectura no bloqueante basada en interrupciones y máquinas de estados.

## ✨ Características

* **Control Preciso**: Base de tiempo de 1ms generada mediante el Timer2.
* **Comunicación Robusta**: Implementación de UART con buffers circulares (Ring Buffers) para recepción y transmisión.
* **Parsing Eficiente**: Procesamiento de tramas de datos optimizado para sistemas embebidos, sin uso de librerías pesadas como `stdio`.
* **Arquitectura FSM**: Máquina de estados finitos para la gestión del ciclo de vida de los pulsos (IDLE, HIGH, LOW).
* **Indicadores Visuales**: Salida de pulso en `RB7` y estado de ejecución en `RB1`.

## 🛠️ Requisitos de Hardware

* **Microcontrolador**: PIC compatible con compilador XC8 (ej. PIC16F88x o similar).
* **Frecuencia de Operación**: 8 MHz (Oscilador interno).
* **Terminal Serial**: Configurada a **38400 baudios**.

## 📡 Protocolo de Comunicación

El dispositivo escucha tramas de datos con el siguiente formato:

`>TH,TL,CC<`

Donde:
* **TH**: Tiempo en estado Alto (milisegundos).
* **TL**: Tiempo en estado Bajo (milisegundos).
* **CC**: Cantidad de Ciclos a repetir.

**Ejemplo:** Para generar 5 pulsos de 1 segundo encendido y 0.5 segundos apagado, enviar:
`>1000,500,5<`

## 📂 Estructura del Proyecto

| Archivo | Descripción |
| :--- | :--- |
| `main.c` | Lógica principal y Máquina de Estados. |
| `uart.c/h` | Driver de comunicación serial con buffers de interrupción. |
| `timer.c/h` | Configuración del Timer2 para la base de tiempo de 1ms. |
| `helpers.c/h` | Utilidades de conversión (atoi/itoa) y parseo de tramas. |
| `interrupts.c` | Manejo centralizado de las Rutinas de Servicio de Interrupción (ISR). |
| `config.h` | Configuración de fusibles y frecuencia del sistema. |

## 🚀 Instalación y Uso

1.  Clonar este repositorio.
2.  Abrir el proyecto en **MPLAB X IDE**.
3.  Asegurarse de tener instalado el compilador **XC8**.
4.  Compilar y programar el microcontrolador.
5.  Conectar una terminal serial (como Tera Term o el Monitor Serial de Arduino) a 38400 bps.
6.  Enviar la trama de configuración deseada entre los caracteres `>` y `<`.

## ⚙️ Detalles de Implementación

* **No bloqueante**: El código principal no utiliza `delay` durante la ejecución de los pulsos, lo que permite que el sistema siga respondiendo a interrupciones.
* **Seguridad**: Se implementan secciones críticas para el acceso a variables compartidas con las interrupciones (como `contador_ms`).
* **Personalización**: Los pines y la velocidad de comunicación son fácilmente modificables en los archivos de cabecera correspondientes.
