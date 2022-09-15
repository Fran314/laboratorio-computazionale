# Palisade Test

Questi sono i test che ho scritto per testare la performance della libreria PALISADE sul Raspberry Pi 4.

## Utilizzo
Per compilare il test
```bash
mkdir <nome-test>/build
cd <nome-test>/build
cmake ..
make
```
Se si decide di usare uno dei test `serialized` assicuratevi che esista una cartella `demoData` nella cartella di esecuzione, o otterrete un errore di serializzazione

Per eseguire il test
```bash
./test <FLAG> <SIZE>
```
dove
- `FLAG (int \in [1, 7], default: 7)` indica quali tipi di operazioni omomorfiche calcolare, in particolare:
  - `FLAG & 0x1`: esegui somme
  - `FLAG & 0x2`: esegui moltiplicazioni
  - `FLAG & 0x4`: esegui rotazioni
- `SIZE (int \in [6, 8192], default: 12)` indica la dimensione dei vettori da utilizzare

## Risultati
Il tempo di esecuzione per i test `serialized` risulta molto maggiore nella prima esecuzione (in cui alloca i file per i dati serializzati) che nelle esecuzioni successive, quindi considereremo queste ignorando il delay dato dal file system.

I risultati indicano la media dei tempi di esecuzione delle varie sezioni mediati su 5000 esecuzioni per tipo.

I test sono stati eseguiti sulla seguente macchina
| Category | Value                                             |
| :------: | ------------------------------------------------- |
|  Device  | Raspberry Pi 4                                    |
|    OS    | Raspberry Pi Os 64bit v11 (bullseye)              |
|   CPU    | Quad core Cortex-A72 (ARM v8) 64-bit SoC @ 1.5GHz |
|   RAM    | 2GB LPDDR4-3200 SDRAM                             |

### Serialized-bfv
| Tipo                           | Tempo (ms) |
| ------------------------------ | ---------- |
| Cipher context generation      | 192.298    |
| Cipher context serialization   | 40.483     |
| Cipher context deserialization | 114.857    |
|                                |            |
| Data encryption                | 39.532     |
| Ciphertexts serialization      | 10.670     |
| Ciphertexts deserialization    | 64.189     |
|                                |            |
| Homomorphic operations(*)      | 107.299    |

### Simple-bfv
| Tipo                      | Tempo (ms) |
| ------------------------- | ---------- |
| Cipher context generation | 190.212    |
|                           |            |
| Data encryption           | 38.209     |
|                           |            |
| Homomorphic operations(*) | 100.935    |

*: le operazioni omomorfiche eseguite sono due addizioni, due moltiplicazioni e quattro shift