#include "pxt.h"
#include "AccCSCSensor.h"

AccCSCSensor::AccCSCSensor(
//================================================================
#if MICROBIT_CODAL
//================================================================

    codal::Accelerometer  &_accelerometer

//================================================================
#else // MICROBIT_CODAL
//================================================================

    MicroBitAccelerometer &_accelerometer

//================================================================
#endif // MICROBIT_CODAL
//================================================================
):
        accelerometer(_accelerometer)
{
    accelerometer.setPeriod(50);
    _evtCadence = new MicroBitEvent(CUSTOM_ID_ACC_CSC_SENSOR, CUSTOM_ACC_CSC_SENSOR_EVT_CADENCE, CREATE_ONLY);

    memset(&pitching, 0, sizeof(pitching));
    pitchingNextIdx = 0;
    pitchingMax = 0;
    pitchingMin = 0;
    pitchingMeanLast = 0;
    pitchingLast = 0;
    pitchingSensitivity = ACC_CSC_SENSOR_PITCHING_SENSITIVITY;

    cadenceTimeUs = system_timer_current_time_us();

    if (EventModel::defaultEventBus)
        EventModel::defaultEventBus->listen(MICROBIT_ID_ACCELEROMETER, MICROBIT_ACCELEROMETER_EVT_DATA_UPDATE, this, &AccCSCSensor::accelerometerUpdate,  MESSAGE_BUS_LISTENER_IMMEDIATE);
}

void AccCSCSensor::accelerometerUpdate(MicroBitEvent e)
{
    int ap = accelerometer.getY();

    // pitching
    pitchingMax = pitchingMax > ap ? pitchingMax : ap;
    pitchingMin = pitchingMin < ap ? pitchingMin : ap;
    pitching[pitchingNextIdx] = ap;
    pitchingNextIdx = (pitchingNextIdx+1) % ACC_CSC_SENSOR_PITCHING_SIZE;

    int pitchingMean = 0;
    for (int i=0; i<ACC_CSC_SENSOR_PITCHING_SIZE; i++)
    {
        pitchingMean += pitching[i];
    }
    pitchingMean = pitchingMean / ACC_CSC_SENSOR_PITCHING_SIZE;
    
    // check point
    int cp;
    if ((pitchingMeanLast < pitchingLast) && (pitchingMean >= ap))
    {
        cp = 1;
    }
    else if ((pitchingMeanLast >= pitchingLast) && (pitchingMean < ap))
    {
        cp = -1;
    }
    else
    {
        cp = 0;
    }
    if ((cp != 0) && ((pitchingMax - pitchingMin)<pitchingSensitivity))
    {
        cp = 0;
    }

    // Next
    pitchingLast = ap;
    pitchingMeanLast = pitchingMean;
    if (cp>0)
    {
        pitchingMax = ap;
    }
    else if (cp<0)
    {
        pitchingMin = ap;
    }

    // cadence event
    if (cp>0) {
        cadenceTimeUs = e.timestamp;
        _evtCadence->timestamp = e.timestamp;
        _evtCadence->fire();
    }
}

uint64_t AccCSCSensor::getCadenceTimeUs()
{
    return cadenceTimeUs;
}
