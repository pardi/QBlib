#include "QBchain.h"


//-----------------------------------------------------
//                                              QBchain
//-----------------------------------------------------

/*
/ *****************************************************
/ Costructor of QBchain class
/ *****************************************************
/       _input:
/       _output:
/
*/

QBchain::QBchain(){

    cube_comm = NULL;
}

//-----------------------------------------------------
//                                             addCubes
//-----------------------------------------------------

/*
/ *****************************************************
/ Add cubes to the chain
/ *****************************************************
/       _input:
/       - cubeBuf, vector with IDs of cubes
/       _output:
/       - [state]
/
*/

bool QBchain::addCubes(std::vector<int> cubeBuf)
{
    QBcube* tmp_cube;

    // Allocate object QBcube, one for each cube in cubeBuf

    for (std::vector<int>::iterator it = cubeBuf.begin(); it != cubeBuf.end(); ++it){
        tmp_cube = new QBcube(cube_comm, *it);
        cubes.push_back(tmp_cube);
    }

    return true;
}

//-----------------------------------------------------
//                                             addHands
//-----------------------------------------------------

/*
/ *****************************************************
/ Add hands to the chain
/ *****************************************************
/       _input:
/               - cubeBuf, vector with ID of hands
/       _output:
/               - [state]
/
*/

bool QBchain::addHands(std::vector<int> handBuf){

    QBHand* tmp_hand;

    // Alloc object QBHand, one for each cube in handBuf

    for (std::vector<int>::iterator it = handBuf.begin(); it != handBuf.end(); ++it){
        tmp_hand = new QBHand(cube_comm, *it);
        hands.push_back(tmp_hand);
    }

    return true;
}

//-----------------------------------------------------
//                                             ~QBchain
//-----------------------------------------------------

/*
/ *****************************************************
/ Destructor of QBchain class
/ *****************************************************
/       _input:
/       _output:
/
*/

QBchain::~QBchain()
{

    close();
    // Delete all cubes

    for (std::vector<QBcube*>::iterator it = cubes.begin(); it!=cubes.end(); ++it)
            delete (*it);

    // Delete all hands

    for (std::vector<QBHand*>::iterator it = hands.begin(); it!=hands.end(); ++it)
            delete (*it);

    cube_comm = NULL;
}


//-----------------------------------------------------
//                                                 open
//-----------------------------------------------------

/*
/ *****************************************************
/ Open connection with all cubes and hands
/ *****************************************************
/       _input:
/       _output:
/               [state]
/
*/

bool QBchain::open(const char* port = "/dev/ttyUSB0"){

    // Check connection state
    if (cube_comm != NULL)
        return false;

    cube_comm = new comm_settings;

    // Call cube function

    openRS485(cube_comm, port);

    if (cube_comm->file_handle == INVALID_HANDLE_VALUE)
        return false;

    // Set default configuration for all objects

    init();

    return true;
}

//-----------------------------------------------------
//                                                close
//-----------------------------------------------------

/*
/ *****************************************************
/ Close connection of all cubes and hands
/ *****************************************************
/       _input:
/       _output:
/               [state]
/
*/

bool QBchain::close(){

    // Check connection state

    if (cube_comm == NULL)
        return false;

    // Deactivate cubes

    deactivate();

    // Call cube function

    closeRS485(cube_comm);

    return true;
}

//-----------------------------------------------------
//                                             activate
//-----------------------------------------------------

/*
/ *****************************************************
/ Close connection of all cubes and hands
/ *****************************************************
/       _input:
/       _output:
/               [state]
/
*/

bool QBchain::activate(){

    // Check connection state

    if (cube_comm == NULL)
        return false;

    // Activate all cubes

    int i = -1;

    for (std::vector<QBcube*>::iterator it = cubes.begin(); it!=cubes.end(); ++it)
        while(!((*it)->activate()) && ((++i) < 10));

    if (i == NUM_OUT)
        return false;

    i = -1;

    // Activate all hands

    for (std::vector<QBHand*>::iterator it = hands.begin(); it!=hands.end(); ++it)
        while(!((*it)->activate()) && ((++i) < 10));

    if (i == NUM_OUT)
        return false;

    return true;
}

// TODO: Selective Activate

//-----------------------------------------------------
//                                           deactivate
//-----------------------------------------------------

/*
/ *****************************************************
/ Close connection of all cubes and hands
/ *****************************************************
/       _input:
/       _output:
/               [state]
/
*/

