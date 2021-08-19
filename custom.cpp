#include "MicroBitCSCService.h"

namespace custom_cpp {
    
    MicroBitCSCService* _pCSCService = NULL;

    /**
    * Start CSC Service.
    */
    //%
    void startCSCService() {
        if (NULL!=_pCSCService) return;
        _pCSCService = new MicroBitCSCService();
    }

    /**
     * Count up crank revolutions
     * 
     */
    //%    
    void countUpCrankRevolutions()
    {
        startCSCService();
        _pCSCService->countUpCrankRevolutions();
    }

}