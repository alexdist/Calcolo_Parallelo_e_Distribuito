# Calcolo del Pi Greco

## Descrizione
Questo programma utilizza **OpenMP** per calcolare il valore approssimato di **π (pi greco)** attraverso l'**integrazione numerica** del metodo dei **rettangoli**. La somma degli intervalli viene eseguita in parallelo per migliorare l'efficienza computazionale.

## Metodo di Calcolo
L'approssimazione di π è ottenuta attraverso l'integrazione della funzione:

\[
f(x) = \frac{4}{1 + x^2}
\]

nell'intervallo \([0,1]\). Il valore dell'integrale rappresenta un'approssimazione di π.

L'algoritmo suddivide l'intervallo in **N sottointervalli**, calcola il valore della funzione nel punto medio di ogni intervallo e somma i contributi per ottenere il risultato finale:

\[
\pi \approx \sum_{i=1}^{N} f(x_i) \cdot dx
\]

dove \(dx\) è la larghezza di ciascun intervallo:

\[
dx = \frac{1}{N}
\]

## Parallelizzazione con OpenMP
La somma viene calcolata in parallelo utilizzando **OpenMP**, con la direttiva:

```c
#pragma omp parallel for private(x, f, i) shared(dx) reduction(+:sum)
```

- `parallel for`: distribuisce il ciclo tra più thread.
- `private(x, f, i)`: ogni thread ha una copia locale delle variabili.
- `shared(dx)`: la variabile `dx` è condivisa tra tutti i thread.
- `reduction(+:sum)`: garantisce che la somma parziale di ciascun thread venga combinata correttamente.

## Funzionamento del Programma
1. Il numero di intervalli è predefinito con `#define N 100000000`.
2. Si calcola la larghezza dell'intervallo `dx`.
3. Il ciclo for calcola la somma dell'area dei rettangoli in parallelo.
4. Il valore finale di π viene calcolato e stampato a schermo.

## Esempio di Output
```
Numero di intervalli: 100000000
PI 3.141592653589793116000000
```

## Struttura del Codice
- Definizione della costante `N` (numero di intervalli).
- Calcolo della larghezza `dx`.
- Calcolo parallelo della somma dell'area dei rettangoli.
- Stampa del valore finale di π.

