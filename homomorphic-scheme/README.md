# Schema omomorfico

Questi sono i test che ho scritto per testare la performance della libreria PALISADE sul Raspberry Pi 4.

## Compilazione
Per compilare è necessario avere installato la libreria PALISADE

Per compilare il test
```bash
mkdir <nome-schema>/build
cd <nome-schema>/build
cmake ..
make
```

## Utilizzo
Se si decide simulare lo schema `serialized` assicuratevi che esista una cartella `demoData` nella cartella di esecuzione, o otterrete un errore di serializzazione

Per eseguire il test
```bash
./test
```