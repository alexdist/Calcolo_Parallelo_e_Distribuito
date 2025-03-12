
# Somma Vettori

## Descrizione
Questo programma in C utilizza **OpenMP** per eseguire la somma di due vettori in parallelo, sfruttando il numero di core specificato dall'utente.

## Funzionamento
1. L'utente sceglie il numero di core da utilizzare.
2. L'utente inserisce la dimensione del vettore `N`.
3. L'utente fornisce gli elementi dei due vettori `a` e `b`.
4. Il programma utilizza OpenMP per eseguire la somma in parallelo.
5. Il risultato viene memorizzato nel vettore `c` e stampato in output.

## Esempio di Input/Output
**Esempio di esecuzione:**
```
Con quanti core vuoi eseguire il calcolo?
2
Inserire N
4
Inserire gli elementi del vettore a
1 2 3 4
Inserire gli elementi del vettore b
5 6 7 8
6.000000 8.000000 10.000000 12.000000
```

## Struttura del Codice
- Allocazione dinamica dei vettori `a`, `b` e `c`.
- Lettura dei dati da input.
- Uso della direttiva `#pragma omp parallel` per la somma parallela.
- Stampa del risultato.


