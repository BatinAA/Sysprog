#include <stdio.h>
#include <stdlib.h> 
#include <signal.h> // Функции sigaction(), sigsuspend(), sig*()
#include <unistd.h> // Функция alarm()

void handle_signal(int signal);
void handle_sigalrm(int signal);
void do_sleep(int seconds);

/* Как использовать:

    Программа напечатает PID. Открываем другой терминал чтобы отправить сигнал процессу.
    Например:
    $ kill -HUP <pid>
    $ kill -USR1 <pid>
    $ kill -ALRM <pid>

    Чтобы выйти из процесса: ^C ( = SIGINT) или SIGKILL, SIGTERM
*/

int main() {
    struct sigaction sa;

    // Печатает PID процесса
    printf("My pid is: %d\n", getpid());

    // Установка обработчика sighup
    sa.sa_handler = &handle_signal;

    // Перезагрузка системного вызова, если это возможно
    sa.sa_flags = SA_RESTART;

    // Блокирует каждый сигнал во время обработчика
    sigfillset(&sa.sa_mask);

    // Перехват SIGHUP и SIGINT
    if (sigaction(SIGHUP, &sa, NULL) == -1) {
        perror("Error: cannot handle SIGHUP");
    }

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Error: cannot handle SIGUSR1");
    }

    // Will always fail, SIGKILL is intended to force kill your process
    if (sigaction(SIGKILL, &sa, NULL) == -1) {
        perror("Cannot handle SIGKILL"); // Will always happen
        printf("You can never handle SIGKILL anyway...\n");
    }

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error: cannot handle SIGINT"); // Should not happen
    }

    for (;;) {
        printf("\nSleeping for ~3 seconds\n");
        do_sleep(3); // Later to be replaced with a SIGALRM
    }
}

void handle_signal(int signal) {
    /*
     * Please note that printf et al. are NOT safe to use in signal handlers.
     * Look for async safe functions.
     */
    const char *signal_name;
    sigset_t pending;

    // Find out which signal we're handling
    switch (signal) {
        case SIGHUP:
            signal_name = "SIGHUP";
            break;
        case SIGUSR1:
            signal_name = "SIGUSR1";
            break;
        case SIGINT:
            printf("Caught SIGINT, exiting now\n");
            exit(0);
        default:
            fprintf(stderr, "Caught wrong signal: %d\n", signal);
            return;
    }

    printf("Caught %s, sleeping for ~3 seconds\n"
           "Try sending another SIGHUP / SIGINT / SIGALRM "
           "(or more) meanwhile\n", signal_name);

    /*
    Действительно, все сигналы блокируются во время этого обработчика. 
    Но, по крайней мере, если отправить 2 других SIGHUP, 
    будет доставлен только один, то есть сигналы не ставятся в очередь.
    Однако, если отправить HUP, INT, HUP, можно увидеть, 
    что оба INT и HUP поставлены в очередь.
    */

    do_sleep(3);
    printf("Done sleeping for %s\n", signal_name);

    // Ожидание ввода, пока идет сон
    sigpending(&pending);
    if (sigismember(&pending, SIGHUP)) {
        printf("A SIGHUP is waiting\n");
    }
    if (sigismember(&pending, SIGUSR1)) {
        printf("A SIGUSR1 is waiting\n");
    }

    printf("Done handling %s\n\n", signal_name);
}

void handle_sigalrm(int signal) {
    if (signal != SIGALRM) {
        fprintf(stderr, "Caught wrong signal: %d\n", signal);
    }

    printf("Got sigalrm, do_sleep() will end\n");
}

void do_sleep(int seconds) {
    struct sigaction sa;
    sigset_t mask;

    sa.sa_handler = &handle_sigalrm; // Перехват и игнорирование SIGALRM
    sa.sa_flags = SA_RESETHAND; // Удаление обработчика после первого сигнала
    sigfillset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);

    // Получение текущего signal mask
    sigprocmask(0, NULL, &mask);

    // Разблокировка SIGALRM
    sigdelset(&mask, SIGALRM);

    // Ожидание с этой mask
    alarm(seconds);
    sigsuspend(&mask);

    printf("sigsuspend() returned\n");
}