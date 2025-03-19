
# Somma Vettori V2

## Descrizione
Questa è una versione migliorata del programma di somma vettoriale con OpenMP. La modifica principale riguarda la gestione del caso in cui `N` non sia divisibile per `t` (numero di thread).

## Modifica Principale
- Viene calcolato il resto della divisione `r = N % t`.
- I thread con identificativo inferiore a `r` calcolano un elemento in più del vettore soluzione `c`.
- La variabile `step` viene introdotta per garantire un'allocazione corretta degli elementi tra i thread.

## Funzionamento
1. L'utente sceglie il numero di core (`np`).
2. L'utente inserisce la dimensione del vettore `N`.
3. Il programma alloca dinamicamente i vettori `a`, `b` e `c`.
4. Viene gestita l'eventuale divisione non equa degli elementi tra i thread.
5. Il risultato della somma viene stampato a schermo.

## Esempio di Input/Output
**Esempio di esecuzione:**
```
Con quanti core vuoi eseguire il calcolo?
3
Inserire N
7
Inserire gli elementi del vettore a
1 2 3 4 5 6 7
Inserire gli elementi del vettore b
8 9 10 11 12 13 14
sono 0, di 3: numeri 2, r=1
sono 1, di 3: numeri 2, r=1
sono 2, di 3: numeri 1, r=1
sono 0, di 3: numeri 3, r=1, passo=0
sono 1, di 3: numeri 2, r=1, passo=1
sono 2, di 3: numeri 2, r=1, passo=1
9.000000 11.000000 13.000000 15.000000 17.000000 19.000000 21.000000
```

## Struttura del Codice
- Allocazione dinamica dei vettori `a`, `b` e `c`.
- Lettura dei dati da input.
- Calcolo del bilanciamento tra i thread quando `N` non è divisibile per `t`.
- Uso della direttiva `#pragma omp parallel` per la somma parallela.
- Stampa del risultato.


