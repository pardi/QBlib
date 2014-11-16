#include "QBcube.h"

// Costruttore di un elemento Cubo scelto un ID

QBcube::QBcube(int id = 1)
{
    ID = id;
    cube_comm = NULL;
    stateAct = false;
    stateOpen = false;
}

// Costruttore di un elemento Cubo scelto un ID, con seriale fornita dall'esterno

QBcube::QBcube(comm_settings* cs, int id = 1)
{
    ID = id;
    stateAct = false;
    stateOpen = true;
    cube_comm = cs;
}

// Distruttore

QBcube::~QBcube()
{
    close();
}


short int* QBcube::getMeas(){

    short int *meas = new short int(3);

    if (cube_comm == NULL)
        return NULL;

    commGetMeasurements(cube_comm, ID, meas);

    return meas;
}

short int* QBcube::setPosition(double position, double stiffness){

    if ((stiffness > 1) || (stiffness < 0))
        return NULL;

  //  if (!stateAct || open())
  //      return NULL;

    if(!activate())
        return NULL;

    short int pos, stiff;

    pos = -position * (180.0 / M_PI);

    // Convert degrees to ticks.
    pos *= DEG_TICK_MULTIPLIER;
    stiff = stiffness * DEG_TICK_MULTIPLIER;

    // Stiffness and position set.
    short int curr_ref[2];

    if (pos > (DEFAULT_SUP_LIMIT / pow(2, DEFAULT_RESOLUTION) - DEFAULT_STIFFNESS * DEG_TICK_MULTIPLIER))
        pos = (DEFAULT_SUP_LIMIT / pow(2, DEFAULT_RESOLUTION) - DEFAULT_STIFFNESS * DEG_TICK_MULTIPLIER);
    else
        if (pos < (DEFAULT_INF_LIMIT / pow(2, DEFAULT_RESOLUTION) + DEFAULT_STIFFNESS * DEG_TICK_MULTIPLIER))
            pos = (DEFAULT_INF_LIMIT / pow(2, DEFAULT_RESOLUTION) + DEFAULT_STIFFNESS * DEG_TICK_MULTIPLIER);


    if (stiff > DEFAULT_STIFFNESS * DEG_TICK_MULTIPLIER)
        stiff = DEFAULT_STIFFNESS * DEG_TICK_MULTIPLIER;
    else
        if (stiff < 0)
            stiff = 0;

    // Position for the 2 engine of the cube.
    curr_ref[0] = pos - stiff;
    curr_ref[1] = pos + stiff;

    commSetInputs(cube_comm, ID, curr_ref);

    if (cube_comm->file_handle == INVALID_HANDLE_VALUE)
        return NULL;

    return getMeas();

}


bool QBcube::open(){

    if (stateAct){
        std::cout << "Cube already open." << std::endl;
        return false;
    }

    if(cube_comm == NULL){
        cube_comm = new comm_settings;
        if(port_.empty()){
            std::cout << "Port not setted." << std::endl;
            return false;
        }
        openRS485(cube_comm, port_.c_str());

        if (cube_comm->file_handle == INVALID_HANDLE_VALUE)
            return false;
    }

    return activate();
}

// Attivo il cubo

bool QBcube::activate(){

    if (stateAct)
        return true;

    if (cube_comm == NULL)
        return false;

    commActivate(cube_comm, ID, 1);

    if (cube_comm->file_handle == INVALID_HANDLE_VALUE)
        return false;

    stateAct = true;

    return true;
}

// Chiudo la comunicazione seriale

bool QBcube::close(){

    if (cube_comm == NULL)
        return false;

    stateAct = false;

    closeRS485(cube_comm);

    if (cube_comm->file_handle == INVALID_HANDLE_VALUE)
        return false;

    delete cube_comm;
    cube_comm = NULL;

    return true;
}

// Setto il nome della porta

bool QBcube::setPort(const char * P_IN){

    if (P_IN == NULL)
        return false;

    if (!port_.empty()){
        std::cout << "Porta gia' settata" << std::endl;
        return false;
    }
    port_ = P_IN;

    return true;
}

// Inizializzo i valori di default del Cubo

void QBcube::Init(){

    int pos_limits[4];

    while(commGetParam(cube_comm, ID, PARAM_POS_LIMIT, pos_limits, 4));

    std::cout << pos_limits[0] << " " << pos_limits[1] << " " << pos_limits[2] << " " << pos_limits[3] << std::endl;


}

double QBcube::getAngle(){

	short int* meas;

	meas = getMeas();

	double k = meas[2];

	//std::cout << 88*DEG_TICK_MULTIPLIER*(M_PI/180)<< "D " <<meas[2] << std::endl;

	return -(((double) meas[2])/DEG_TICK_MULTIPLIER)*(M_PI/180.0);

}

