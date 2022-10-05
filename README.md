# Laboratorio computazionale

Questa repository contiene tutto il codice utilizzato per il progetto di Laboratorio Computazionale. Il progetto si propone di paragonare due metodi di crittografia post-quantistica diversi (uno basato sulla crittografia omomorfica, l'altro basato sulla *privacy-preserving data aggregation*) per la somma di dati cifrati.

La repository è divisa in 3 sottocartelle
- [homomorphic-scheme](./homomorphic-scheme/) dove è riportato il codice del protocollo omomorfico, per poter verificare la correttezza del protocollo
- [ppda-scheme](./ppda-scheme/) dove è riportato il codice del protocollo PPDA, per poter verificare la correttezza del protocollo
- [tests](./tests/) dove è riportato il codice sorgente di tutti i test della sperimentazione, per poterli eventualmente ripetere (per ottenere gli stessi risultati andranno chiaramente ripetuti sulle stesse macchine sulle quali sono stati testati ai fini della relazione)