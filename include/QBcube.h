#ifndef QBCUBE_H
#define QBCUBE_H

#include <string>
#include <iostream>
#include <math.h>

#include "qbmove_communications.h"
#include "definitions.h"



class QBcube
{
    public:
        QBcube(int);
        QBcube(comm_settings*, int);
        virtual ~QBcube();

        bool open();
        bool setPort(const char*);
        short int* getMeas();
        virtual short int* setPosition(double, double); // Pos e Stiff Simple
        bool close();
        double getAngle();

        bool activate();


    protected:
        // Protect Variables

        comm_settings* cube_comm;
        int ID;
        bool stateAct, stateOpen;
        std::string port_;


        // Protect Function
        void Init();

    private:

};

#endif // QBCUBE_H
