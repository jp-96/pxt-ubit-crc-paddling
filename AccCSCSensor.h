#ifndef ACC_CSC_SENSOR_H
#define ACC_CSC_SENSOR_H

#include "pxt.h"

#define CUSTOM_ID_ACC_CSC_SENSOR  2049
#define CUSTOM_ACC_CSC_SENSOR_EVT_CADENCE 0x01

#define ACC_CSC_SENSOR_PITCHING_SIZE 5
#define ACC_CSC_SENSOR_PITCHING_SENSITIVITY 100

class AccCSCSensor
{
    public:

//================================================================
#if MICROBIT_CODAL
//================================================================

    /**
      * Constructor.
      * Create a representation of the CSC Controller
      * 
      */
    AccCSCSensor(codal::Accelerometer  &_accelerometer);

//================================================================
#else // MICROBIT_CODAL
//================================================================

    /**
      * Constructor.
      * Create a representation of the CSC Controller
      * 
      */
    AccCSCSensor(MicroBitAccelerometer &_accelerometer);

//================================================================
#endif // MICROBIT_CODAL
//================================================================

    /**
     * Get last time(us) on cadence.
     * 
     */
    uint64_t getCadenceTimeUs();

  private:
    
    /**
     * Accelerometer update callback
     */
    void accelerometerUpdate(MicroBitEvent e);

//================================================================
#if MICROBIT_CODAL
//================================================================

    // Accelerometer
    codal::Accelerometer	&accelerometer;

//================================================================
#else // MICROBIT_CODAL
//================================================================

    // Accelerometer
    MicroBitAccelerometer	&accelerometer;

//================================================================
#endif // MICROBIT_CODAL
//================================================================

    // pitching data for cadence
    int pitching[ACC_CSC_SENSOR_PITCHING_SIZE];
    int pitchingSize;
    int pitchingNextIdx;
    int pitchingMax;
    int pitchingMin;
    int pitchingMeanLast;
    int pitchingLast;
    int pitchingSensitivity;
    
    // Last time on cadence
    uint64_t cadenceTimeUs;

    // micro:bit event CUSTOM_ID_ACC_CSC_SENSOR, CUSTOM_ACC_CSC_SENSOR_EVT_CADENCE
    MicroBitEvent *_evtCadence;
};

#endif // #ifndef ACC_CSC_SENSOR_H