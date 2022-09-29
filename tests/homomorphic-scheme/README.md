# Test - Schema omomorfico

In questa cartella è possibile trovare il codice sorgente dei test relativi allo schema omomorfico.

I test sono stati eseguiti sulla seguente macchina
| Category | Value                                             |
| :------: | ------------------------------------------------- |
|  Device  | Raspberry Pi 4                                    |
|    OS    | Raspberry Pi Os 64bit v11 (bullseye)              |
|   CPU    | Quad core Cortex-A72 (ARM v8) 64-bit SoC @ 1.5GHz |
|   RAM    | 2GB LPDDR4-3200 SDRAM                             |

## Compilazione
Per compilare il codice, seguire la seguente procedura
```bash
mkdir build
cd build
cmake ../
make
```

## Test
Per eseguire i test basta eseguire
```bash
./test <SECURITY_LEVEL> <SAMPLE_SIZE> <VALUES_AMOUNT>
```
Ai fini della sperimentazione si è usato i valori 1 (256), 2 (192) e 3 (128) per `SECURITY_LEVEL`, il valore `SAMPLE_SIZE=1000` e i valori 1, 256, 1024 e 8192 per `VALUES_AMOUNT`

Per le stime sulla memoria utilizata è stato utilizzato `top`