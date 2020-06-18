#include <string>
#include <iostream>
#include <ctime>
#include <unistd.h>

#include "sensor_sample1.cpp"
#include "sensor_sample2.cpp"
#include "sensor_ds18b20.c" //FIXME: C code mit g++ compiler sogar moeglich???

//eigene mit gcc kompilierte C Funktionen hier in c++ zur Verfuegung stellen
extern "C" {
#include "mqtt.h"
}




//============KONFIGURATION DER VORHANDENEN SENSOREN============

//MQTT Broker Adresse konfigurieren:
const char *mqtt_broker_addr = "tcp://localhost:1883";


//typdefinition/struct fuer sensoreintrag deklarieren                                          
typedef struct sensoreintrag 
{                                                                               
    const char *name;                                                        
    const char *mqtt_topic;                                                  
    uint32_t timestamp;                                                                                            
    float value;                                                                
    float (*read_SensorFunktion)();    //pointer auf funktion                          
} sensoreintrag;



//hier alle vorhandenen sensoren definieren <========= hier individuell die gerade angeschlossenen Sensoren mit deren funktion zum auslesen hinzufügen!!! (rest bleibt auf jedem sensor-pi gleich)

int sensoranzahl = 3;
struct sensoreintrag sensoren[] = {
	{
      name: "Temperatursensor-Zimmer",
      mqtt_topic: "sensordaten/zimmer1/temperatur",      //MQTT topic (daten werden dort hingesendet)
      timestamp: 0,   //derzeit keine funktion
      value: 0,       //startwert optional
      //Funktion mit der der sensor ausgelesen wird (rueckgabewert float, keine parameter)
      //read_SensorFunktion:   readfloat_ds18b20  //derzeit kein temperatursensor angeschlossen => stattdessen testweise beispielsensor aufrufen
      read_SensorFunktion:   readfloat_samplesensor1       
	      //!!!dementsprechende datei in der diese funktion definiert ist (von der gruppe) muss natuerlich oben included werden vgl samplesensor!!!
	},
	{
      name: "Beispielsensor1",
      mqtt_topic: "sensordaten/testing/beispielsensor1",
      timestamp: 0,
      value: 0,
      read_SensorFunktion:  readfloat_samplesensor1
	},	{
      name: "Beispielsensor",
      mqtt_topic: "sensordaten/testing/beispielsensor2",
      timestamp: 0,
      value: 0,
      read_SensorFunktion:  readfloat_samplesensor2
	},
};






//==================DEFINITION VON BENOETIGTEN FUNKTIONEN===================

//Funktion zum umwandeln eines einer Float-Zahl zu JSON-String mit Timestamp
//Quelle: Herr Koller (abgeaendert)

std::string Float2JSON(float value) 
    //const char* Val2JSON(int *tmp) 
{
    std::string val;
    time_t result = time(nullptr);

    val  = "{";
    val += "\"time\":";
    val += std::to_string(result);//timestamp in string umwandeln
    val += ",";
    val += "\"value\":";
    val += std::to_string(value); //float wert in string umwandeln
    val += "}";
    //printf("DEBUG: float value: %f JSON-String: %s\n", value, val.c_str());
    return val;
}






//=====================MAIN FUNKTION (wird beim programmstart ausgefuehrt)=====================
int main(int argc, char* argv[]){

    //MQTT: verbindung mit broker aufbauen
    //mqtt_connect("tcp://localhost:1883");
    mqtt_connect(mqtt_broker_addr);
    //TODO: fuer adresse evtl makro oder globale variable in der config anlegen




    //dauerhaft definierte sensoren auslesen und den wert mit timestamp as json format an definiertes MQTT-Topic senden
    for (; ;){

        printf("\n\n===Sensorauswertung startet...===\n");
        for(int i = 0;i<sensoranzahl;i++){

            printf("Starting Sensor: %s\n",sensoren[i].name);

            //Sensor mit jeweilger konfigurierter funktion auslesen
            sensoren[i].value = sensoren[i].read_SensorFunktion();

            if(sensoren[i].value != -1){ //pruefen ob gueltigen wert erhalten (errorcode -1)
                //Sensorwert (float) in JSON-STRING umwandeln
                std::string json_string = Float2JSON(sensoren[i].value);

                //C++ string in C string umwandeln (pointer auf char array) da MQTT funktion als C compiled wird
                const char *json_CharArray = json_string.c_str(); 
                //DEBUG: printf("\n\nChararray pointer adresse: %d \n Chararray string: %s \n\n",json_CharArray, json_CharArray);

                //MQTT: json string publishen
                mqtt_publishString(json_CharArray, sensoren[i].mqtt_topic);
                printf("\n");
            }else{
                printf("keinen gueltigen Wert beim auslesen erhalten!!\n");
            }
        }

        printf("done - waiting...\n");
        usleep(10*1000000); //10s warten
    }
    //FIXME: mqtt verliert abunzu verbindung? - warum (soecket error, siehe /var/log/mosquitto/...)? (neu verbinden ist bereits in mqtt.c implementiert)



    mqtt_disconnect();
    return 0;
}
