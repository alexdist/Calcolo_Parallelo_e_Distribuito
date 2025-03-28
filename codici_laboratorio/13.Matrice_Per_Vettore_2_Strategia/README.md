# Prodotto Matrice per Vettore con OpenMP (Divisione per Colonne)

## Descrizione
Questo programma calcola il **prodotto tra una matrice e un vettore** utilizzando **OpenMP** e applicando la **seconda strategia di parallelizzazione**, ovvero la **divisione della matrice per colonne**. Ogni thread elabora un sottoinsieme delle colonne della matrice e aggiorna il vettore risultato `v`.

## Metodo di Calcolo
Dati:
- una matrice `A` di dimensioni `N × M`
- un vettore `x` di dimensione `M`

Il prodotto matrice-vettore produce un vettore `v` di dimensione `N` tale che:

![Formula prodotto matrice vettore](prodotto_matrice_vettore.png)

## Strategia di Parallelizzazione: Divisione per Colonne
La matrice viene suddivisa tra i thread **per colonne**:
- Il numero totale di colonne viene diviso per il numero di thread (`num_threads`).
- Se il numero di colonne non è divisibile esattamente, i primi `r` thread ricevono una colonna in più.
- Ogni thread aggiorna i valori del vettore `v` per tutte le righe usando solo le colonne assegnate.

### Calcolo dell'indice `index`
```c
index = j + tid * nloc + step;
```
- `j` è l'indice locale del ciclo del thread (da 0 a `nloc-1`)
- `tid * nloc` determina la partizione per thread
- `step` corregge lo spostamento per i thread che non ricevono colonne extra

Ogni thread esegue:
```c
for(int j = 0; j < nloc; j++) {
    int index = j + tid * nloc + step;
    for(int i = 0; i < righe; i++) {
        #pragma omp atomic
        v[i] += matrice[i][index] * x[index];
    }
}
```

La clausola `#pragma omp atomic` garantisce che l'accesso concorrente a `v[i]` sia sicuro, evitando condizioni di race.

## Differenza rispetto alla Strategia per Righe
| Strategia                | Divisione per Righe                               | Divisione per Colonne                               |
|--------------------------|---------------------------------------------------|-----------------------------------------------------|
| Approccio                | Ogni thread elabora un sottoinsieme di righe      | Ogni thread elabora un sottoinsieme di colonne      |
| Accesso memoria          | Maggiore località spaziale (riga-contigua)        | Accesso a colonne, meno efficiente per cache        |
| Scrittura nel vettore `v`| Scrittura esclusiva (righe diverse)               | Scrittura concorrente gestita con `atomic`          |
| Rischio conflitti        | Basso                                              | Evitato tramite sincronizzazione                    |

## Funzionamento del Programma
1. L'utente sceglie il numero di core (`np`).
2. L'utente inserisce le dimensioni della matrice e del vettore.
3. Vengono allocati e riempiti la matrice e il vettore `x`.
4. Ogni thread calcola la parte del prodotto corrispondente alle colonne assegnate.
5. Il risultato viene stampato.

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
- **Divisione del lavoro** per colonne.
- **Esecuzione parallela** con sincronizzazione su `v[i]`.
- **Stampa del risultato**.


