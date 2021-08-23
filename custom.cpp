#include "MicroBitCSCService.h"

namespace custom {
    
    MicroBitCSCService* _pCSCService = NULL;

    //%
    void startCSCService() {
        if (NULL!=_pCSCService) return;
        _pCSCService = new MicroBitCSCService();
    }

    //%
    void countUpCrankRevolutions()
    {
        startCSCService();
        _pCSCService->countUpCrankRevolutions();
    }

}