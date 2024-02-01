#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

/*Creare un programma in C che rispetta i seguenti requisiti:
- genera un processo figlio
- padre e figlio si identificano
- il figlio chiede all'utente di inserire cinque numeri e li memorizza in un array
- il figlio, tramite una pipe, inoltra l'array al padre
- il padre stampa i numeri ricevuti dopo averli moltiplicati per un numero inserito dall'utente e chiesto dal padre stesso.*/

int status; // per wait ()

int main(int argc, char *argv[])
{
    int fd[2];
    int p;
    int n;

    if (pipe(fd) == -1) // creazione pipe ()
    {
        printf("\nErrore nella creazione della pipe");
        exit(-1);
    }

    p = fork();
    if (p < 0)
    {
        printf("\nErrore nella creazione del figlio");
    }
    if (p == 0) // figlio
    {
        close(fd[0]); // figlio deve scrivere nella pipe quindi chiude in lettura
        printf("\nSono il figlio. Il mio PID è: %d, mio padre ha PID: %d", getpid(), getppid());

        int numeri[5];
        printf("\nInserisci cinque numeri\n");
        for (int i = 0; i < 5; i++)
        {
            printf("numero %d\n", i + 1);
            scanf("%d", &numeri[i]);
        }

        if (write(fd[1], numeri, sizeof(numeri)) == -1) // scrittura
        {
            printf("\nErrore di strittura nella pipe");
            exit(-1);
        }
        close(fd[1]);
    }
    else // padre
    {
        wait(&status); // in attesa che il figlio termini
        int nMoltiplica;
        int destinazioneNumeri[5];

        close(fd[1]); // padre deve leggere nella pipe quindi chiude in scrittura

        printf("\nSono il padre. Il mio PID è: %d, mio figlio ha PID: %d", getpid(), p);

        printf("\nCon che numero vuoi moltiplicare i numeri inseriti?\n");
        scanf("%d", &nMoltiplica);
        printf("moltiplico i numeri inseriti con %d\n", nMoltiplica);

        n = read(fd[0], destinazioneNumeri, sizeof(destinazioneNumeri)); // lettura
        if (n == -1)
        {
            printf("\nErrore nell'apertura della pipe");
            exit(-1);
        }
        else
        {

            printf("\nNumeri ricevuti e moltiplicati:\n");
            for (int i = 0; i < 5; i++)
            {
                printf("%d\n", destinazioneNumeri[i] * nMoltiplica);
            }
            close(fd[0]);
        }
    }

    return 0;
}