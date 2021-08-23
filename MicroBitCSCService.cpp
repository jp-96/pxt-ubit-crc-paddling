#include "pxt.h"
#include "struct/include/struct/struct.h"
#include "MicroBitCSCService.h"

//================================================================
#if MICROBIT_CODAL
//================================================================

MicroBitCSCServiceDal::MicroBitCSCServiceDal(BLEDevice &_ble) :
        ble(_ble)
{
    //
}
void MicroBitCSCServiceDal::notifySCMeasurement(uint32_t cumulativeWheelRevolutions, uint16_t lastWheelEventTime, uint16_t cumulativeCrankRevolutions, uint16_t lastCrankEventTime)
{
    //
}

//================================================================
#else // MICROBIT_CODAL
//================================================================

#include "ble/UUID.h"

MicroBitCSCServiceDal::MicroBitCSCServiceDal(BLEDevice &_ble) :
        ble(_ble)
{
    // Create the data structures that represent each of our characteristics in Soft Device.
    GattCharacteristic  cscMeasurementCharacteristic(CSCMeasurementCharacteristicUUID, (uint8_t *)&cscMeasurementCharacteristicBuffer, 0,
                        sizeof(cscMeasurementCharacteristicBuffer), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);
    GattCharacteristic  cscFeatureCharacteristic(CSCFeatureCharacteristicUUID, (uint8_t *)&cscFeatureCharacteristicBuffer, 0,
                        sizeof(cscFeatureCharacteristicBuffer), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);
    
    // Initialise our characteristic values.
    memset(&cscMeasurementCharacteristicBuffer, 0, sizeof(cscMeasurementCharacteristicBuffer));
    struct_pack(cscFeatureCharacteristicBuffer, "<H", CSCFeature);
    
    // Set default security requirements
    cscMeasurementCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    cscFeatureCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    
    // setup GATT table
    GattCharacteristic *characteristics[] = {
        &cscMeasurementCharacteristic,
        &cscFeatureCharacteristic,
    };
    GattService service(CSCServiceUUID, characteristics, sizeof(characteristics) / sizeof(GattCharacteristic *));
    ble.addService(service);

    // service advertising data
    uint8_t serviceIDs[2];
    struct_pack(serviceIDs, "<H", CSCServiceUUID);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, serviceIDs, sizeof(serviceIDs));
    ble.accumulateAdvertisingPayload(GapAdvertisingData::CYCLING_SPEED_AND_CADENCE_SENSOR );
    
    // retreive handles
    cscMeasurementCharacteristicHandle = cscMeasurementCharacteristic.getValueHandle();
    cscFeatureCharacteristicHandle     = cscFeatureCharacteristic.getValueHandle();
    
    // initialize data
    ble.gattServer().write(cscMeasurementCharacteristicHandle, (uint8_t *)&cscMeasurementCharacteristicBuffer, sizeof(cscMeasurementCharacteristicBuffer));
    ble.gattServer().write(cscFeatureCharacteristicHandle, (uint8_t *)&cscFeatureCharacteristicBuffer, sizeof(cscFeatureCharacteristicBuffer));
    
}

void MicroBitCSCServiceDal::notifySCMeasurement(uint32_t cumulativeWheelRevolutions, uint16_t lastWheelEventTime, uint16_t cumulativeCrankRevolutions, uint16_t lastCrankEventTime)
{   
    if (ble.getGapState().connected)
    {
        struct_pack(cscMeasurementCharacteristicBuffer, "<BHH", CSCMeasurementFlags, cumulativeCrankRevolutions, lastCrankEventTime);
        ble.gattServer().notify(cscMeasurementCharacteristicHandle, (uint8_t *)&cscMeasurementCharacteristicBuffer, sizeof(cscMeasurementCharacteristicBuffer));
    }
}

// UUIDs
const uint16_t  CSCServiceUUID = 0x1816;
const uint16_t  CSCMeasurementCharacteristicUUID = 0x2A5B;
const uint16_t  CSCFeatureCharacteristicUUID     = 0x2A5C;
const uint16_t  SensorLocationCharacteristicUUID = 0x2A5D;
const uint16_t  SCControlPointCharacteristicUUID = 0x2A55;

// Flags, CSC measurement
//                                     76543210
//                                     000000 (bit 7-2) Reserved For Future Use
//                                           1 (bit 1) Crank Revolution Data Present
//                                            0 (bit 0) Wheel Revolution Data Present 
const uint8_t  CSCMeasurementFlags = 0b00000010;
// CSC Feature
//                            5432109876543210
//                            0000000000000 (bit 15-3) Reserved For Future Use
//                                         0 (bit 2) Multiple Sensor Locations Supported  
//                                          1 (bit 1) Crank Revolution Data Supported  
//                                           0 (bit 0) Wheel Revolution Data Supported
const uint16_t CSCFeature = 0b0000000000000010;

//================================================================
#endif // MICROBIT_CODAL
//================================================================


MicroBitCSCService::MicroBitCSCService()
{
    pCSCServiceDal = new MicroBitCSCServiceDal(*uBit.ble);
    updateSampleTimestamp = 0;
    cumulativeCrankRevolutions = 1;
    lastCrankEventTime = getEventTimeRollOverEvery64Seconds(system_timer_current_time_us());
    
    // start
    idleTick();
}

void MicroBitCSCService::idleTick()
{
    update();
}

void MicroBitCSCService::update(void)
{
    uint64_t currentTime = system_timer_current_time_us();
    if(!(status & MICROBIT_CSC_SERVICE_ADDED_TO_IDLE))
    {
        // If we're running under a fiber scheduer, register ourselves for a periodic callback to keep our data up to date.
        // Otherwise, we do just do this on demand, when polled through our read() interface.
        fiber_add_idle_component(this);
        status |= MICROBIT_CSC_SERVICE_ADDED_TO_IDLE;
    }
    
    if (currentTime >= updateSampleTimestamp)
    {        
        // Schedule our next sample.
        updateSampleTimestamp = currentTime + SENSOR_UPDATE_PERIOD_US;
        pCSCServiceDal->notifySCMeasurement(0, 0, cumulativeCrankRevolutions, lastCrankEventTime);
    }
}

uint16_t MicroBitCSCService::getEventTimeRollOverEvery64Seconds(uint64_t timestamp)
{
    return (timestamp * 1024) / 1000000;
}

void MicroBitCSCService::countUpCrankRevolutions(uint64_t timestamp)
{
    lastCrankEventTime = getEventTimeRollOverEvery64Seconds(timestamp);
    ++cumulativeCrankRevolutions;
}
