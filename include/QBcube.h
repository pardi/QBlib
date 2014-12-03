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
        double getStiff();

        void Init();

        bool activate();

    protected:
        // Protect Variables

        comm_settings* cube_comm;
        int ID;
        bool stateAct, stateOpen;
        std::string port_;

        int POS_LIMIT_M1[2], POS_LIMIT_M2[2]; // Posizioni limite motore 1 e 2

        // Protect Function


    private:

};

#endif // QBCUBE_H
