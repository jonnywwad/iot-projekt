//#include"sensor_sample.h"
#include <ctime>

//deklaration testfunktion
void readintarr_samplesensor1(int *tmp);


//deklaration testfunktion
float readfloat_samplesensor1();



//definition testfunktion
void readintarr_samplesensor1(int *tmp){
    //letzte beiden zahlen vom timestamp in das integer array schreiben
    time_t timestamp = time(nullptr);
    std::string s_timestamp = std::to_string(timestamp);
    const char *char_timestamp = s_timestamp.c_str();
    tmp[0] = char_timestamp[9];
    tmp[1] = 11;
    //asd
}


//definition testfunktion
float readfloat_samplesensor1(){
    //zufallszahl von 20-30 erstellen
    return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*10+20;
}
