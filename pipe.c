#include <stdlib.h> // exit()
#include <unistd.h> // fork() and getpid()
#include <stdio.h>  // printf()
#include <string.h>
#include <time.h>
#include <errno.h>    // errno
#include <sys/wait.h> // wait

#define MSGSIZE 2

int main(int argc, char **argv)
{
    int producer, consumer;
    printf("\nMasukkan jumlah Producer: ");
    scanf("%d", &producer);
    printf("\nMasukkan jumlah Consumer: ");
    scanf("%d", &consumer);
    printf("\n");
    int count = 0;
    char inbuf[MSGSIZE]; // buffer
    int pid;
    int got_pid, status;
    int fd[2]; // pipe

    /* create pipe */
    if (pipe(fd) < 0)
    {
        exit(1); // error
    }

    switch (pid = fork())
    {
    case 0:
        /* fork returns 0 ke proses anak */
        for (int i = 0; i < producer; i++)
        {
            fork();
        }
        printf("Mulai Producer %d\n", getpid());

        /* tulis data ke pipe */
        srand(getpid());

        int arrIntChild[MSGSIZE];
        for (int i = 0; i < MSGSIZE; i++)
        {
            arrIntChild[i] = (rand() % (10 - 0 + 1)) + 1;
            printf("Proses Producer ke %d untuk %d = %d\n", i + 1, getpid(), arrIntChild[i]);
        }
        write(fd[1], arrIntChild, sizeof(arrIntChild));
        printf("\n");
        break;

    default:
        /* fork returns pid ke proses ortu */
        while (got_pid = wait(&status))
        {
            /* sleep sampai sesuatu terjadi */
            /* wait selesai,  bangun */
            for (int i = 0; i < consumer; i++)
            {
                fork();
            }
            if (got_pid == pid)
                printf("Mulai Consumer %d\n", getpid());
                
            /* tutup bagian output dari pipe */
            close(fd[1]);
            
            /* baca yang ditulis child dari pipe */
            int arrIntParent[MSGSIZE];
            
            /* baca yang ditulis child dari pipe */
            read(fd[0], arrIntParent, sizeof(arrIntParent));
            
            /* membuat variabel untuk menjumlahkan item, menyimpan pid dan jumlah item customer */
            int sum = 0, arrPIDCustomer[MSGSIZE], arrSumCustomer[MSGSIZE];
            for (int i = 0; i < MSGSIZE; i++)
            {
                sum = sum + arrIntParent[i];
            }
            
            for(int i = 0; i < consumer; i++)
            {
                if(sum > 22 || sum < 0)
                {
                    printf("Consumer %d kehabisan item!\n", getpid());
                }
                else 
                {
                    printf("Total untuk proses Consumer %d dari %d = %d\n", getpid(), pid, sum);
                }
            }
            break;
            
            if ((got_pid == -1) && (errno != EINTR))
            {
                /* ada error */
                perror("waitpid");
            }
        }
    case -1: // error
        exit(1);
    }
    exit(0);
}
