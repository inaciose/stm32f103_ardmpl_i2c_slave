
#include <Arduino.h>
#include <Wire_slave.h>

#define I2C_SLAVE_ADDR 0x08

#ifndef LED_BUILTIN
  #define LED_BUILTIN PC13
#endif

#define LED_PIN LED_BUILTIN
bool ledon = false;

// handle serial numeric data
union u_tag {
    char b[4];
    int i;
    float f;
} i2c_data[8];

// debug
volatile int receiveEventCount = 0;
volatile int requestEventCount = 0;

int intenger_type = 65500;
float float_type = 100.0;

void requestEvent()
{
    Wire.write("hello ");
    Wire.write(90); 
    Wire.write("b");
    Wire.write((byte*)&intenger_type, 4);
    Wire.write((byte*)&float_type, 4);

    //Onrequest tmp = { .c = {'h', 'e', 'l', 'l', 'o', ' '}, .b=90, .d = 'b', .i = intenger_type, .f=float_type };
}

void receiveEvent(int howMany){

    char i2c_byte[32];
    int i = 0;

    // init i2c receive buffer
    for(i=0; i < 32; i++) {
        i2c_byte[i] = 0;
    }

    // receive data
    i = 0;
    while (Wire.available()) {
        char c = Wire.read(); 
        i2c_byte[i] = c; 
        i++;
    }

    // store receive data to use later
        for(i=0; i < 8; i++) {
        i2c_data[i].b[0] = i2c_byte[i*4 + 0];
        i2c_data[i].b[1] = i2c_byte[i*4 + 1];
        i2c_data[i].b[2] = i2c_byte[i*4 + 2];
        i2c_data[i].b[3] = i2c_byte[i*4 + 3];
    }

    // debug
    receiveEventCount++;
}

void setup()
{

    pinMode(LED_PIN, OUTPUT);

    Serial.begin(115200);
    Serial.println("Serial.begin done");

    Wire.begin(I2C_SLAVE_ADDR);
    Serial.println("Wire.begin done");

    // set callbacks
    Wire.onRequest(requestEvent);
    Wire.onReceive(receiveEvent);
    Serial.println("Set callbacks done");

}

void loop()
{

    Serial.print((byte)i2c_data[0].b[0]); Serial.print("\t");
    Serial.print((byte)i2c_data[0].b[1]); Serial.print("\t");
    Serial.print((byte)i2c_data[0].b[2]); Serial.print("\t");
    Serial.print((char)i2c_data[0].b[3]); Serial.print("\t");
    
    Serial.print((byte)i2c_data[1].b[0]); Serial.print("\t");
    Serial.print((byte)i2c_data[1].b[1]); Serial.print("\t");
    Serial.print((byte)i2c_data[1].b[2]); Serial.print("\t");
    Serial.print((byte)i2c_data[1].b[3]); Serial.print("\t");

    Serial.print((int)i2c_data[1].i); Serial.print("\t");

    Serial.print((float)i2c_data[2].f); Serial.print("\t");

    Serial.print(receiveEventCount); Serial.print("\t");
    Serial.print(requestEventCount); Serial.print("\t");

    Serial.println("");

    intenger_type++;
    float_type += 0.2;

    delay(500);

#ifdef LED_PIN
    // led blink control
    static uint32_t ledtimer = millis();
    if(millis() > ledtimer) {
    ledtimer = millis()+1000;
        if(ledon) {
            digitalWrite(LED_PIN, LOW);
            ledon = false;
        } else {
            digitalWrite(LED_PIN, HIGH);
            ledon = true;  
        }
    }
#endif

}
