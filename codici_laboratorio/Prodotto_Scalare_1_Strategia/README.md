# Prodotto Scalare di Due Vettori con OpenMP

## Descrizione
Questo programma utilizza **OpenMP** per calcolare il **prodotto scalare** di due vettori di dimensione `N`. L'operazione viene parallelizzata utilizzando la direttiva `#pragma omp parallel for` con la strategia di **schedulazione dinamica** per distribuire il carico tra i thread in modo efficiente.

## Metodo di Calcolo
Il prodotto scalare tra due vettori `A` e `B` è definito come:

\[
S = \sum_{i=0}^{N-1} A[i] \cdot B[i]
\]

Dove `S` è il risultato della somma degli elementi **moltiplicati componente per componente**.

## Parallelizzazione con OpenMP
L'operazione viene eseguita in parallelo con OpenMP:
```c
#pragma omp parallel for shared(sum) schedule(dynamic) num_threads(np)
```
- `parallel for`: distribuisce il ciclo tra più thread.
- `shared(sum)`: la variabile `sum` è condivisa tra i thread.
- `schedule(dynamic)`: la distribuzione dei calcoli tra i thread avviene dinamicamente, adattandosi a possibili sbilanciamenti nel carico di lavoro.
- `num_threads(np)`: imposta il numero di thread specificato dall'utente.
- `#pragma omp critical`: evita race condition quando più thread aggiornano `sum` simultaneamente.

## Funzionamento del Programma
1. L'utente inserisce il numero di core (`np`).
2. L'utente definisce la dimensione `N` dei vettori `A` e `B`.
3. Vengono allocati e riempiti i due vettori.
4. Il programma calcola il **prodotto scalare** in parallelo.
5. Il tempo di esecuzione viene misurato con `omp_get_wtime()`.
6. Il risultato finale viene stampato a schermo.

## Esempio di Input/Output
**Esempio di esecuzione:**
```
Con quanti core vuoi eseguire il calcolo?: 2

Inserire N: 4

Inserire gli elementi nel vettore A
Elemento 0-esimo: 1
Elemento 1-esimo: 2
Elemento 2-esimo: 3
Elemento 3-esimo: 4

Inserire gli elementi nel vettore B
Elemento 0-esimo: 5
Elemento 1-esimo: 6
Elemento 2-esimo: 7
Elemento 3-esimo: 8

Vettore A:
[0] = 1
[1] = 2
[2] = 3
[3] = 4
Vettore B:
[0] = 5
[1] = 6
[2] = 7
[3] = 8
Risultato: 30
Tempo: 0.000001
```

## Struttura del Codice
- Allocazione dinamica dei vettori `A` e `B`.
- Lettura dei dati da input.
- Stampa dei vettori.
- Calcolo parallelo del prodotto scalare con OpenMP.
- Stampa del risultato e del tempo di esecuzione.


