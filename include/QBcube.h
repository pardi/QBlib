#ifndef QBCUBE_H
#define QBCUBE_H

#include <string.h>
#include <iostream>
#include <math.h>

#include "qbmove_communications.h"
#include "definitions.h"



class QBcube
{
    public:

    //------------------------------ QBcube
    /*
    / Costructor of QBcube class
    */
    QBcube(const int id = 1);

    //------------------------------ QBcube
    /*
    / External costructor of QBcube class
    */
    QBcube(comm_settings*, const int id = 1);

    //------------------------------ ~QBcube
    /*
    / Distructor of QBcube class
    */
    virtual ~QBcube();


    // ------------------------------
    //      Further Functions
    // ------------------------------

    //------------------------------ open
    /*
    / Open serial communication
    */
    //bool open();
    bool open(const char*);

    //------------------------------ setPort
    /*
    / Set communication port name
    */
    //bool setPort(const char*);

    //------------------------------ getMeas
    /*
    / Get measurement of positions [1, 2 and L] from cube in tic
    */
    bool getMeas(short int*);

    //------------------------------ setPosition
    /*
    / Set position and stiffness preset of the cube, default input is set to radiants
    */
    bool setPosAndPreset(double, double, const char* unit = "rad");

    //------------------------------ setPosition
    /*
    / Set position and stiffness percentage of the cube, default input is set to radiants
    */
    bool setPosAndStiffPerc(double, double, const char* unit = "rad");

    //------------------------------ close
    /*
    / Close serial communication
    */
    void close();

    //------------------------------ getAngle
    /*
    / Get position angle in the chosen unit of measure
    */
    bool getAngle(double*, const char* unit  = "rad");


    //------------------------------ gePreset
    /*
    / Get preset of the cube
    */
    bool getPreset(double*);

    // //------------------------------ init

    // / Inizialize default values for the cube

    // void init();

    //------------------------------ activate
    /*
    / Active the cube
    */
    bool activate();

    //------------------------------ deactivate
    /*
    / Active the cube
    */
    bool deactivate();


    protected:
        // Protect Variables

    comm_settings* cube_comm;
    int ID;
    bool stateAct;

    //, stateOpen;


    private:

};

#endif // QBCUBE_H
