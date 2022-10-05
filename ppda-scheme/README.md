# Schema PPDA

## Versione in C++
### Compilazione
Per compilare, è necessario assicurarsi di aver installato la libreria `libssl-dev`
```bash
sudo apt-get install libssl-dev
```
Per compilare il progetto, eseguite
```bash
g++ main.cpp -lcrypto -o test
```

### Utilizzo
Per simulare il protocollo
```bash
./test <N>
```
dove `N (int, default 4)` indica il numero di partecipanti da simulare nell'anello

## Versione in Micropython
La versione in micropython è pensata per essere eseguita su un Raspberry Pi Pico,
ma utilizzando solo librerie esistenti anche in Python, è eseguibile su qualsiasi
dispositivo che abbia un interprete per quest'ultimo.

### Utilizzo
Per simulare il protocollo
```bash
python main.py
```