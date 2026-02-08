# Practica5-web_server_Panel - Monitor Solar 12V con Gauge

## Descripción

Sistema de monitoreo para panel solar, batería 12V y salida PWM con interfaz web avanzada usando AJAX y Canvas Gauges. La lógica principal se implementa en JavaScript del lado del cliente para mantener el código Arduino lo más liviano posible.

## Cambios Implementados

### 1. **PanelSolarsimple.ino** - Código Arduino Optimizado

El código se ha refactorizado para ser lo más liviano posible, manteniendo solo lo esencial:

#### Configuración de Pines:
- **A2**: Voltaje Panel Solar (divisor de voltaje 1200kΩ/1500kΩ)
- **A3**: Voltaje Batería 12V (mismo divisor)
- **A4**: Salida PWM (control)
- **A5**: LDR - Sensor de intensidad de luz solar
- **D2**: LED Rojo - Indicador de descarga
- **D3**: LED Amarillo - Indicador de equilibrio
- **D5**: LED Verde - Indicador de generación
- **D6-D9**: LEDs de control (mantenidos como en la práctica anterior)

#### Funciones Principales:

- **leerVoltaje()**: Lee voltaje con promediado de 10 muestras y aplica conversión del divisor
- **leerADC()**: Lectura ADC sin conversión para los gauges
- **actualizarLEDsEstado()**: Determina el estado del sistema basado en voltajes:
  - **Descargando (ROJO)**: Batería < 12.0V y Panel < 12.5V
  - **Equilibrio (AMARILLO)**: Ninguno de los anteriores
  - **Generando (VERDE)**: Panel > 13.0V y Batería < 12.8V

- **XML_response()**: Envía respuesta XML con:
  - Valores ADC para gauges (panel_gauge, bateria_gauge, pwm_gauge)
  - Voltajes calculados (panel_volt, bateria_volt, pwm_volt)
  - Lectura LDR
  - Estados de LEDs (D6-D9 y LEDs de estado)

### 2. **index.htm** - Interfaz Web Avanzada

Interfaz completamente rediseñada con:

#### Canvas Gauges:
- **3 Gauges** (Panel, Batería, PWM) mostrando valor 0-1023 ADC
- Escala visual con colores
- Actualización en tiempo real cada 1 segundo
- Valores en voltios mostrados bajo cada gauge

#### Estado del Sistema:
- **3 LEDs Circulares** que cambian de color según estado:
  - LED Rojo: indica descarga
  - LED Amarillo: indica equilibrio
  - LED Verde: indica generación

#### Sensor LDR:
- Lectura ADC (0-1023)
- Conversión a porcentaje de intensidad de luz (0-100%)

#### Control de LEDs:
- **D6 y D7**: Checkboxes para control manual
- **D8 y D9**: Botones para toggle

#### Diseño:
- Interfaz moderna y responsiva
- Gradiente de fondo (púrpura)
- Grid layout adaptativo
- Sombras y bordes redondeados
- Compatible con dispositivos móviles

## Flujo de Datos

```
Arduino (Lee sensores) → XML Response → JavaScript (Procesa)
                                          ↓
                                    Actualiza Gauges
                                    Actualiza LEDs
                                    Actualiza valores
                                    ↓
                                  HTML (Visualiza)
```

## Actualización de Datos

- **Frecuencia AJAX**: 1 segundo
- **Actualización de LEDs de estado**: 500ms (en Arduino)
- **Promediado de lecturas**: 10 muestras por lectura

## Configuración del Divisor de Voltaje

```
Entrada (0-20V) → R1=30kΩ, R2=10kΩ → Arduino (0-5V)
Factor = (R1 + R2) / R2 = 4.0
Rango: 0-20V
```

## Cálibración

La variable `CALIBRACION = 0.94` se puede ajustar si hay errores sistemáticos comparando con un multímetro.

## Ventajas de esta Implementación

1. **Arduino liviano**: Solo ~340 líneas
2. **Lógica en cliente**: Mayor carga en JavaScript que en Arduino
3. **AJAX eficiente**: Usa XML para comunicación
4. **Escalable**: Fácil agregar más sensores o gauges
5. **Responsivo**: Interfaz se adapta a cualquier tamaño de pantalla
6. **Visual atractivo**: Gauges profesionales y LEDs indicadores
7. **Tiempo real**: Actualización rápida sin recargar página

## Archivos SD Necesarios

- **index.htm**: Interfaz web (en SD card)

## Pruebas Recomendadas

1. Verificar voltajes con multímetro comparado con valores mostrados
2. Probar en diferentes condiciones de luz (LDR)
3. Verificar cambios de estado de LEDs con variación de voltajes
4. Comprobar respuesta de LEDs D6-D9

## Notas Importantes

- Los pines D10-D13 están reservados para Ethernet Shield
- La SD card debe estar formateada en FAT16
- El divisor de voltaje es crítico para lecturas precisas
- El pin D4 está reservado para SD card

## Mejoras Futuras Posibles

- Gráficos de histórico de voltajes
- Almacenamiento de datos en SD
- Control PWM del panel desde web
- Alerta de voltajes críticos
- Dashboard con más estadísticas
