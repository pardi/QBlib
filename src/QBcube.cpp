#include "QBcube.h"

using std::cout;
using std::cerr;
using std::cin;
using std::endl;

//-----------------------------------------------------
//                                               QBcube
//-----------------------------------------------------

/*
/ *****************************************************
/ Costructor of QBcube class
/ *****************************************************
/   argument:
/       - id, ID of the cube
/   return:
/
*/

QBcube::QBcube(const int id) {

    ID = id;
    cube_comm = NULL;
}

//-----------------------------------------------------
//                                               QBcube
//-----------------------------------------------------

/*
/ *****************************************************
/ External costructor of QBcube class
/ *****************************************************
/   argument:
/       - cs, serial communication pointer to the cube
/       - id, ID of the cube
/   return:
/
*/

QBcube::QBcube(comm_settings* cs, const int id) {

    ID = id;
    cube_comm = cs;
}


//-----------------------------------------------------
//                                              ~QBcube
//-----------------------------------------------------

/*
/ *****************************************************
/ Distructor of QBcube class
/ *****************************************************
/
/   arguments:
/   return:
/
*/

QBcube::~QBcube() {

    close();

    delete cube_comm;
    cube_comm = NULL;
}

//-----------------------------------------------------
//                                              getMeas
//-----------------------------------------------------

/*
/ *****************************************************
/ Get measurement of positions [1, 2, L] from cube in ticks
/ *****************************************************
/   arguments:
/       - meas, 3 elements array pointer for measurements
/   return:
/       true  on success
/       false on failure
/
*/

bool QBcube::getMeas(short int* meas) {

    if (cube_comm == NULL)
        return false;

    if (commGetMeasurements(cube_comm, ID, meas))
        return false;

    return true;
}

//-----------------------------------------------------
//                                      setPosAndPreset
//-----------------------------------------------------

/*
/ *****************************************************
/ Set position and stiffness preset of the cube, default
/ input is set to radiants
/ *****************************************************
/   argumnets:
/       - position, reference angle
/       - stiffPreset, stiffness preset
/       - unit, measurement unit for position
/               and stiffness [rad or deg]
/   return:
/       true  on success
/       false on failure
/
*/

bool QBcube::setPosAndPreset(double position, double stiffPreset, const char* unit) {

    // Check input values consistence

    if (stiffPreset > DEFAULT_STIFFNESS) {

        stiffPreset = DEFAULT_STIFFNESS;
        cerr << "WARNING: Preset saturated to " << stiffPreset << endl;

    } else if (stiffPreset < 0) {

        stiffPreset = 0;
        cerr << "WARNING: Preset saturated to " << stiffPreset << endl;

    }

    // Convert position in 'deg' if needed

    if (!strcmp(unit, "rad")) {

        position *= (180.0 / M_PI);

    } else if (strcmp(unit, "deg")) {

        cerr << "ERROR: specify 'deg' or 'rad' as unit" << endl;
        return false;

    }

    // Check Max Position available
    // XXX TODO: confronto fatto in gradi, cambinare in tic?
    
    if (position > DEFAULT_SUP_LIMIT*DEG_TICK_MULTIPLIER - stiffPreset){
        cerr << position << endl;
        position = DEFAULT_SUP_LIMIT*DEG_TICK_MULTIPLIER - stiffPreset;
        cerr << "WARNING: Position saturated to " << position << endl;
    }
    else 
        if (position < DEFAULT_INF_LIMIT*DEG_TICK_MULTIPLIER + stiffPreset){
            position = DEFAULT_INF_LIMIT*DEG_TICK_MULTIPLIER + stiffPreset;
            cerr << "WARNING: Position saturated to " << position << endl;
        }

    short int pos, sPreset;
    short int curr_ref[2];

    pos = position * DEG_TICK_MULTIPLIER;
    sPreset = stiffPreset * DEG_TICK_MULTIPLIER;

    // Set position for the 2 motors of the cube

    curr_ref[0] = pos - sPreset;
    curr_ref[1] = pos + sPreset;

    // Call API function

    commSetInputs(cube_comm, ID, curr_ref);

    return true;
}

//-----------------------------------------------------
//                                   setPosAndStiffPerc
//-----------------------------------------------------

/*
/ *****************************************************
/ Set position and stiffness percentage of the cube,
/ default input is set to radiants
/ *****************************************************
/   argumnets:
/       - position, reference angle
/       - stiffPerc, stiffness percentage; range: [0 - 32767]
/       - unit, measurement unit for position [rad or deg]
/
/   return:
/       true  on success
/       false on failure
/
*/

