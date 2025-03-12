Compilazione ed Esecuzione

Queste istruzioni valgono per tutti i codici presenti nella cartella.

Compilazione

Per compilare un codice sorgente in C con OpenMP, utilizzare il seguente comando:

gcc -fopenmp -o nome-eseguibile nome-codice.c

Dove:

nome-eseguibile è il nome che si vuole dare al file compilato.

nome-codice.c è il file sorgente da compilare.

L'opzione -fopenmp abilita il supporto a OpenMP.

Esempio:

gcc -fopenmp -o somma-vettori somma-vettori.c

Esecuzione

Una volta compilato, il codice può essere eseguito con:

./nome-eseguibile

Esempio:

./somma-vettori

Requisiti

Compilatore gcc con supporto a OpenMP.

OpenMP installato sul sistema.

Queste istruzioni sono valide per tutti i file presenti nella cartella di laboratorio.
