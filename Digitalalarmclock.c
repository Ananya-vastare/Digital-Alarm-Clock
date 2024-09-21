#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#define CLEAR_SCREEN() system("cls")

char buffer[9];
int hours, minutes, seconds;
HANDLE hThread;

void beeping() // To produce sound
{
    int frequency = 1000; // 1000 Hz (1 kHz)
    int duration = 500;   // 500 milliseconds

    // Play the beep sound
    Beep(frequency, duration);
}

void timings() // To retrieve current time
{
    time_t now; // declaration of time variable
    struct tm *ltime;
    time(&now);
    ltime = localtime(&now);
    strftime(buffer, sizeof(buffer), "%H:%M:%S", ltime);
}

DWORD WINAPI alarmtracking(LPVOID lpParam) // To ring the alarm
{
    int ch, cm, cs;
    while (1)
    {
        timings();
        sscanf(buffer, "%d:%d:%d", &ch, &cm, &cs);
        if (ch == hours && cm == minutes && cs == seconds)
        {
            printf("\nALARM TIME!\n");
            beeping();
            ExitThread(0); // Terminate the thread after the alarm rings
        }
        Sleep(1000);
    }
    return 0;
}

void setalarm() // for setting the alarm
{
    int currenthours, currentminutes, currentseconds;
    printf("\nEnter the hours, minutes, and seconds\n");
    scanf("%d %d %d", &hours, &minutes, &seconds);
    timings();
    // Parse the time from buffer
    sscanf(buffer, "%d:%d:%d", &currenthours, &currentminutes, &currentseconds);
    if (hours < currenthours ||
        (hours == currenthours && minutes < currentminutes) ||
        (hours == currenthours && minutes == currentminutes && seconds < currentseconds))
    {
        printf("\nCannot set the alarm as the time has already passed");
        exit(0);
    }
    else
    {
        printf("\nAlarm set\n");
        beeping();
        hThread = CreateThread(NULL, 0, alarmtracking, NULL, 0, NULL); // Start a new thread for alarm tracking
        if (hThread == NULL)
        {
            printf("Error creating thread\n");
            exit(1);
        }
    }
}

void digitalclock() // To show timings like a digital clock
{
    while (1)
    {
        // Get current time
        timings();
        printf("\nTo set an alarm clock press escape button\n");
        printf("\n\n\t\t%s", buffer); // Printing the current time
        Sleep(1000);
        CLEAR_SCREEN();
        if (kbhit()) // when the user presses a key
        {
            printf("\nSetting the alarm\n");
            setalarm();
        }
    }
}

int main()
{
    digitalclock();                         // To call the digital clock
    WaitForSingleObject(hThread, INFINITE); // Wait for the alarm thread to finish (optional)
    return 0;
}
