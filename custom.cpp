
#include "pxt.h"
#include "MicroBitCSCService.h"
#include "AccCSCSensor.h"

namespace custom {
    
    MicroBitCSCService* _pCSCService = NULL;
    AccCSCSensor *_pCSCSensor = NULL;

    void cscSensorUpdate(MicroBitEvent e)
    {
        if (NULL==_pCSCService) return;
        _pCSCService->countUpCrankRevolutions(e.timestamp);
    }

    //%
    void startCSCService() {
        if (NULL!=_pCSCService) return;
        _pCSCService = new MicroBitCSCService();
        _pCSCSensor = new AccCSCSensor(uBit.accelerometer);
        if (EventModel::defaultEventBus)
            EventModel::defaultEventBus->listen(CUSTOM_ID_ACC_CSC_SENSOR, CUSTOM_ACC_CSC_SENSOR_EVT_CADENCE, &cscSensorUpdate,  MESSAGE_BUS_LISTENER_IMMEDIATE);
    }

    //%
    void countUpCrankRevolutions()
    {
        startCSCService();
        new MicroBitEvent(CUSTOM_ID_ACC_CSC_SENSOR, CUSTOM_ACC_CSC_SENSOR_EVT_CADENCE);
    }

}