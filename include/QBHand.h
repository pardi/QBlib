#ifndef QBHAND_H
#define QBHAND_H

#include <QBcube.h>


class QBHand : public QBcube
{
    public:
        //------------------------------ QBHand
        /*
        / Costructor of QBHand class
        */
        QBHand(int);

        //------------------------------ QBHand
        /*
        / External costructor of QBHand class
        */
        QBHand(comm_settings*, int);

        //------------------------------ ~QBHand
        /*
        / Destructor of QBHand class
        */
        virtual ~QBHand();

        //------------------------------ setPosition
        /*
        / Set closure of the hand
        */
        bool setPosPerc(double);

        //------------------------------ getMeas
        /*
        /  Get Measurement in tic of the hand closure
        */
        bool getMeas(short int*);


        //------------------------------ getAngle
        /*
        / Get Measurement in angle of the hand closure
        */
        bool getAngle(double*);


        //------------------------------ open
        /*
        / Open connection
        */

        bool open(const char*);

        //------------------------------ init
        /*
        / Inizialize default values for the cube
        */

        void init();

    protected:

        // Position limit for motors
        int POS_LIMIT_M1[2], POS_LIMIT_M2[2];

    private:


};

#endif // QBHAND_H
