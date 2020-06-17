#pragma once
#include "MQTTClient.h"
#include <stdbool.h>

//Deklaration aller selbst erstellten mqtt Funktionen:
void mqtt_connect(const char *address);

void mqtt_disconnect(void);

void mqtt_publishint(int input, char *topic);
void mqtt_publishString(const char *input, const char *topic);


bool mqtt_checkconnection(const char *address);




//globale variable mqtt client anlegen
extern MQTTClient mqtt_client;
extern int rc; //mqtt status code
