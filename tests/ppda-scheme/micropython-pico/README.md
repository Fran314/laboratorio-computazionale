# Test - Schema PPDA in micropython (Raspberry Pi Pico W)

In questa cartella è possibile trovare il codice sorgente dei test relativi allo schema PPDA pensati per il Raspberry Pi Pico W e quindi scritti in micropython.

I test sono stati eseguiti sulla seguente macchina
| Category  | Value                              |
| :-------: | ---------------------------------- |
|  Device   | Raspberry Pi Pico                  |
| Processor | Dual-core Arm Cortex-M0+ @ 133 MHz |
|           | 264kB on-chip SRAM                 |
|           | 2MB on-board QSPI flash            |

## Test
Per eseguire il test basta eseguire sul Raspberry Pi Pico il file `main.py`. L'output, diviso mille, rappresenterà il tempo medio per cifrare un intero a 16 bit in questo protocollo sul Raspberry Pi Pico