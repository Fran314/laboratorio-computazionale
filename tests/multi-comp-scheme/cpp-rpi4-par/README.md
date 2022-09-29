# Test - Schema *multiparty computation* (parallelo) in C++ (Raspberry Pi 4)

In questa cartella è possibile trovare il codice sorgente dei test relativi allo schema in *multiparty computation* con cifratura eseguita parallelamente pensati per il Raspberry Pi 4.

I test sono stati eseguiti sulla seguente macchina
| Category | Value                                             |
| :------: | ------------------------------------------------- |
|  Device  | Raspberry Pi 4                                    |
|    OS    | Raspberry Pi Os 64bit v11 (bullseye)              |
|   CPU    | Quad core Cortex-A72 (ARM v8) 64-bit SoC @ 1.5GHz |
|   RAM    | 2GB LPDDR4-3200 SDRAM                             |

## Compilazione
Per compilare, è necessario assicurarsi di aver installato le librerie `libssl-dev` e `libomp-dev`
```bash
sudo apt-get install libssl-dev libomp-dev
```

Per compilare il codice, eseguite il seguente comando
```bash
g++ main.cpp -lcrypto -fopenmp -o test
```

## Test
Per eseguire i test basta eseguire
```bash
./test <SAMPLE_SIZE> <VALUES_AMOUNT>
```
Ai fini della sperimentazione si è usato il valore `SAMPLE_SIZE=1000` e i valori 1, 256, 1024 e 8192 per `VALUES_AMOUNT`