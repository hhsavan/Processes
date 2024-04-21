#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h> // Include the header file for wait()
int signalCount = 0;

void custom_handler(int signal)
{
    printf("Custom handler invoked for signal %d, signalCount: %d\n", signal, signalCount);
    if (signalCount == 5)
        exit(0);
    signalCount++;
}

void CustomSignalHandlerHelper()
{
    // SIGINT sinyali için özel işleyiciyi tanımla
    signal(SIGINT, custom_handler);

    printf("Press Ctrl+C to trigger SIGINT signal\n");
    // Sonsuz döngüde bekleyerek programın sonlanmasını önle
    while (1)
    {
        sleep(1);
    }
}

void SignaliEngelle()
{
    // SIGINT sinyalini engelle
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigprocmask(SIG_BLOCK, &set, NULL);

    printf("SIGINT signal is blocked. Press Ctrl+C to unblock\n");

    // Sonsuz döngüde bekleyerek Ctrl+C'ye basıldığında sinyalin engellemesini kaldır
    while (1)
    {
        sleep(1);
    }
}
// Custom signal handler for SIGUSR1
void sigusr1_handler(int sig)
{
    printf("Custom signal caught.\n");
}
// usage: kill -SIGUSR1 <pid>
void CustomSignalImplementation()
{
    // Register the custom signal handler for SIGUSR1
    signal(SIGUSR1, sigusr1_handler);

    printf("Custom signal is registered. Use 'kill -SIGUSR1 <pid>' to send the signal.\n");

    // Infinite loop to keep the program running
    while (1)
    {
        sleep(1);
    }
}

// Function to handle the custom signal sent by the parent process
void custom_signal_handler(int sig)
{
    // Write the message specified by the parent process
    printf("Child process received custom signal from parent.\n");
}

/*
a child process in a while loop it writes something in every 2 seconds. with sleep
mother process send child process to a custom signal to write another thing to secreen.
when child process take this custom signal, it will write what parent process says
*/
void CustomSignalExample2()
{
    // Register the custom signal handler for SIGUSR1
    signal(SIGUSR1, custom_signal_handler);

    // Fork a child process
    pid_t pid = fork();

    if (pid == -1)
    {
        // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child process
        printf("Child process started.\n");

        // Child process continuously writes something every 2 seconds
        while (1)
        {
            printf("Child: Writing something to screen.\n");
            sleep(2);
        }
    }
    else
    {
        // Parent process
        printf("Parent process started.\n");

        // Wait for a moment
        sleep(5);

        // Send the custom signal (SIGUSR1) to the child process
        printf("Parent: Sending custom signal to child.\n");
        kill(pid, SIGUSR1);

        // Wait for the child process to finish
        wait(NULL);
        printf("Parent process exiting.\n");
    }
}

void SigAction()
{
    // Define the action to be taken for the signal
    struct sigaction action;
    action.sa_handler = custom_handler; // Set the signal handler function
    sigemptyset(&action.sa_mask);       // Clear the signal mask during signal handling
    action.sa_flags = 0;                // No special flags

    // Register the custom signal handler using sigaction
    if (sigaction(SIGINT, &action, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Custom signal handler registered for SIGINT (Ctrl+C).\n");

    // Infinite loop to keep the program running
    while (1)
    {
    }
}

// Custom signal handler function
void ProcessWakeUpFunc(int sig)
{
    printf("Process woken up by signal.\n");
}
void WakeAProcessWithAsignal()
{
    // Register the custom signal handler for SIGUSR1
    signal(SIGUSR1, ProcessWakeUpFunc);

    printf("Process is sleeping. Use 'kill -SIGUSR1 <pid>' to wake it up.\n");
    pid_t fd = fork();

    if (fd == 0)
    {
        printf("The child process is sleeping\n");
        pause(); // Sleep until a signal is received
        printf("The child process is waken up\n");
        exit(0);
    }

    sleep(5);
    kill(fd,SIGUSR1);
}

void interruptOldugundaKaldigiYerdenDevam(){

    int cnt, fd=0, BUF_SIZE=100;
    void *buf;
    //eğer read işlemi bir interrupt tarafından kesilirse, cpuyu aldığında tekrar dener.
    //bu kullanım daha doğru
    while ((cnt = read(fd, buf, BUF_SIZE)) == -1 && errno == EINTR)
    continue;
 
    // bunda ise denemez
    while ((cnt = read(fd, buf, BUF_SIZE)) == -1)
    continue;
}
