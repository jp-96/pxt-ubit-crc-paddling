#ifndef MICROBIT_CSC_SERVICE_H
#define MICROBIT_CSC_SERVICE_H

#include "pxt.h"

// UUIDs for our service and characteristics
extern const uint16_t  CSCServiceUUID;
extern const uint16_t  CSCMeasurementCharacteristicUUID;
extern const uint16_t  CSCFeatureCharacteristicUUID;
extern const uint16_t  SensorLocationCharacteristicUUID;
extern const uint16_t  SCControlPointCharacteristicUUID;

// consts
extern const uint8_t  CSCMeasurementFlags;
extern const uint16_t CSCFeature;

//================================================================
#if MICROBIT_CODAL
//================================================================

#include "MicroBitBLEManager.h"
#include "MicroBitBLEService.h"

class MicroBitCSCServiceDal //: public MicroBitBLEService
{
    public:

    /**
      * Constructor.
      * @param _ble The instance of a BLE device that we're running on.
      */
    MicroBitCSCServiceDal(BLEDevice &_ble);

    /**
      * Notify CSC measurement.
      * 
      */
    void notifySCMeasurement(uint32_t cumulativeWheelRevolutions, uint16_t lastWheelEventTime, uint16_t cumulativeCrankRevolutions, uint16_t lastCrankEventTime);

    private:

    // Bluetooth stack we're running on.
    BLEDevice &ble;

};

//================================================================
#else // MICROBIT_CODAL
//================================================================

#include "ble/BLE.h"

class MicroBitCSCServiceDal
{
    public:

    /**
      * Constructor.
      * @param _ble The instance of a BLE device that we're running on.
      */
    MicroBitCSCServiceDal(BLEDevice &_ble);

    /**
      * Notify CSC measurement.
      * 
      */
    void notifySCMeasurement(uint32_t cumulativeWheelRevolutions, uint16_t lastWheelEventTime, uint16_t cumulativeCrankRevolutions, uint16_t lastCrankEventTime);

    private:

    // Bluetooth stack we're running on.
    BLEDevice &ble;

    // memory for buffers.
    uint8_t cscMeasurementCharacteristicBuffer[5];
    uint8_t cscFeatureCharacteristicBuffer[2];

    // Handles to access each characteristic when they are held by Soft Device.
    GattAttribute::Handle_t cscMeasurementCharacteristicHandle;
    GattAttribute::Handle_t cscFeatureCharacteristicHandle;
    
};

//================================================================
#endif // MICROBIT_CODAL
//================================================================

/**
  * Status flags
  */
// Universal flags used as part of the status field
// #define MICROBIT_COMPONENT_RUNNING 0x01
#define MICROBIT_CSC_SERVICE_ADDED_TO_IDLE 0x02

class MicroBitCSCService : public MicroBitComponent
{
    
    public:

    /**
      * Constructor.
      * Create a representation of the CSC Service
      * 
      */
    MicroBitCSCService();

    /**
      * Periodic callback from MicroBit idle thread.
      * 
      */
    virtual void idleTick();

    /**
     * Count up crank revolutions.
     * @param timestamp The time on crank sensor in microseconds.
     */
    void countUpCrankRevolutions(uint64_t timestamp);

private:

    /**
     * Update to notify CSC mesurement
     * 
     */
    void update();

    /**
     * Get event time (1/1024s)
     * The Last Wheel Event Time value rolls over every 64 seconds
     * The Last Crank Event Time value rolls over every 64 seconds.
     * @param timestamp The time in microseconds.
     * 
     **/
    uint16_t getEventTimeRollOverEvery64Seconds(uint64_t timestamp);

    // instance
    MicroBitCSCServiceDal* pCSCServiceDal;

    // next sample timestamp scheduled
    uint64_t updateSampleTimestamp;
    static const uint64_t SENSOR_UPDATE_PERIOD_US = 1000000; // 1s

    // CSC measurements
    uint16_t cumulativeCrankRevolutions;
    uint16_t lastCrankEventTime;

};

#endif
