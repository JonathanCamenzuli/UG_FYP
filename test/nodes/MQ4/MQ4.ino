/**
 * @file MQ4.ino
 *
 * @author Jonathan Camenzuli
 *
 * @brief MQ4 Sensor Test
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 10/05/2023
 *
 * @section hardware Hardware
 * -  Arduino MKR NB 1500
 * -  MQ4 Gas Sensor
 */

// Include the library
#include <MQUnifiedsensor.h>

// Definitions
#define board "Arduino UNO"
#define Voltage_Resolution 3.3
#define pin A1                // Analog input 0 of your arduino
#define type "MQ-4"           // MQ4
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ4CleanAir 4.4  // RS / R0 = 60 ppm
// #define calibration_button 13 //Pin to calibrate your sensor

// Declare Sensor
MQUnifiedsensor MQ4(board, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

void setup()
{
    // Init the serial port communication - to debug the library
    Serial.begin(9600); // Init serial port

    // Waiting for Serial port to Connect
    while (!Serial)
        ;

    // Set math model to calculate the PPM concentration and the value of constants
    // delay(2500);
    MQ4.setRegressionMethod(0); //_PPM =  a*ratio^b

    /*****************************  MQ Init ********************************************/
    // Remarks: Configure the pin of arduino as input.
    /************************************************************************************/
    MQ4.init();
    MQ4.setRL(1000);

    /*
      //If the RL value is different from 10K please assign your RL value with the following method:
      MQ4.setRL(37);
    */
    /*****************************  MQ CAlibration ********************************************/
    // Explanation:
    // In this routine the sensor will measure the resistance of the sensor supposedly before being pre-heated
    // and on clean air (Calibration conditions), setting up R0 value.
    // We recomend executing this routine only on setup in laboratory conditions.
    // This routine does not need to be executed on each restart, you can load your R0 value from eeprom.
    // Acknowledgements: https://jayconsystems.com/blog/understanding-a-gas-sensor
    Serial.print("Calibrating please wait.");
    float calcR0 = 0;
    for (int i = 1; i <= 10; i++)
    {
        MQ4.update(); // Update data, the arduino will read the voltage from the analog pin
        calcR0 += MQ4.calibrate(RatioMQ4CleanAir);
        Serial.print(".");
    }
    Serial.print("calcR0:");
    Serial.println(calcR0);
    MQ4.setR0(calcR0 / 10);
    Serial.println("  done!.");

    if (isinf(calcR0))
    {
        Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");
        while (1)
            ;
    }
    if (calcR0 == 0)
    {
        Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");
        while (1)
            ;
    }
    /*****************************  MQ CAlibration ********************************************/

    Serial.println("*************** Values from MQ-4 **********************");
    Serial.println("|   CO    |   Smoke    |");
}

void loop()
{
    MQ4.update(); // Update data, the arduino will read the voltage from the analog pin

    /*
      Exponential regression:
    Gas    | a      | b
    LPG    | 3811.9 | -3.113
    CH4    | 1012.7 | -2.786
    CO     | 200000000000000 | -19.05
    Alcohol| 60000000000 | -14.01
    smoke  | 30000000 | -8.308
    */
    // MQ4.setA(3811.9);
    // MQ4.setB(-3.113);             // Configure the equation to to calculate CH4 concentration
    // float LPG = MQ4.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

    // MQ4.setA(1012.7);
    // MQ4.setB(-2.786);             // Configure the equation to to calculate CH4 concentration
    // float CH4 = MQ4.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

    MQ4.setA(-0.05849699);
    MQ4.setB(0.75427267);        // Configure the equation to to calculate CH4 concentration
    float CO = MQ4.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

    // MQ4.setA(60000000000);
    // MQ4.setB(-14.01);                 // Configure the equation to to calculate CH4 concentration
    // float Alcohol = MQ4.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

    MQ4.setA(-0.036579755);
    MQ4.setB(0.6076452);            // Configure the equation to to calculate CH4 concentration
    float Smoke = MQ4.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

    Serial.print("|    ");
    // Serial.print(LPG);
    // Serial.print("    |    ");
    // Serial.print(CH4);
    // Serial.print("    |    ");
    Serial.print(CO);
    Serial.print("    |    ");
    // Serial.print(Alcohol);
    // Serial.print("    |    ");
    Serial.print(Smoke);
    Serial.println("    |");

    delay(500); // Sampling frequency
}