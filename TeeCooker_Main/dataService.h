#ifndef DATASERVICE_H
#define DATASERVICE_H
#include <Arduino.h>

class Tea {
  public:
    char* m_name;
    double m_brewingTime; // in min
    double m_waterTemp; // in Gradcelsius

    Tea(char* name, double brewingTime, double waterTemp): m_name(name), m_brewingTime(brewingTime), m_waterTemp(waterTemp){}
};

Tea* getTea(int tea);

#endif
