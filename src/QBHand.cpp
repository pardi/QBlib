#include "QBHand.h"

//-----------------------------------------------------
//                                               QBHand
//-----------------------------------------------------

/*
/ *****************************************************
/ Costructor of QBHand class
/ *****************************************************
/   parameters:
/               id, ID of the cube
/   return:
/
*/

QBHand::QBHand(int id) : QBcube(id) {

    cube_comm = NULL;
}

//-----------------------------------------------------
//                                               QBHand
//-----------------------------------------------------

/*
/ *****************************************************
/ Costructor of QBHand class
/ *****************************************************
/   parameters:
/               cs, communications channel
/       id, ID of the cube
/   return:
/
*/

QBHand::QBHand(comm_settings* cs, int id) : QBcube(cs, id) {

    init();
}


//-----------------------------------------------------
//                                              ~QBHand
//-----------------------------------------------------

/*
/ *****************************************************
/ Destructor of QBHand class
/ *****************************************************
/   parameters:
/   return:
/
*/

QBHand::~QBHand() {

}
//-----------------------------------------------------
//                                                 open
//-----------------------------------------------------

/*
/ *****************************************************
/ Open serial communication with cube
/ *****************************************************
/   parameters:
/   return:
/               [status]
/
*/
bool QBHand::open(const char* port) {

    // Check Connection State

    if(!this->QBcube::open(port))
        return false;

    init();

    return true;
}


//-----------------------------------------------------
//                                           setPosPerc
//-----------------------------------------------------

/*
/ *****************************************************
/ Set closure of the hand
/ *****************************************************
/   parameters:
/       - rateC, close hand rate
/   return:
/       [state]
/
*/

bool QBHand::setPosPerc(double rateC) {

    // Check input value

    if (rateC < 0.0)
        rateC = 0;

    if (rateC > 1.0)
        rateC = 1.0;

    short int curr_ref[2];

    // Position for the 2 engine of the cube.

    curr_ref[0] = rateC*POS_LIMIT_M1[1]/4;
    curr_ref[1] = rateC*POS_LIMIT_M1[1]/4;

    // Call cube position

    commSetInputs(cube_comm, ID, curr_ref);

    return true;
}


//-----------------------------------------------------
//                                              getMeas
//-----------------------------------------------------

/*
/ *****************************************************
/ Get Measurement in tic of the hand closure
/ *****************************************************
/   parameters:
/               - meas, value, in tic, of the hand
/           closure
/   return:
/               [state]
/
*/

bool QBHand::getMeas(short int* meas) {

    // Check connection state

    if (cube_comm == NULL)
        return false;

    short int buf_meas[3];

    // Call cube function

    if(commGetMeasurements(cube_comm, ID, buf_meas))
        return false;

    // Pick engine measurement

    meas[0] = buf_meas[0];

    return true;
}

//-----------------------------------------------------
//                                             getAngle
//-----------------------------------------------------

/*
/ *****************************************************
/ Get Measurement in angle of the hand closure
/ *****************************************************
/   parameters:
/   return:
/               value in radiants of hand closure
/
*/

bool QBHand::getAngle(double* angle) {

    // Get measurment in tic

    short int meas;

    if (!getMeas(&meas))
        return false;

    // Trasform in rad and return measured value

    *angle = (((double) meas)/DEG_TICK_MULTIPLIER)*(M_PI/180.0);

    return true;
}

//-----------------------------------------------------
//                                                 init
//-----------------------------------------------------

/*
/ *****************************************************
/ Inizialize default values for the cube
/ *****************************************************
/   parameters:
/   return:
/
*/

void QBHand::init() {

    int pos_limits[4];

    // Retrive informations

    while(commGetParam(cube_comm, ID, PARAM_POS_LIMIT, pos_limits, 4));

    // Save informations

    POS_LIMIT_M1[0] = pos_limits[0]/2;
    POS_LIMIT_M1[1] = pos_limits[1]/2;
    POS_LIMIT_M2[0] = pos_limits[2]/2;
    POS_LIMIT_M2[1] = pos_limits[3]/2;

}