#include "QBHand.h"

QBHand::QBHand(int id) : QBcube(id)
{
    cube_comm = NULL;
}

QBHand::QBHand(comm_settings* cs, int id) : QBcube(cs, id)
{

}


QBHand::~QBHand()
{
    //dtor
}

short int* QBHand::setPosition(double position){

    if (!activate())
        return NULL;

    short int curr_ref[2];

    // Position for the 2 engine of the cube.
    curr_ref[0] = position*15000;
    curr_ref[1] = position*15000;

    commSetInputs(cube_comm, ID, curr_ref);

    if (cube_comm->file_handle == INVALID_HANDLE_VALUE)
        return NULL;

    return getMeas();

}

short int* QBHand::getMeas(){

    if (cube_comm == NULL)
        return NULL;

    short int buf_meas[3];
    short int* meas = new short int;

    commGetMeasurements(cube_comm, ID, buf_meas);

    *meas = buf_meas[3];

    return meas;
}

void QBHand::Init(){

    int pos_limits[4];

    while(commGetParam(cube_comm, ID, PARAM_POS_LIMIT, pos_limits, 4));

    std::cout << pos_limits[0] << " " << pos_limits[1] << " " << pos_limits[2] << " " << pos_limits[3] << std::endl;
}
