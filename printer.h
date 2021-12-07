#ifndef PRINTER_H
#define PRINTER_H

#include <memory>

/********* Printer *********
 * The Printer class prints the states of each object to stdout.
 * States of all objects are buffered until an object state must be rewritten.
 * When this happens, the states are printed out and the new state is updated.
 * Each object is stored in a (individual) single buffer 
 * and all single buffers are stored in an array of buffers.
 * This array stores objects in the following order: 
 * unique objects, students, machines, couriers
 ***************************/
_Monitor Printer {
    public:
        // object kinds
        enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
        Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
        ~Printer();
        void print( Kind kind, char state );
        void print( Kind kind, char state, unsigned int value1 );
        void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
        void print( Kind kind, unsigned int lid, char state );
        void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
        void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );

    private:
        /********* SingleBuff *********
         * SingleBuff stores the state of one object, 
         * until the Printer prints it out.
         ***************************/
        struct SingleBuff { 
            bool empty = true;          // is the buffer empty?
            char state;                 // state
            unsigned int value1;        // value1 (may be empty)
            unsigned int value2;        // value2 (may be empty)
        }; // SingleBuff

        unsigned int numStud; // number of students
        unsigned int numMach; // number of vending machines
        unsigned int numCour; // number of couriers
        std::unique_ptr<SingleBuff []> arrayOfBuff; // array of SingleBuff 
                                                    // (one per object)

        // get index of object with local identifier
        unsigned int getIndex( Kind kind, unsigned int lid ); 

        void printValue1( SingleBuff& buff ); // print value1 of buff
        void printValue2( SingleBuff& buff ); // print value1 and value2 of buff
        void flush();       // flush arrayOfBuff
        void printTabs( SingleBuff& buff, int& numMissingTabs ); // print tabs
        
        // print unique object buffer
        void printUnique( SingleBuff& buff, Kind kind); 
        void printStudBuff( SingleBuff& buff ); // print student buffer
        void printMachBuff( SingleBuff& buff ); // print machine buffer
        void printCourBuff( SingleBuff& buff ); // print courier buffer
}; // Printer

#endif // PRINTER_H