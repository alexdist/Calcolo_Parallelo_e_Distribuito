# Prodotto Matrice per Vettore 1 Strategia (Divisione per Righe)

## Descrizione
Questo programma calcola il **prodotto tra una matrice e un vettore** utilizzando **OpenMP** e applicando la **prima strategia di parallelizzazione**, ovvero la **divisione della matrice per righe**. Ogni thread elabora un sottoinsieme delle righe della matrice, moltiplicandole per il vettore `x` e producendo il risultato nel vettore `v`.

## Metodo di Calcolo
Dato:
- una matrice `A` di dimensioni `N × M`
- un vettore `x` di dimensione `M`

Il prodotto matrice-vettore produce un vettore `v` di dimensione `N` tale che:

![Formula prodotto matrice per vettore](prodotto_matrice_vettore.png)


## Strategia di Parallelizzazione: Divisione per Righe
La matrice viene suddivisa tra i thread **per righe**:
- Il numero totale di righe viene diviso per il numero di thread (`num_threads`).
- Se il numero di righe non è divisibile esattamente, i primi `r` thread ricevono una riga in più.
- Ogni thread calcola i valori del vettore `v` corrispondenti alle righe assegnate.

## Parallelizzazione con OpenMP
```c
#pragma omp parallel private(nloc, step, r, tid) shared(matrice, v, x, righe, colonne, num_threads)
```
- `private(...)`: variabili locali per ogni thread.
- `shared(...)`: variabili condivise tra i thread.
- All'interno del blocco parallelo:
  - si calcola `nloc`, ovvero il numero di righe da elaborare per ogni thread.
  - `r = righe % num_threads` rappresenta il numero di righe extra da distribuire tra i primi thread.
  - Se `tid < r`, il thread riceve una riga in più e `step = 0`, altrimenti `step = r`.

### Calcolo dell'indice `index`
L'indice `index` rappresenta la riga della matrice da elaborare da parte del thread corrente. Si calcola come:

```c
index = i + tid * nloc + step;
```

Dove:
- `i` è l'indice locale del ciclo del thread (da 0 a `nloc-1`)
- `tid * nloc` sposta l'intervallo in base al numero del thread
- `step` corregge lo spostamento per compensare le righe extra già assegnate ai primi `r` thread

Questa formula garantisce che **ogni thread lavori su righe disgiunte** e che l'intera matrice venga coperta anche se il numero di righe non è perfettamente divisibile.

Ogni thread esegue:
```c
for(int i=0;i<nloc;i++) {
    int index = i+tid*nloc+step;
    for(int j=0;j<colonne;j++) {
        v[index] += matrice[index][j] * x[j];
    }
}
```

## Funzionamento del Programma
1. L'utente sceglie il numero di core (`np`).
2. L'utente inserisce il numero di righe e colonne della matrice.
3. L'utente inserisce gli elementi del vettore `x`.
4. L'utente inserisce gli elementi della matrice.
5. Il programma esegue il calcolo parallelo del prodotto `A * x`.
6. Il vettore risultato `v` viene stampato.

## Esempio di Input/Output
```
Con quanti core vuoi eseguire il calcolo? 2
Inserire il numero di righe: 3
Inserire il numero di colonne: 3

Inserire gli elementi del vettore x: 
Elemento [0]-->1
Elemento [1]-->2
Elemento [2]-->3

Inserire gli elementi della matrice
Inserisci l'elemento[0][0]-->1
Inserisci l'elemento[0][1]-->0
Inserisci l'elemento[0][2]-->2
...

Matrice:
1 0 2
...

Prodotto MatricexVettore v:
7
...
```

## Struttura del Codice
- **Input dell'utente** per dimensioni e valori.
- **Allocazione dinamica** della matrice e dei vettori.
- **Divisione del lavoro** tra i thread.
- **Esecuzione parallela** del calcolo riga per riga.
- **Stampa del risultato**.




