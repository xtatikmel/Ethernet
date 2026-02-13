# Resumen de Prácticas - Proyecto Arduino Ethernet

## Índice

1. [Práctica 1 - Web Client](#práctica-1---web-client)
2. [Práctica 1 - LED Control](#práctica-1---led-control)
3. [Práctica 1 - Switch](#práctica-1---switch)
4. [Práctica 2 - Servidor Web con LEDs y Entradas](#práctica-2---servidor-web-con-leds-y-entradas)
5. [Práctica 2 - AJAX Switch](#práctica-2---ajax-switch)
6. [Práctica 2 - AJAX Switch (Manual)](#práctica-2---ajax-switch-manual)
7. [Práctica 2 - Voltímetro](#práctica-2---voltímetro)
8. [Práctica 2 - Voltímetro Panel Solar](#práctica-2---voltímetro-panel-solar)
9. [Práctica 4 - SD Ajax (Estado de Switch)](#práctica-4---sd-ajax-estado-de-switch)
10. [Práctica 4 - SD Ajax con Botones y XML](#práctica-4---sd-ajax-con-botones-y-xml)
11. [Práctica 4 - SD Ajax XML (Switches + Analógico)](#práctica-4---sd-ajax-xml-switches--analógico)
12. [Práctica 4 - SD Ajax Gauge](#práctica-4---sd-ajax-gauge)
13. [Práctica 4 - SD Imagen](#práctica-4---sd-imagen)
14. [Práctica 4 - SD Link (Páginas enlazadas)](#práctica-4---sd-link-páginas-enlazadas)
15. [Práctica 5 - Mega 24 Salidas](#práctica-5---mega-24-salidas)
16. [Práctica 5 - Web Server IO](#práctica-5---web-server-io)
17. [Práctica 5 - Web Server Panel Solar](#práctica-5---web-server-panel-solar)
18. [Práctica 5 - Panel Solar Simple 2](#práctica-5---panel-solar-simple-2)
19. [Práctica 6 - ThingSpeak](#práctica-6---thingspeak)
20. [Sensorica](#sensorica)

---

## Práctica 1 - Web Client

**Directorio:** [`Practica1/`](Practica1/)  
**Archivo:** [`Practica1.ino`](Practica1/Practica1.ino)

### Objetivo
Conectar Arduino como **cliente web** a un servidor externo (Google) mediante HTTP GET, demostrando la capacidad de realizar peticiones HTTP salientes.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield W5100 |
| Librerías | `SPI.h`, `Ethernet.h` |
| Protocolo | HTTP GET (puerto 80) |
| Red | DHCP automático |

### Consideraciones para pruebas
- Configurar la dirección MAC correcta del shield (`0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED`)
- Requiere conexión a Internet activa para alcanzar Google
- El shield Ethernet ocupa los pines **10, 11, 12, 13** (SPI)
- Verificar que DHCP asigne IP correctamente mediante el Monitor Serial (9600 baud)
- Si DHCP falla, el programa entra en bucle infinito

---

## Práctica 1 - LED Control

**Directorio:** [`Practica1-_Led/`](Practica1-_Led/)  
**Archivo:** [`Preactica_1_-_Led.ino`](Practica1-_Led/Preactica_1_-_Led.ino)

### Objetivo
Crear un **servidor web** en Arduino que sirve una página HTML con un checkbox para encender/apagar un LED conectado al pin 2.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + LED en pin 2 + resistencia |
| Librerías | `SPI.h`, `Ethernet.h` |
| Protocolo | HTTP Server (puerto 80) |
| Red | IP estática `192.168.1.177` |
| Interfaz | HTML con formulario GET y checkbox |

### Consideraciones para pruebas
- Conectar LED con resistencia en serie entre **pin 2** y **GND**
- IP fija: `192.168.1.177` — ajustar según tu red local
- La página se recarga completamente al hacer clic en el checkbox (`onclick="submit();"`)
- Monitorear peticiones HTTP por Serial a 9600 baud
- El HTML se genera directamente desde el código Arduino (sin SD)

---

## Práctica 1 - Switch

**Directorio:** [`Practica1-Swich/`](Practica1-Swich/)  
**Archivo:** [`Practica1-Swich.ino.ino`](Practica1-Swich/Practica1-Swich.ino.ino)

### Objetivo
Servidor web que muestra el **estado de un switch** (ON/OFF) conectado al pin 3. La página se auto-refresca cada segundo.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + Switch en pin 3 |
| Librerías | `SPI.h`, `Ethernet.h` |
| Protocolo | HTTP Server (puerto 80) |
| Red | IP estática `10.0.0.20` |
| Interfaz | HTML con meta-refresh cada 1 segundo |

### Consideraciones para pruebas
- Switch conectado al **pin 3** como INPUT
- IP: `10.0.0.20` — cambiar según la red
- La página usa `<meta http-equiv="refresh" content="1">` para auto-actualización (recarga completa)
- No usa tarjeta SD

---

## Práctica 2 - Servidor Web con LEDs y Entradas

**Directorio:** [`Practica2/`](Practica2/)  
**Archivo:** [`Practica2.ino`](Practica2/Practica2.ino)

### Objetivo
Servidor web completo que permite **controlar 2 LEDs** (ON/OFF), leer **2 entradas analógicas** (A0, A1) y **2 entradas digitales** (D4, D5) con botones HTML.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + 2 LEDs (pin 2, 3) + 2 botones (pin 4, 5 con pullup) |
| Librerías | `SPI.h`, `Ethernet.h` |
| Protocolo | HTTP Server con parámetros GET (`?Data=1` a `?Data=4`) |
| Red | IP estática `192.168.1.177` |
| Interfaz | HTML con botones ON/OFF y lectura de sensores |

### Consideraciones para pruebas
- LEDs en pines **2** y **3**, botones en pines **4** y **5** (INPUT_PULLUP)
- Los LEDs inician en HIGH (encendidos)
- Los comandos se envían por URL: `?Data=1` (LED1 ON), `?Data=2` (LED1 OFF), `?Data=3` (LED2 ON), `?Data=4` (LED2 OFF)
- La cadena HTTP se limita a 50 caracteres para parseo
- Tutorial de referencia: [Naylamp Mechatronics](https://naylampmechatronics.com/blog/27_tutorial-ethernet-shield-y-arduino.html)

---

## Práctica 2 - AJAX Switch (Automático)

**Directorio:** [`Practica2-AJAX_switch/`](Practica2-AJAX_switch/)  
**Archivo:** [`Practica2-AJAX_switch.ino`](Practica2-AJAX_switch/Practica2-AJAX_switch.ino)

### Objetivo
Mostrar el estado de un switch usando **AJAX** con actualización automática cada segundo, sin recargar la página completa.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + Switch en pin 3 |
| Librerías | `SPI.h`, `Ethernet.h` |
| Protocolo | HTTP Server + AJAX (XMLHttpRequest) |
| Red | IP estática `10.0.0.20` |
| Interfaz | HTML + JavaScript con `setTimeout` cada 1000ms |

### Consideraciones para pruebas
- Switch en **pin 3** como INPUT
- La conexión usa `Connection: keep-alive`
- AJAX solicita `ajax_switch` con parámetro `nocache` para evitar caché del navegador
- La actualización es automática con `setTimeout('GetSwitchState()', 1000)`
- No requiere SD card

---

## Práctica 2 - AJAX Switch (Manual)

**Directorio:** [`Practica2-Ajax.ino/`](Practica2-Ajax.ino/)  
**Archivo:** [`Practica2-Ajax.ino.ino`](Practica2-Ajax.ino/Practica2-Ajax.ino.ino)

### Objetivo
Similar a la anterior pero el estado del switch se lee **manualmente** al hacer clic en un botón HTML, demostrando AJAX bajo demanda.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + Switch en pin 3 |
| Librerías | `SPI.h`, `Ethernet.h` |
| Protocolo | HTTP Server + AJAX manual |
| Red | IP estática `10.0.0.20` |
| Interfaz | HTML + botón "Get Switch State" con JavaScript |

### Consideraciones para pruebas
- Idéntico hardware al AJAX automático
- La diferencia es que el usuario debe presionar el botón para consultar el estado
- Útil para comparar AJAX manual vs automático

---

## Práctica 2 - Voltímetro

**Directorio:** [`Practica2-Voltimetro/`](Practica2-Voltimetro/)  
**Archivo:** [`Practica2-Voltimetro.ino`](Practica2-Voltimetro/Practica2-Voltimetro.ino)

### Objetivo
Servidor web con interfaz estilizada que muestra **voltajes** en A0 y A1, estado de **entradas digitales** (D4, D5) y control de **2 LEDs** con botones ON/OFF.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + 2 LEDs (pin 2, 3) + 2 botones (pin 4, 5) |
| Librerías | `SPI.h`, `Ethernet.h` |
| Protocolo | HTTP Server con parámetros GET |
| Red | IP estática `192.168.1.177` |
| Interfaz | HTML con CSS inline (botones estilizados verde/rojo) |

### Consideraciones para pruebas
- A0 se lee con referencia de **5.0V**, A1 con referencia de **3.3V**
- Timeout de lectura HTTP de **2 segundos**
- Detecta fin de petición HTTP por doble salto de línea (`\r\n\r\n`)
- Delay de **100ms** antes de cerrar conexión (más robusto que 1ms)
- Interfaz responsive con `meta viewport`

---

## Práctica 2 - Voltímetro Panel Solar

**Directorio:** [`Practica2-VoltimetroPanelSolar/`](Practica2-VoltimetroPanelSolar/)  
**Archivo:** [`Practica2-VoltimetroPanelSolar.ino`](Practica2-VoltimetroPanelSolar/Practica2-VoltimetroPanelSolar.ino)

### Objetivo
Medir voltaje de **batería 12V** y **panel solar 12V** usando divisor de voltaje, con promediado de muestras para reducir ruido.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + Divisor de voltaje (R1=1200Ω, R2=1500Ω) + 2 LEDs |
| Librerías | `SPI.h`, `Ethernet.h` |
| Sensores | A0: Batería 12V, A1: Panel Solar 12V |
| Red | IP estática `192.168.1.177` |
| Técnica | Promediado de 10 muestras, factor de calibración |

### Consideraciones para pruebas
- **Divisor de voltaje** con factor = (R1 + R2) / R2 = 1.8
- Variable `CALIBRACION = 0.96` para ajuste fino con multímetro
- Se promedian **10 muestras** por lectura para reducir ruido
- Referencia analógica configurada a DEFAULT (5V)
- Verificar valores con multímetro y ajustar calibración

---

## Práctica 4 - SD Ajax (Estado de Switch)

**Directorios:** [`Practica4-eth_websrv_SD_Ajax/`](Practica4-eth_websrv_SD_Ajax/) y [`Practica4-SDAjax/`](Practica4-SDAjax/)  
**Archivos:** [`Practica4-eth_websrv_SD_Ajax.ino`](Practica4-eth_websrv_SD_Ajax/Practica4-eth_websrv_SD_Ajax.ino), [`SDAjax.ino`](Practica4-SDAjax/SDAjax.ino)

### Objetivo
Servidor web que sirve la página desde **tarjeta SD** y usa AJAX para mostrar el estado de un switch en pin 3, actualizándose cada segundo.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + SD card (FAT16, 2GB) + Switch en pin 3 |
| Librerías | `SPI.h`, `Ethernet.h`, `SD.h` |
| Protocolo | HTTP Server + AJAX |
| Almacenamiento | Página `index.htm` en SD card |

### Consideraciones para pruebas
- SD card formateada en **FAT16** (2GB)
- El archivo `index.htm` debe estar en la raíz de la SD
- Pin **4** reservado para SD card (chip select)
- Pines **10-13** reservados para Ethernet Shield (SPI)
- Se eliminó el uso de la clase `String` para ahorrar SRAM

---

## Práctica 4 - SD Ajax con Botones y XML

**Directorio:** [`Practica4-button/`](Practica4-button/)  
**Archivos:** [`button.ino`](Practica4-button/button.ino), [`button.html`](Practica4-button/button.html)

### Objetivo
Servidor web con SD que muestra el estado de **2 switches** (pin 7, 8) y **1 entrada analógica** (A2) usando AJAX y XML.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + SD card + 2 Switches (pin 7, 8) + Potenciómetro (A2) |
| Librerías | `SPI.h`, `Ethernet.h`, `SD.h` |
| Protocolo | HTTP Server + AJAX + XML |
| Almacenamiento | Página en SD card |

### Consideraciones para pruebas
- Switches en pines **7** y **8**
- Potenciómetro en **A2**
- Respuesta en formato XML para comunicación AJAX
- SD card FAT16

---

## Práctica 4 - SD Ajax XML (Switches + Analógico)

**Directorio:** [`Practica4-eth_websrv_SD_Ajax_XML/`](Practica4-eth_websrv_SD_Ajax_XML/)  
**Archivos:** [`Practica4-eth_websrv_SD_Ajax_XML.ino`](Practica4-eth_websrv_SD_Ajax_XML/Practica4-eth_websrv_SD_Ajax_XML.ino), [`index.htm`](Practica4-eth_websrv_SD_Ajax_XML/index.htm)

### Objetivo
Mostrar estado de **2 switches** y **1 entrada analógica** (A0) con AJAX y respuestas XML desde SD card.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + SD card + 2 Switches (pin 7, 8) + Potenciómetro (A0) |
| Librerías | `SPI.h`, `Ethernet.h`, `SD.h` |
| Protocolo | HTTP + AJAX + XML |

### Consideraciones para pruebas
- Similar a Práctica 4-button pero con potenciómetro en **A0**
- Se eliminó el uso de la clase `String` para optimizar memoria

---

## Práctica 4 - SD Ajax Gauge

**Directorio:** [`Practica4-eth_websrv_SD_Ajax_gauge/`](Practica4-eth_websrv_SD_Ajax_gauge/)  
**Archivos:** [`Practica4-eth_websrv_SD_Ajax_gauge.ino`](Practica4-eth_websrv_SD_Ajax_gauge/Practica4-eth_websrv_SD_Ajax_gauge.ino), [`index.htm`](Practica4-eth_websrv_SD_Ajax_gauge/Arduino_web_gauge/index.htm)

### Objetivo
Mostrar el valor de una **entrada analógica** (A0) en un **gauge visual** (dial) en la página web, usando Canvas Gauge de JavaScript.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + SD card + Potenciómetro (A0) |
| Librerías | `SPI.h`, `Ethernet.h`, `SD.h` |
| Protocolo | HTTP + AJAX |
| Visualización | Canvas Gauge ([canv-gauge](https://github.com/Mikhus/canv-gauge)) |

### Consideraciones para pruebas
- Potenciómetro en **A0**
- Los archivos del gauge deben estar en la SD card
- Verificar licencia del gauge (`gauge_license.txt`)
- SD card FAT16

---

## Práctica 4 - SD Imagen

**Directorio:** [`Practica4-eth_websrv_SD_image/`](Practica4-eth_websrv_SD_image/)  
**Archivo:** [`Practica4-eth_websrv_SD_image.ino`](Practica4-eth_websrv_SD_image/Practica4-eth_websrv_SD_image.ino)

### Objetivo
Servidor web que sirve páginas HTML con **imágenes JPG** desde la tarjeta SD.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + SD card (FAT16) |
| Librerías | `SPI.h`, `Ethernet.h`, `SD.h` |
| Contenido | `index.htm`, `page2.htm`, `pic.jpg` en SD |

### Consideraciones para pruebas
- Requiere 3 archivos en la SD: `index.htm`, `page2.htm`, `pic.jpg`
- Buffer de petición HTTP limitado a **20 bytes** (`REQ_BUF_SZ`)
- Verificar que la imagen JPG no sea demasiado grande para la transferencia

---

## Práctica 4 - SD Link (Páginas enlazadas)

**Directorio:** [`Practica4-eth_websrv_SD_link/`](Practica4-eth_websrv_SD_link/)  
**Archivo:** [`Practica4-eth_websrv_SD_link.ino`](Practica4-eth_websrv_SD_link/Practica4-eth_websrv_SD_link.ino)

### Objetivo
Servidor web con **dos páginas HTML enlazadas** entre sí, servidas desde SD card. Demuestra navegación entre páginas.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + SD card (FAT16) |
| Librerías | `SPI.h`, `Ethernet.h`, `SD.h` |
| Contenido | `index.htm` y `page2.htm` en SD |

### Consideraciones para pruebas
- Requiere `index.htm` y `page2.htm` en la SD
- Se eliminó el uso de la clase `String` para ahorrar SRAM
- Se agregaron funciones `StrClear()` y `StrContains()` como alternativa
- Se deshabilita el chip Ethernet al inicio para inicializar SD correctamente

---

## Práctica 5 - Mega 24 Salidas

**Directorio:** [`Practica5-Mega/`](Practica5-Mega/)  
**Archivo:** [`eth_websrv_SD_Ajax_24_out.ino`](Practica5-Mega/eth_websrv_SD_Ajax_24_out/eth_websrv_SD_Ajax_24_out.ino)

### Objetivo
Controlar **24 salidas digitales** mediante checkboxes en una página web, usando Arduino MEGA 2560.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | **Arduino MEGA 2560** + Ethernet Shield + SD card + 24 LEDs (pines 26-49) |
| Librerías | `SPI.h`, `Ethernet.h`, `SD.h` |
| Protocolo | HTTP + AJAX |
| Buffer | `REQ_BUF_SZ = 60` |

### Consideraciones para pruebas
- **Solo funciona con Arduino MEGA 2560** (no UNO)
- LEDs conectados de **pin 26 a pin 49**
- Pines **50-53** NO se pueden usar (SPI del Ethernet Shield)
- Pin **4** reservado para SD card
- SD card FAT16 con `index.htm`

---

## Práctica 5 - Web Server IO

**Directorio:** [`Practica5-web_server_IO/`](Practica5-web_server_IO/)  
**Archivo:** [`eth_websrv_SD_Ajax_in_out.ino`](Practica5-web_server_IO/eth_websrv_SD_Ajax_in_out/eth_websrv_SD_Ajax_in_out.ino)

### Objetivo
Servidor web completo con **4 entradas analógicas**, **3 switches** y **4 salidas** (2 checkboxes + 2 botones), todo con AJAX desde SD.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + SD card |
| Entradas analógicas | A2, A3, A4 |
| Switches | Pines 2, 3, 5 |
| Salidas (LEDs) | Pines 6, 7, 8, 9 |
| Librerías | `SPI.h`, `Ethernet.h`, `SD.h` |
| Interfaz | Checkboxes + Botones con AJAX |

### Consideraciones para pruebas
- Entradas analógicas en **A2 a A4**
- Switches en pines **2, 3, 5**
- LEDs de salida en pines **6 a 9**
- Buffer HTTP de **60 bytes**
- Página `index.htm` en SD card
- Incluye imágenes de referencia en carpeta `Images/`

---

## Práctica 5 - Web Server Panel Solar

**Directorio:** [`Practica5-web_server_Panel/`](Practica5-web_server_Panel/)  
**Archivo:** [`PanelSolarsimple.ino`](Practica5-web_server_Panel/PanelSolarsimple/PanelSolarsimple.ino)

### Objetivo
Sistema de **monitoreo de panel solar**, batería 12V y salida PWM con interfaz web avanzada usando AJAX, Canvas Gauges y LEDs indicadores de estado.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + SD card + Divisor de voltaje (30kΩ/10kΩ) + LDR + LEDs |
| Sensores | A2: Panel Solar, A3: Batería 12V, A4: PWM, A5: LDR |
| LEDs estado | D2: Rojo (descarga), D3: Amarillo (equilibrio), D5: Verde (generación) |
| LEDs control | D6-D9 |
| Librerías | `SPI.h`, `Ethernet.h`, `SD.h` |
| Visualización | Canvas Gauges, AJAX cada 1 segundo, XML |

### Consideraciones para pruebas
- **Divisor de voltaje**: R1=30kΩ, R2=10kΩ → Factor = 4.0 → Rango 0-20V
- **Calibración**: Variable `CALIBRACION = 0.94` (ajustar con multímetro)
- Promediado de **10 muestras** por lectura
- Estados del sistema:
  - 🔴 **Descargando**: Batería < 12.0V y Panel < 12.5V
  - 🟡 **Equilibrio**: Estado intermedio
  - 🟢 **Generando**: Panel > 13.0V y Batería < 12.8V
- Pines D10-D13 reservados para Ethernet Shield
- Pin D4 reservado para SD card
- SD card FAT16 con `index.htm`
- Ver documentación detallada en [`README_CAMBIOS.md`](../Practica5-web_server_Panel/README_CAMBIOS.md)

---

## Práctica 5 - Panel Solar Simple 2

**Directorio:** [`Practica5-web_server_Panel/PanelSolarsimple2/`](Practica5-web_server_Panel/PanelSolarsimple2/)  
**Archivo:** [`PanelSolarsimple2.ino`](Practica5-web_server_Panel/PanelSolarsimple2/PanelSolarsimple2.ino)

### Objetivo
Versión simplificada del monitor solar **sin SD card**, con HTML generado directamente desde Arduino. Control de 4 LEDs y lectura de voltajes con divisor.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + Divisor de voltaje (30kΩ/10kΩ) + 4 LEDs (D6-D9) |
| Librerías | `SPI.h`, `Ethernet.h` |
| Red | IP estática `192.168.1.177` |
| Divisor | Factor = (30k + 10k) / 10k = 4.0 |

### Consideraciones para pruebas
- **No requiere SD card** — HTML embebido en el código
- LEDs en pines **6, 7, 8, 9**
- Función `leerVoltaje()` con conversión de divisor de voltaje
- Más ligero que la versión con SD pero sin gauges visuales

---

## Práctica 6 - ThingSpeak

**Directorio:** [`Practica6-ThingSpeak/`](Practica6-ThingSpeak/)  
**Archivos:** [`Practica6-ThingSpeak.ino`](Practica6-ThingSpeak/Practica6-ThingSpeak.ino), [`index.htm`](Practica6-ThingSpeak/index.htm)

### Objetivo
Servidor web con gauge que además **envía datos a ThingSpeak** (plataforma IoT en la nube) cada 20 segundos, permitiendo monitoreo remoto de voltaje.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + SD card + Potenciómetro (A2) |
| Librerías | `SPI.h`, `Ethernet.h`, `SD.h` |
| Nube | ThingSpeak (API REST) |
| Visualización | Canvas Gauge local + ThingSpeak remoto |
| Frecuencia | Envío cada 20 segundos (100 × 200ms) |

### Consideraciones para pruebas
- Requiere **cuenta en ThingSpeak** y una **Write API Key** válida
- Reemplazar `I9DQOZ8589KEHA9J` con tu API Key real en `index.htm`
- Requiere conexión a Internet para enviar datos a ThingSpeak
- El gauge local muestra valor ADC (0-1023) convertido a voltaje (0-5V)
- SD card FAT16 con `index.htm`

---

## Sensorica

**Directorio:** [`Sensorica/`](Sensorica/)  
**Archivos:** [`Sensorica.ino`](Sensorica/Sensorica.ino), [`index.html`](Sensorica/index.html)

### Objetivo
Servidor web básico que sirve una página almacenada en SD card. Base para proyectos de sensorización.

### Tecnología
| Componente | Detalle |
|---|---|
| Hardware | Arduino Uno + Ethernet Shield + SD card (FAT16, 8GB) |
| Librerías | `SPI.h`, `Ethernet.h`, `SD.h` |
| Red | IP estática `192.168.1.177` |
| Autor | Jeferson Guevara (Sep 2024) |

### Consideraciones para pruebas
- SD card de **8GB** formateada en FAT16
- Página `index.html` en la raíz de la SD
- Proyecto base — verificar que sirve la página correctamente antes de agregar sensores

---

## Consideraciones Generales para Todas las Prácticas

### Hardware Común
- **Ethernet Shield W5100**: Ocupa pines SPI (10, 11, 12, 13)
- **SD Card**: Usa pin 4 como chip select
- **Conflicto de pines**: No usar pines 4, 10-13 para otros propósitos

### Red
- Configurar la **dirección MAC** única para cada shield
- Ajustar la **IP estática** según tu red local (o usar DHCP en Práctica 1)
- Puerto HTTP por defecto: **80**

### Software
- Arduino IDE 1.0.3+ (compatible con versiones posteriores)
- Librerías estándar: `SPI.h`, `Ethernet.h`, `SD.h`

### SD Card
- Formato: **FAT16** (recomendado para tarjetas de 2GB)
- Archivos en la **raíz** de la tarjeta
- Nombres de archivo en formato **8.3** (máximo 8 caracteres + extensión de 3)

### Depuración
- Usar **Monitor Serial** a 9600 baud para diagnóstico
- Verificar conexión Ethernet con `Ethernet.localIP()`
- Comprobar que la SD se inicializa correctamente antes de servir páginas

### Progresión de Aprendizaje
```
Práctica 1: Cliente web básico → Servidor web básico (LED/Switch)
     ↓
Práctica 2: Control de múltiples I/O → Introducción a AJAX → Voltímetro
     ↓
Práctica 4: Páginas desde SD card → AJAX + XML → Gauges visuales
     ↓
Práctica 5: Múltiples I/O con SD → Panel Solar completo
     ↓
Práctica 6: Integración con IoT (ThingSpeak)
```
