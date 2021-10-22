#include <Time.h>
#include "SoftwareSerial.h"

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

int gRxPin = 10;
int gTxPin = 11;

SoftwareSerial BTSerial(gRxPin, gTxPin);

void setup()
{
    pinMode(gRxPin, INPUT);
    pinMode(gTxPin, OUTPUT);
    BTSerial.begin(38400);
    Serial.begin(9600);
    delay(500);
}

// (0)type:R,G,B:px,px,px,px
// (1)type:R,G,B:text
String inp = "";
void loop()
{
    while (BTSerial.available())
    {
        char temp = BTSerial.read();
        if (temp != '\n')
            inp += temp;
    }
    if (inp != "")
    {
        String t = getValue(inp, ':', 0);
        String c = getValue(inp, ':', 1);
        String d = getValue(inp, ':', 2);

        int countCommaD = 0;
        for (int i = 0; i < d.length(); i++)
            countCommaD += (d[i] == ',');

        Serial.print("TYPE: ");
        Serial.println(t);
        Serial.print("COLOR: ");
        Serial.print(getValue(c, ',', 0));
        Serial.print(" ");
        Serial.print(getValue(c, ',', 1));
        Serial.print(" ");
        Serial.println(getValue(c, ',', 2));
        Serial.print("DATA: ");
        for (int i = 0; i <= countCommaD; i++)
        {
            Serial.print(getValue(d, ',', i));
            Serial.print(" ");
        }
        Serial.println("");
    }
    inp = "";
}