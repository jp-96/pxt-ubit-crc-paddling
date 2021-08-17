#include "customlib.h"

namespace customlib {

    int countdown(int value) {
        return --value < minvalue ? maxvalue : value;
    }

    int countup(int value) {
        return ++value > maxvalue ? minvalue : value;
    }

}