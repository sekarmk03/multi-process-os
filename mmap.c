#include <stdlib.h> /* exit() */
#include <unistd.h> /* fork() and getpid() */
#include <stdio.h>  /* printf() */
#include <string.h>
#include <time.h>
#include <errno.h>    /* errno */
#include <sys/wait.h> /*wait*/
#include <sys/mman.h>

#define MSGSIZE 2

int sum;

int main(int argc, char **argv)
{
    int n, m;
    int pid;
    int got_pid, status;
    scanf("%d", &n);
    scanf("%d", &m);

    int size = MSGSIZE * sizeof(int);
    // buat memory map
    void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    printf("Mapped at %p\n", addr);
    int *shared = addr;

    switch (pid = fork())
    {
    case 0: /* fork returns 0 ke proses anak */
        for (int i = 0; i < n; i++)
        {
            fork();
        }
        printf("Mulai Producer %d\n", getpid());
        // tulis data ke pipe
        srand(getpid());
        for (int i = 0; i < MSGSIZE; i++)
        {
            shared[i] = (rand() % (10 - 0 + 1)) + 1;
            printf("proses Producer ke %d untuk %d = %d\n", i + 1, getpid(), shared[i]);
        }
        printf("\n");
        break;
    default: /* fork returns pid ke proses ortu */
        while (got_pid = wait(&status))
        { /* sleep sampai sesuatu terjadi */
            /* wait selesai,  bangun */
            if (got_pid == pid)
                for (int i = 0; i < m; i++)
                {
                    fork();
                }
            printf("Mulai Consumer %d\n", getpid());
            for (int i = 0; i < MSGSIZE; i++)
            {
                sum = sum + shared[i];
            }
            printf("Total untuk proses Consumer %d = %d\n", getpid(), sum);
            break;
            if ((got_pid == -1) && (errno != EINTR))
            {
                /* ada error*/
                perror("waitpid");
            }
        }
    case -1: /* error */
        exit(1);
    }
    exit(0);
}