bool QBchain::deactivate(){

    // Check connection state

    if (cube_comm == NULL)
        return false;

    int i = -1;

    // Activate all cubes

    for (std::vector<QBcube*>::iterator it = cubes.begin(); it!=cubes.end(); ++it)
        while(!((*it)->deactivate()) && ((++i) < 10));

    if (i == NUM_OUT)
        return false;

    i = -1;

    // Activate all hands

    for (std::vector<QBHand*>::iterator it = hands.begin(); it!=hands.end(); ++it)
        while(!((*it)->deactivate()) && ((++i) < 10));

    if (i == NUM_OUT)
        return false;

    return true;
}




//-----------------------------------------------------
//                                               moveIt
//-----------------------------------------------------

/*
/ *****************************************************
/ Move the chain to desiderated position and preset,
/ commanded is given in q-space configuration
/ *****************************************************
/       _input:
/       - position, position for each cubes
/       - preset, preset for each cubes
/       - unit, unit of measure
/       _output:
/               [state]
/
*/

bool QBchain::moveIt(std::vector<double> position, std::vector<double> preset, const char* unit = "rad"){

    // Check input rightness values

    if (position.size() != preset.size())
        return false;

    if (!strcmp(unit, "deg") && !strcmp(unit, "rad"))
        return false;

    std::vector<QBcube*>::iterator it = cubes.begin();
    std::vector<double>::iterator nP = position.begin();
    std::vector<double>::iterator nS = preset.begin();

    // Call function setPosAndPreset() for each cube

    while(it != cubes.end()){
        (*it)->setPosAndPreset(*nP, *nS, unit);
        ++it;
        ++nP;
        ++nS;
    }
    return true;
}
//-----------------------------------------------------
//                                              closeIt
//-----------------------------------------------------

/*
/ *****************************************************
/ Close hands
/ *****************************************************
/       _input:
/               - position, position for each cubes
/       _output:
/               [state]
/
*/

bool QBchain::closeIt(std::vector<double> position){

    std::vector<QBHand*>::iterator it = hands.begin();
    std::vector<double>::iterator nP = position.begin();

    // Close hands

    while(it != hands.end()){
        (*it)->setPosPerc(*nP);
        ++it;
        ++nP;
    }

    return true;
}

//-----------------------------------------------------
//                                               readIt
//-----------------------------------------------------

/*
/ *****************************************************
/ Read position of all engines
/ *****************************************************
/       _input:
/       - meas, vector of measurements [1/2/L]
/           for each cube
/       - mHand, vector of hands posititions
/       _output:
/               [state]
/
*/

bool QBchain::readIt(short int* meas, short int* mHand){

    int i = 0;

    // Read position from cubes

    for (std::vector<QBcube*>::iterator it = cubes.begin(); it!=cubes.end(); ++it){
        (*it)->getMeas(&meas[3*i]);
        ++i;
    }

    // Read position from hands

    i = 0;

    for (std::vector<QBHand*>::iterator it = hands.begin(); it!=hands.end(); ++it)
        (*it)->getMeas(&mHand[i++]);

    return true;
}

//-----------------------------------------------------
//                                              readAng
//-----------------------------------------------------

/*
/ *****************************************************
/ Read position of all engines
/ *****************************************************
/       _input:
/       - angles, angles of all cubes
        - handsC, closing rate of all hands
/               - unit, unit of measure
/       _output:
/               [state]
/
*/

bool QBchain::readAng(double* angles, double* handsC, const char* unit = "rad"){

    // Check inputs rightness

    if ((cubes.size() == 0) && (hands.size() == 0))
        return false;


    if (!strcmp(unit, "deg") && !strcmp(unit, "rad"))
        return false;

    // Read Angle of cubes and hands

    int i = 0;
    //bool flagC = true, flagM = true;

    for (std::vector<QBcube*>::iterator it = cubes.begin(); it!=cubes.end(); ++it){
        while(!((*it)->getAngle(&angles[i], unit)));
        
        i++;
         //flagC &= (*it)->getAngle(&angles[i++], unit);
    }

    i = 0;

    for (std::vector<QBHand*>::iterator it = hands.begin(); it!=hands.end(); ++it){
        while(!((*it)->getAngle(&handsC[i])));

        i++;
    }
//        flagM &= (*it)->getAngle(&handsC[i++]);


      return true;
}

//-----------------------------------------------------
//                                                 init
//-----------------------------------------------------

/*
/ *****************************************************
/ Init default function for all objects
/ *****************************************************
/       _input:
/       _output:
/               [state]
/
*/

void QBchain::init(){

    // Call init function for each hand

    for (std::vector<QBHand*>::iterator it = hands.begin(); it!=hands.end(); ++it)
        (*it)->init();
}
