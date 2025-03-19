# Prodotto Scalare di Due Vettori (Seconda Strategia di Collezione dei Risultati)

## Descrizione
Questo programma utilizza **OpenMP** per calcolare il **prodotto scalare** di due vettori di dimensione `N`, implementando la **seconda strategia di collezione dei risultati**. L'operazione viene parallelizzata utilizzando la direttiva `#pragma omp parallel for` con l'uso della riduzione (`reduction(+:sum)`) per gestire correttamente l'aggiornamento della somma totale.

## Metodo di Calcolo
Il prodotto scalare tra due vettori `A` e `B` è definito come:

![Formula del Prodotto Scalare](prodotto_scalare_formula.png)

Dove `S` è il risultato della somma degli elementi **moltiplicati componente per componente**.

## Differenza con la Prima Strategia
- **Prima strategia:**
  - Ogni thread calcola il proprio contributo parziale e lo aggiorna in una variabile **condivisa**.
  - Si utilizza una **sezione critica (`#pragma omp critical`)** per evitare race conditions.
  - Questo introduce un **collo di bottiglia** poiché solo un thread alla volta può aggiornare la somma totale.

- **Seconda strategia (usata in questo codice):**
  - Utilizza la direttiva **`reduction(+:sum)`**, che permette a ogni thread di accumulare la propria somma locale senza bisogno di una sezione critica.
  - OpenMP si occupa automaticamente di sommare i risultati parziali alla fine dell'elaborazione.
  - **Più efficiente** rispetto alla prima strategia, in quanto elimina la necessità di sincronizzazione esplicita tra i thread.

## Parallelizzazione con OpenMP
L'operazione viene eseguita in parallelo con OpenMP:
```c
#pragma omp parallel for schedule(dynamic) reduction(+:sum) num_threads(np)
```
- `parallel for`: distribuisce il ciclo tra più thread.
- `schedule(dynamic)`: la distribuzione dei calcoli tra i thread avviene dinamicamente, adattandosi a possibili sbilanciamenti nel carico di lavoro.
- `reduction(+:sum)`: gestisce automaticamente l'aggregazione dei risultati parziali senza la necessità di sezioni critiche.
- `num_threads(np)`: imposta il numero di thread specificato dall'utente.

## Funzionamento del Programma
1. L'utente inserisce il numero di core (`np`).
2. L'utente definisce la dimensione `N` dei vettori `A` e `B`.
3. Vengono allocati e riempiti i due vettori.
4. Il programma calcola il **prodotto scalare** in parallelo con la **seconda strategia di collezione dei risultati**.
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
[0] = 1
[1] = 2
[2] = 3
[3] = 4
Risultato: 30
Tempo: 0.000001
```

## Struttura del Codice
- Allocazione dinamica dei vettori `A` e `B`.
- Lettura dei dati da input.
- Stampa dei vettori.
- Calcolo parallelo del prodotto scalare con la **seconda strategia di collezione dei risultati**.
- Stampa del risultato e del tempo di esecuzione.


