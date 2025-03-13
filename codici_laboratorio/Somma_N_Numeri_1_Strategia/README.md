# Somma di N Numeri

## Descrizione
Questo programma utilizza **OpenMP** per calcolare la somma di `N` numeri sfruttando la **prima strategia di collezione delle somme parziali**. Ogni thread calcola una somma parziale di un sottoinsieme dei numeri e la aggiunge alla somma totale `sumtot`.

## Funzionamento
1. L'utente sceglie il numero di core (`np`).
2. L'utente inserisce `N`, il numero di valori da sommare.
3. Viene allocato dinamicamente il vettore `a` e vengono letti gli `N` numeri.
4. La somma viene calcolata in parallelo:
   - Ogni thread calcola una **somma parziale** del proprio sottoinsieme di numeri.
   - Le somme parziali vengono poi **sommate globalmente** in `sumtot`.
5. Il risultato viene stampato a schermo.

## Esempio di Input/Output
**Esempio di esecuzione:**
```
Con quanti core vuoi eseguire il calcolo?
2
Inserire N
6
Inserire i numeri da sommare
1 2 3 4 5 6
sono 0, di 2: numeri 3
sono 1, di 2: numeri 3
somma totale: 21.000000
```

## Struttura del Codice
- Allocazione dinamica del vettore `a`.
- Lettura dei dati da input.
- Divisione dei dati tra i thread con `#pragma omp parallel`.
- Raccolta delle somme parziali e aggiornamento della somma totale `sumtot`.
- Stampa del risultato.


