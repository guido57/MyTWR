/**
 * @file      SA868_ATDebug_Example.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-01-05
 *
 */
 #define ARDUHAL_LOG_LEVEL 4


 #include "LilyGo_TWR.h"




bool waitResponse(String &data, String rsp, uint32_t timeout)
{
    uint32_t startMillis = millis();
    do {
        while (Serial2.available() > 0) {
            int8_t ch = Serial2.read();
            printf("%c", ch);
            data += static_cast<char>(ch);
            if (rsp.length() && data.endsWith(rsp)) {
                return true;
            }
        }
    } while (millis() - startMillis < 1000);
    return false;
}

bool waitResponse(String rsp, uint32_t timeout = 1000)
{
    String data;
    return waitResponse(data, rsp, timeout);
}

bool isConnected()
{
    Serial2.println("AT+DMOCONNECT");
    if (waitResponse("+DMOCONNECT:0\r\n") == 1) {
        return true;
    }
    // delay(100);
    // Serial2.println("AT+DMOSETVOLUME=3");
    // if (waitResponse("SA868S-") == 1) {
    //     String model = Serial2.readString();
    //     if (model.startsWith("UHF") ||
    //             model.startsWith("VHF")) {
    //         return true;
    //     }
    //     return true;
    // }
    // return false;
}


void setup()
{
    bool rlst = false;

    Serial.begin(115200);
    delay(5000);
    //* Initializing PMU is related to other peripherals
    //* Automatically detect the revision version through GPIO detection.
    //* If GPIO2 has been externally connected to other devices, the automatic detection may not work properly.
    //* Please initialize with the version specified below.
    //* Rev2.1 is not affected by GPIO2 because GPIO2 is not exposed in Rev2.1
    // twr.begin();
    //RadioSerial.begin(9600, SERIAL_8N1, SA868_RX_PIN, SA868_TX_PIN);
    Serial2.begin(9600, SERIAL_8N1, SA868_RX_PIN, SA868_TX_PIN);

    // Set SA8x8 PowerDown to HIGH
    pinMode(SA868_PD_PIN, OUTPUT);
    digitalWrite(SA868_PD_PIN, HIGH);
    pinMode(SA868_SQL, INPUT);

    //* If GPIO2 is already connected to other devices, please initialize it with the specified version.
    // twr.begin(LILYGO_TWR_REV2_0);

    // if (twr.getVersion() == TWRClass::TWR_REV2V1) {
    //     Serial.println("Detection using TWR Rev2.1");

        // Initialize SA868
        //radio.setPins(SA868_PTT_PIN, SA868_PD_PIN);
        //rlst = radio.begin(RadioSerial, twr.getBandDefinition());
        // rlst = radio.begin(RadioSerial, SA8X8_VHF);

        // radio.serial = &Serial2;

    pinMode(SA868_PTT_PIN, OUTPUT);
    digitalWrite(SA868_PTT_PIN, HIGH);

    pinMode(SA868_PD_PIN, OUTPUT);
    digitalWrite(SA868_PD_PIN, LOW);
    
    pinMode(SA868_PD_PIN, OUTPUT);
    digitalWrite(SA868_PD_PIN, HIGH);

    delay(1000);

    int retry = 5;
    while (retry--) {
        if (isConnected()) {
            break;
        }
        delay(100);
    }

    if (!isConnected()) {
        while (1) {
            Serial.println("SA868 is not online !");
            delay(1000);
        }    
    }

    Serial.println("SA868 is online !");

    Serial2.println("AT+DMOSETVOLUME=3");
    if (waitResponse("SA868S-") == 1) {
        String model = Serial2.readString();
    }
  
    // } else {

    //     Serial.println("Detection using TWR Rev2.0");

    //     //* Rev2.0 cannot determine whether it is UHF or VHF, and can only specify the initialization BAND.

    //     //* Designated as UHF
    //     //  rlst = radio.begin(RadioSerial, SA8X8_UHF);

    //     //* Designated as VHF
    //     radio.setPins(SA868_PTT_PIN, SA868_PD_PIN, SA868_RF_PIN);
    //     rlst = radio.begin(RadioSerial, SA8X8_VHF);

    // }

    // if (!rlst) {
    //     while (1) {
    //         Serial.println("SA868 is not online !");
    //         delay(1000);
    //     }
    // }
    // Serial.println("SA868 is online !");
    

}

void loop()
{
    while (RadioSerial.available()) {
        Serial.write(RadioSerial.read());
    }
    while (Serial.available()) {
        RadioSerial.write(Serial.read());
    }
    delay(1);
}


