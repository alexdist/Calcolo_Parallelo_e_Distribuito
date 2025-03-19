# Somma di N Numeri (1 Strategia) con Gestione di N non Divisibile per il numero di Threads

## Descrizione
Questa versione del programma di somma di `N` numeri con **OpenMP** introduce un **meccanismo di correzione** per gestire il caso in cui `N` non sia divisibile per il numero di thread `t`. Questo garantisce che tutti gli elementi vengano correttamente assegnati ai thread, evitando errori o calcoli incompleti.

## Differenze rispetto alla versione classica
- **Gestione del resto della divisione `N/t`**: viene calcolato `r = N % t`.
- **Distribuzione più equa dei dati**:
  - I thread con identificativo inferiore a `r` ricevono **un elemento in più**.
  - Gli altri thread iniziano a lavorare con un **offset (step) pari a `r`**.
- **Maggiore bilanciamento del carico tra i thread**, evitando squilibri quando `N` non è multiplo di `t`.

## Funzionamento
1. L'utente sceglie il numero di core (`np`).
2. L'utente inserisce `N`, il numero di valori da sommare.
3. Viene allocato dinamicamente il vettore `a` e vengono letti gli `N` numeri.
4. I thread eseguono la somma in parallelo:
   - Il calcolo di `nloc` (numero di elementi per thread) tiene conto del resto `r`.
   - Viene applicato un meccanismo di correzione per distribuire equamente gli elementi tra i thread.
   - Ogni thread calcola la propria somma parziale e la aggiunge a `sumtot`.
5. Il risultato viene stampato a schermo.

## Esempio di Input/Output
**Esempio di esecuzione:**
```
Con quanti core vuoi eseguire il calcolo?
3
Inserire N
7
Inserire i numeri da sommare
1 2 3 4 5 6 7
sono 0, di 3: numeri 2, r=1
sono 1, di 3: numeri 2, r=1
sono 2, di 3: numeri 2, r=1
sono 0, di 3: numeri 3, r=1, passo=0
sono 1, di 3: numeri 2, r=1, passo=1
sono 2, di 3: numeri 2, r=1, passo=1
sono 0, di 3: numeri 3, r=1, la mia sum=6.000000
sono 1, di 3: numeri 2, r=1, la mia sum=9.000000
sono 2, di 3: numeri 2, r=1, la mia sum=13.000000
somma totale: 28.000000
```

## Struttura del Codice
- Allocazione dinamica del vettore `a`.
- Lettura dei dati da input.
- Calcolo del bilanciamento tra i thread quando `N` non è divisibile per `t`.
- Uso della direttiva `#pragma omp parallel` per la somma parallela.
- Stampa del risultato.


