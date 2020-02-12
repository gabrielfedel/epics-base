/*************************************************************************\
* Copyright (c) 2020 Gabriel Fedel 
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

#include "dbUnitTest.h"
#include "testMain.h"
#include "errlog.h"
#include "dbAccess.h"
#include <epicsThread.h>

void recTestIoc_registerRecordDeviceDriver(struct dbBase *);

static
void testEventSoft(void){
    // Initial behaviour, ao should have stat UDF and sevr INVALID
    testdbGetFieldEqual("ao0.STAT", DBR_LONG, 17); // udf
    testdbGetFieldEqual("ao0.SEVR", DBR_LONG, 3);  // invalid
    // Test if when event (soft) is processed the other records with SCAN
    // event and the same EVNT are processed too
    testdbPutFieldOk("event0.PROC", DBR_LONG, 1);
    epicsThreadSleep(0.1);
    testdbGetFieldEqual("ao0.STAT", DBR_LONG, 0); // udf
    testdbGetFieldEqual("ao0.SEVR", DBR_LONG, 0);  // invalid
}


MAIN(eventTest) {
    testPlan(5);

    testdbPrepare();

    testdbReadDatabase("recTestIoc.dbd", NULL, NULL);
    recTestIoc_registerRecordDeviceDriver(pdbbase);

    testdbReadDatabase("eventTest.db", NULL, NULL);

    eltc(0);
    testIocInitOk();
    eltc(1);

    testEventSoft();

    testIocShutdownOk();
    testdbCleanup();

    return testDone();
}
