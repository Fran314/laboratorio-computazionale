# Ring sum test

Questi sono i test che ho scritto per provare ad implementare il protocollo proposto
in "Private data aggregation with integrity assurance and fault tolerance for mobile
crowd-sensing" per la somma di dati ottenuti in un contesto di mobile crowd-sensing.

Il codice implementa solo il protocollo di cifartura e emula `N` sensori e l'aggregatore
per verificare la correttezza del protocollo.  
Non è ancora implementata l'aggiunta di rumore ai dati non cifrati.  
Non è ancora implementata la ridondanza tramite RRNS.  
Non è ancora implementato lo scambio di chiavi, che vengono generate artificialmente.
Non è ancora implementato il protocollo di JOIN e LEAVE.

Sono riportate due implementazioni:
- Implementazione in C++, con il suppoprto della libreria openssl (per la funzione
sha256) ed è pensata per essere utilizzata su un dispositivo di media bassa potenza
quale un Raspberry Pi (come indicato nei test)
- Implementazione in Micropython, pensata per essere utilizzata su un microcontrollore
di bassissima potenza quale un Raspberry Pi Pico (come indicato nei test)

## C++ version
### Build
Per buildare il progetto, eseguite
```bash
g++ main.cpp -lcrypto -o test
```

### Utilizzo
Per eseguire il test
```bash
./test <N>
```
dove `N (int, default 4)` indica il numero di partecipanti da simulare nell'anello

## Micropython version
TODO scrivi paragrafo

## Test
La versione in C++ è stata testata sul seguente dispositivo
| Category | Value                                             |
| :------: | ------------------------------------------------- |
|  Device  | Raspberry Pi 4                                    |
|    OS    | Raspberry Pi Os 64bit v11 (bullseye)              |
|   CPU    | Quad core Cortex-A72 (ARM v8) 64-bit SoC @ 1.5GHz |
|   RAM    | 2GB LPDDR4-3200 SDRAM                             |

La versione in micropython è stata testata sul seguente dispositivo
| Category  | Value                              |
| :-------: | ---------------------------------- |
|  Device   | Raspberry Pi Pico                  |
| Processor | Dual-core Arm Cortex-M0+ @ 133 MHz |
|           | 264kB on-chip SRAM                 |
|           | 2MB on-board QSPI flash            |
