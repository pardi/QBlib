#ifndef QBCHAIN_H
#define QBCHAIN_H

#define NUM_OUT 10

#include <vector>
#include <string.h>
#include "QBcube.h"
#include "QBHand.h"


class QBchain
{

    comm_settings* cube_comm;
    std::vector<QBcube*> cubes;
    std::vector<QBHand*> hands;

    public:
        //------------------------------ QBchain
        /*
        / Costructor of QBchain class
        */
        QBchain();

        //------------------------------ ~QBchain
        /*
        / Distructor of QBcube class
        */
        virtual ~QBchain();


        // ------------------------------
        //              Further Functions
        // ------------------------------

        //------------------------------ open
        /*
        / Open connection with all cubes and hands
        */
        bool open(const char*);

        //------------------------------ close
        /*
        / Close connection of all cubes and hands
        */
        bool close();

        // ------------------------------
        //       Member Functions
        // ------------------------------

        //------------------------------ addCubes
        /*
        / Add cubes to the chain
        */
        bool addCubes(std::vector<int>);

        //------------------------------ addHands
        /*
        / Add hands to the chain
        */
        bool addHands(std::vector<int>);

        //------------------------------ moveIt
        /*
        / Move the chain to desiderated position and preset,
        / commanded is given in q-space configuration
        */
        bool moveIt(std::vector<double>, std::vector<double>, const char* unit);

        //------------------------------ closeIt
        /*
        / Close hands
        */
        bool closeIt(std::vector<double>);

        //------------------------------ readIt
        /*
        /Read position of all engines
        */
        bool readIt(short int*, short int*);

        //------------------------------ readIt
        /*
        / Read position of all engines
        */
        bool readAng(double*, double*, const char*);

        //------------------------------ init
        /*
        / Init default function for all objects
        */
        void init();

        //------------------------------ active
        /*
        / Close connection of all cubes and hands
        */
        bool activate();

        //------------------------------ active
        /*
        / Close connection of all cubes and hands
        */
        bool deactivate();


    protected:

        //------------------------------ DH2Jacobian
        /* TODO
        / Generate Jacobian through DH matrix
        */
        void DH2Jacobian(const char *);

    private:




};

#endif // QBCHAIN_H

