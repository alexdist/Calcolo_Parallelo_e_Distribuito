# Prodotto di una Matrice per uno Scalare (Divisione per Righe)

## Descrizione
Questo programma utilizza **OpenMP** per eseguire il **prodotto di una matrice per uno scalare** sfruttando la **prima strategia di parallelizzazione**, ovvero la **divisione per righe**. Ogni thread elabora una o più righe della matrice, moltiplicando ciascun elemento per un valore scalare `alpha`.

## Metodo di Calcolo
Il prodotto di una matrice `A` di dimensione `N × M` per uno scalare `α` si calcola come:

\[
A'_{i,j} = A_{i,j} \times \alpha \quad \forall \quad i \in [0,N-1], \quad j \in [0,M-1]
\]

Dove `A'` è la matrice risultante.

## Parallelizzazione con OpenMP
La parallelizzazione avviene sulla variabile `i`, ovvero la riga della matrice. Questo garantisce che ogni thread elabori un sottoinsieme di righe:

```c
#pragma omp parallel for shared(N,M,matrice,alpha) private(i,j) num_threads(np)
```

- `parallel for`: divide il ciclo esterno tra i thread.
- `shared(N,M,matrice,alpha)`: variabili condivise tra i thread.
- `private(i,j)`: variabili locali ai thread.
- `num_threads(np)`: imposta il numero di thread specificato dall'utente.

### **Strategia di Divisione per Righe**
- Ogni thread si occupa di elaborare **una o più righe complete** della matrice.
- Questo approccio riduce il rischio di **race condition**, poiché i thread lavorano su righe diverse.
- **Scalabilità elevata**, specialmente per matrici grandi con molte righe.

## Funzionamento del Programma
1. L'utente sceglie il numero di core (`np`).
2. L'utente inserisce il numero di righe `N` e colonne `M` della matrice.
3. L'utente fornisce il valore dello scalare `alpha`.
4. Viene allocata la matrice e letti i valori da input.
5. Il prodotto tra la matrice e `alpha` viene calcolato in parallelo.
6. Il risultato viene stampato e la memoria allocata viene liberata.

## Esempio di Input/Output
**Esempio di esecuzione:**
```
Con quanti core vuoi eseguire il calcolo? 2

Inserisci le righe della matrice(NxM): 2
Inserisci le colonne della matrice(NxM): 3

Inserisci lo scalare alpha: 3

Inserisci gli elementi della matrice (2x3)
Inserisci l'elemento[0][0]-->1
Inserisci l'elemento[0][1]-->2
Inserisci l'elemento[0][2]-->3
Inserisci l'elemento[1][0]-->4
Inserisci l'elemento[1][1]-->5
Inserisci l'elemento[1][2]-->6

Matrice originale:
1 2 3
4 5 6

Matrice dopo il prodotto con alpha:
3 6 9
12 15 18
```

## Struttura del Codice
- **Allocazione dinamica della matrice**.
- **Lettura dei dati** da input.
- **Stampa della matrice originale**.
- **Calcolo parallelo del prodotto** della matrice per `alpha`.
- **Stampa del risultato**.
- **Deallocazione della memoria**.


