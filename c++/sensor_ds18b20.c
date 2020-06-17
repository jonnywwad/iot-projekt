#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>


//groesstenteils kopiert: http://bradsrpi.blogspot.com/2013/12/c-program-to-read-temperature-from-1.html

void read_ds18b20(int *p_temperature);
float readfloat_ds18b20();



void read_ds18b20(int *p_temperature){
    DIR *dir;
    struct dirent *dirent;
    char dev[16];      // Dev ID
    char devPath[128]; // Path to device
    char buf[256];     // Data from device
    char tmpData[6];   // Temp C * 1000 reported by device 
    char path[] = "/sys/bus/w1/devices"; 
    ssize_t numRead;

    dir = opendir (path);
    if (dir != NULL)
    {
        while ((dirent = readdir (dir)))
            // 1-wire devices are links beginning with 28-
            if (dirent->d_type == DT_LNK && 
                    strstr(dirent->d_name, "28-") != NULL) { 
                strcpy(dev, dirent->d_name);
                //printf("Sensor-ds18b20: Device: %s\n", dev);
            }
        (void) closedir (dir);
    }
    else
    {
        perror ("Sensor-ds18b20: Couldn't open the w1 devices directory");
    }

    // Assemble path to OneWire device
    sprintf(devPath, "%s/%s/w1_slave", path, dev);
    // Read temp continuously
    // Opening the device's file triggers new reading
    float tempC = 999999;
    int fd = open(devPath, O_RDONLY);
    if(fd == -1)
    {
        perror ("Sensor-ds18b20: Couldn't open the w1 device.");
    }
    while((numRead = read(fd, buf, 256)) > 0) 
    {
        strncpy(tmpData, strstr(buf, "t=") + 2, 5); 
        //float tempC = strtof(tmpData, NULL);
        tempC = strtof(tmpData, NULL);
    }
    close(fd);

    //FIXME: manchmal gemessener wert um faktor 10 zu gross???
    if(tempC > 35000){
        printf("\nWERT ZU GROSS GEMESSEN?! => teilen %f\n\n\n",tempC);
        tempC=tempC/10;
    }









    //Temperaturwert in ganzzahl und kommaanteil zerlegen und in int array speichern
    int ganzzahl = (int)(tempC/1000);
    int kommazahl = (int)(((int)tempC % 1000)/10); //note: nochmal durch 10 teilen, damit nur 2 kommastellen erhalten werden
    //printf("Sensor-ds18b20: --DEBUG: ganzzahl: %d, kommazahl: %d\n",ganzzahl, kommazahl);
    p_temperature[0] = ganzzahl;     
    p_temperature[1] = kommazahl;   





    printf("DEBUG: Sensor-ds18b20 Temp: %.3f C  Ganzzahl: %d Kommazahl: %d \n", tempC / 1000, p_temperature[0], p_temperature[1]);
    //printf("Device: %s  - ", dev); 
    //printf("Temp: %.3f C  ", tempC / 1000);

    }






float readfloat_ds18b20(){

    DIR *dir;
    struct dirent *dirent;
    char dev[16];      // Dev ID
    char devPath[128]; // Path to device
    char buf[256];     // Data from device
    char tmpData[6];   // Temp C * 1000 reported by device 
    char path[] = "/sys/bus/w1/devices"; 
    ssize_t numRead;

    dir = opendir (path);
    if (dir != NULL)
    {
        while ((dirent = readdir (dir)))
            // 1-wire devices are links beginning with 28-
            if (dirent->d_type == DT_LNK && 
                    strstr(dirent->d_name, "28-") != NULL) { 
                strcpy(dev, dirent->d_name);
                //printf("Sensor-ds18b20: Device: %s\n", dev);
            }
        (void) closedir (dir);
    }
    else
    {
        perror ("Sensor-ds18b20: Couldn't open the w1 devices directory");
    }

    // Assemble path to OneWire device
    sprintf(devPath, "%s/%s/w1_slave", path, dev);
    // Read temp continuously
    // Opening the device's file triggers new reading
    float tempC = 999999;
    int fd = open(devPath, O_RDONLY);
    if(fd == -1)
    {
        perror ("Sensor-ds18b20: Couldn't open the w1 device.");
    }
    while((numRead = read(fd, buf, 256)) > 0) 
    {
        strncpy(tmpData, strstr(buf, "t=") + 2, 5); 
        //float tempC = strtof(tmpData, NULL);
        tempC = strtof(tmpData, NULL);
    }
    close(fd);

    //FIXME: manchmal gemessener wert um faktor 10 zu gross???
    float value;
    if(tempC > 35000){
        //printf("\nWERT ZU GROSS GEMESSEN?! => teilen %f\n\n\n",tempC);
        printf("\nWERT ZU GROSS GEMESSEN?! => errorcode -1 %fn\n",tempC);
        //tempC=tempC/10;
         value =-1;
    }else{
         value = tempC/1000;
    }





    //printf("DEBUG: Sensor-ds18b20 Temp: %.3f C", value);

return value;


    }
