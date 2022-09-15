# Laboratorio computazionale

Questa è una repository per salvare il codice dei vari test fatti per il progetto
di laboratorio computazionale.

Per ora contiene due categorie di test:
- [palisade-test](./palisade-test/README.md) nel quale provo ad implementare test
di FHE utilizzando la libreria PALISADE, senza particolari protocolli per la gestione
delle chiavi o l'utilizzo dei dati: semplicemente genero dei dati, utilizzo la libreria
per generare un contesto crittografico con cui cifrare i dati, li cifro, li uso
per fare operazioni omomorfiche e li decifro.

- [ring-sum-test](./ring-sum-test/README.md) nel quale provo ad implementare il
protocollo proposto dall'articolo "Private data aggregation with integrity assurance
and fault tolerance for mobile crowd-sensing" per dispositivi a potenza bassa
(Raspberry Pi 4 2GB) o molto bassa (Raspberry Pi Pico).