bool QBcube::setPosAndStiffPerc(double position, double stiffPerc, const char* unit) {

    // Check input values consistence

    if (stiffPerc > 100) {

        stiffPerc = 100;
        cerr << "WARNING: Stiffness percentage saturated to " << stiffPerc << endl;

    } else if (stiffPerc < 0) {

        stiffPerc = 0;
        cerr << "WARNING: Stiffness percentage saturated to " << stiffPerc << endl;

    }

    // Convert position in 'deg' if needed

    if (!strcmp(unit, "rad")) {

        position *= (180.0 / M_PI);

    } else if (strcmp(unit, "deg")) {

        cerr << "ERROR: specify 'deg' or 'rad' as unit" << endl;
        return false;

    }


    // XXX TODO: Check Max Position available
    
    
    short int curr_ref[2];

    curr_ref[0] = (short int)(position * DEG_TICK_MULTIPLIER);
    curr_ref[1] = (short int)((stiffPerc * 32767.0) / 100.0);

    // Call API function

    commSetInputs(cube_comm, ID, curr_ref);

    return false;
}



//-----------------------------------------------------
//                                                 open
//-----------------------------------------------------

/*
/ *****************************************************
/ Open serial communication with cube
/ *****************************************************
/   arguments:
/       port, the communication port
/   return:
/       true  on success
/       false on failure
/
*/

bool QBcube::open(const char* port) {

    // Check Connection State

    if(cube_comm != NULL) {
        cerr << "WARNING: Port already opened" << endl;
        return false;
    }

    cube_comm = new comm_settings;

    // Establish serial connection

    openRS485(cube_comm, port);

    if (cube_comm->file_handle == INVALID_HANDLE_VALUE) {
        cerr << "ERROR: Unable to open port" << endl;
        return false;
    }

    return true;
}

//-----------------------------------------------------
//                                             activate
//-----------------------------------------------------

/*
/ *****************************************************
/ Activate the cube
/ *****************************************************
/   parameters:
/   return:
/       true  on success
/       false on failure
/
*/

bool QBcube::activate() {

    // Check connection state

    if (cube_comm == NULL) {
        cerr << "ERROR: Port not opened" << endl;
        return false;
    }

    // Call cube function

    commActivate(cube_comm, ID, 1);

    // Check if the cube is active

    char status;
    commGetActivate(cube_comm, ID, &status);

    if (!status){
        cerr << "Unable to activate" << endl;
        return false;
    }

    return true;
}

//-----------------------------------------------------
//                                           deactivate
//-----------------------------------------------------

/*
/ *****************************************************
/ Active the cube
/ *****************************************************
/   parameters:
/   return:
/       true  on success
/       false on failure
/
*/

bool QBcube::deactivate() {

    // Check connection state

    if (cube_comm == NULL){
        cerr << "ERROR: Port not opened" << endl;
        return false;
    }

    // Call cube function

    commActivate(cube_comm, ID, 0);

    // Check if the cube is active

    char status;
    commGetActivate(cube_comm, ID, &status);

    if (status){
        cerr << "Unable to deactivate" << endl;
        return false;
    }

    return true;
}


//-----------------------------------------------------
//                                                close
//-----------------------------------------------------

/*
/ *****************************************************
/ Close serial communication
/ *****************************************************
/   arguments:
/   return:
/
*/

void QBcube::close() {

    // Check Communication state

    deactivate();

    // Call cube function

    closeRS485(cube_comm);

    // XXX TODO: implementare close RS485 per la gestione dell'errore in chiusura
}


//-----------------------------------------------------
//                                             getAngle
//-----------------------------------------------------

/*
/ *****************************************************
/ Get position angle in the chosen unit of measure
/ *****************************************************
/   arguments:
/       - angle, getted position in angle
/       - unit, unit of measure [rad o deg]
/   return:
/       true  on success
/       false on failure
/
*/

bool QBcube::getAngle(double* angle, const char* unit){

    short int meas[3];

    // Get measurements
    if(!getMeas(meas))
        return false;

    // Return position in the right unit

    if (!strcmp(unit, "deg")) {
        *angle = (((double) meas[2])/DEG_TICK_MULTIPLIER);
    } else if (!strcmp(unit, "rad")) {
        *angle = (((double) meas[2])/DEG_TICK_MULTIPLIER)*(M_PI/180);
    } else {
        cerr << "ERROR: specify 'deg' or 'rad' as unit" << endl;
        return false;
    }

    return true;
}

//-----------------------------------------------------
//                                            getPreset
//-----------------------------------------------------

/*
/ *****************************************************
/ Get preset of the cube
/ *****************************************************
/   parameters:
/   return:
/       - preset value
/
*/

//XXX ma serve? o comunque non e' meglio calcolarlo dai riferimenti che non dalle misure?
bool QBcube::getPreset(double* preset) {

    short int meas[3];

    // Get position
    if(!getMeas(meas))
        return false;

    // Compute preset value
    *preset = (((double) meas[0] - (double) meas[1])/2)/DEG_TICK_MULTIPLIER;

    return true;
}
