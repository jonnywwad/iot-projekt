#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include <unistd.h>
#include "mqtt.h"
#include <stdbool.h>

#define CLIENTID "IOT-CPP-MQTT-Client"
#define QOS 1
#define TIMEOUT 10000L


//globale variablen (extern)
MQTTClient mqtt_client;
int rc; //(mqtt status code)




//Funktion zum verbinden mit MQTT Broker definieren
void mqtt_connect(const char *address){
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    MQTTClient_create(&mqtt_client, address, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    //int rc; //oben schon  global deklariert
    if ((rc = MQTTClient_connect(mqtt_client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("======ERR: MQTT-Client:Failed to connect, return code: %d=======\n", rc);
    }else{
        printf("==MQTT-Client: connected to Broker!==\n\n");
    }
}





//Funktion zum prüfen der MQTT Verbindung definieren
bool mqtt_checkconnection(const char *address){
    if(rc != 0){ //wenn status nicht MQTTCLIENT_SUCCESS vgl /usr/local/include/MQTTClient.h
        printf("==========ERR: CONNECTION LOST... code: %d, retrying...=========\n",rc);
        mqtt_connect(address);
        return false;
    }else{
        return true;
    }
}











//Funktion zum publishen eines 0 terminierten C-Strings definieren
void mqtt_publishString(const char *p_value, const char *p_topic){
    int tries=0;

    //Pruefen ob Verbindung zum Broker besteht und evtl. wiederherstellen:
    while (rc != 0 && tries < 10){ //mqtt nicht verbunden und noch weniger als 10 versuche durgefuehrt
        printf("==========ERR: CONNECTION NOT ESTABLISHED... code: %d=========\n",rc);
        mqtt_disconnect();
        usleep(1*1000000); //1s warten
        printf("retrying... try: %d\n",tries);
        mqtt_connect("tcp://localhost:1883");
        tries++;
    }

    if (rc == 0){    //Verbindung besteht
        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        MQTTClient_deliveryToken token;
        pubmsg.qos = QOS;
        //int rc; //jetzt global deklariert

        //const char array in normales char array kopieren:
        char stringwert_noconst[strlen(p_value)];
        sprintf(stringwert_noconst, "%s", p_value);
        //printf("--DEBUG: converted const string: %s to noconst: %s\n", p_value, stringwert_noconst);

        //Objekt zum publishen erstellen:
        pubmsg.payload = stringwert_noconst;
        pubmsg.payloadlen = (int)strlen(p_value);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        //publish message:
        MQTTClient_publishMessage(mqtt_client, p_topic, &pubmsg, &token);
        rc = MQTTClient_waitForCompletion(mqtt_client, token, TIMEOUT);
        printf("==MQTT-Client: Message delivered - Value: %s, QOS: %d, Topic: %s, Token: %d==\n", pubmsg.payload, pubmsg.qos, p_topic, token);
    }else{
        printf("ERR: es konnte keine verbindung mit dem broker hergestellt werden...\n es werden keine daten gesendet!\n");
    }
}



//Funktion zum abmelden vom mqtt broker (verbindung schliessen) definieren:
void mqtt_disconnect(void){
        MQTTClient_disconnect(mqtt_client, 10000);
        MQTTClient_destroy(&mqtt_client);
        rc=-3;
    printf("\n\n==MQTT-Client disconnected!==\n");
}













//=================DEL / unused=================
//Funktion zum publishen einer integerzahl definieren
void mqtt_publishint(int p_value, char *p_topic){
    //printf("--DEBUG: Publishing Value: %d, Topic: %s--\n", p_value, p_topic);
        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        MQTTClient_deliveryToken token;
        pubmsg.qos = QOS;
        //int rc; jetzt global
        //integer in c string umwandeln
        char stringwert[8];
        sprintf(stringwert, "wert:%d", p_value);
        printf("converted to string: %s\n", stringwert);
        pubmsg.payload = stringwert;
        pubmsg.payloadlen = (int)strlen(stringwert);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;
        MQTTClient_publishMessage(mqtt_client, p_topic, &pubmsg, &token);
        //printf("Waiting for up to %d seconds for publication of %s\n"
                //"on topic %s for mqtt_client with ClientID: %s\n",
                //(int)(TIMEOUT/1000), PAYLOAD, p_topic, CLIENTID);
        rc = MQTTClient_waitForCompletion(mqtt_client, token, TIMEOUT);
        //printf("Message with delivery token %d delivered\n", token);
        printf("==MQTT-Client: Message delivered - Value: %s, Topic: %s, Token: %d==\n", pubmsg, p_topic, token);
    }
//=================DEL / unused=================
