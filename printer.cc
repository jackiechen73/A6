#include "printer.h"
#include <iostream>         // access cout

#define NUM_UNIQUE  6

using namespace std;        // direct access to std

/********* Printer::Printer *********
 * This is the contructor of the Printer class.
 * It prints the header.
 ***************************/
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers )
: numStud( numStudents ), numMach( numVendingMachines ), numCour( numCouriers ), 
arrayOfBuff( new SingleBuff[NUM_UNIQUE + numStudents + numVendingMachines + numCouriers] ) {
    cout << "Parent\tGropoff\tWATOff\tNames\tTruck\tPlant"; // unique objects
    for ( unsigned int i = 0; i < numStud; i += 1 ) { // print each student
        cout << "\tStud" << i;
    } // for
    for ( unsigned int i = 0; i < numMach; i += 1 ) { // print each machine
        cout << "\tMach" << i;
    } // for
    for ( unsigned int i = 0; i < numCour; i += 1 ) { // print each courier
        cout << "\tCour" << i;
    } // for
    cout << endl;
    for ( unsigned int i = 0; i < NUM_UNIQUE + numStud + numMach + numCour - 1; i += 1 ) {
        cout << "*******\t";
    } // for
    cout << "*******" << endl;
}

/********* Printer::getIndex *********
 * Returns the index of the specified kind with the matching local id.
 * kind: kind of object
 * lid: local id
 ***************************/
unsigned int Printer::getIndex( Kind kind, unsigned int lid ) {
    // arrayOfBuff stores objects in the following order:
    // unique objects, students, machines, couriers
    switch ( kind ) {
        case Student:
            return lid + NUM_UNIQUE;
        case Vending:
            return lid + NUM_UNIQUE + numStud;
        case Courier:
            return lid + NUM_UNIQUE + numStud + numMach;
        default:
            return 0; // should never return this case
    }
}

/********* Printer::printValue1 *********
 * Print value1 of the specified buff.
 * buff: buffer that is printed
 ***************************/
void Printer::printValue1( SingleBuff& buff ) {
    cout << buff.value1;
}

/********* Printer::printValue2 *********
 * Print value1 and value2 of the specified buff.
 * buff: buffer that is printed
 ***************************/
void Printer::printValue2( SingleBuff& buff ) {
    cout << buff.value1 << "," << buff.value2;
}

/********* Printer::flush *********
 * Print the entire arrayOfBuff
 ***************************/
void Printer::flush() {
    unsigned int i = 0; // index of current buffer to be printed
    // tabs in front of current buffer that have no been printed
    int numMissingTabs = 0; 
    // print the unique buffers
    for ( ; i < NUM_UNIQUE; i += 1 ) {
        printTabs( arrayOfBuff[i], numMissingTabs );
        printUnique( arrayOfBuff[i], Kind(i) );
    } // for
    // print each student
    for ( ; i < NUM_UNIQUE + numStud; i += 1 ) {
        printTabs( arrayOfBuff[i], numMissingTabs );
        printStudBuff( arrayOfBuff[i] );
    } // for
    // print each machine
    for ( ; i < NUM_UNIQUE + numStud + numMach; i += 1 ) {
        printTabs( arrayOfBuff[i], numMissingTabs );
        printMachBuff( arrayOfBuff[i] );
    } // for
    // print each courier
    for ( ; i < NUM_UNIQUE + numStud + numMach + numCour; i += 1 ) {
        printTabs( arrayOfBuff[i], numMissingTabs );
        printCourBuff( arrayOfBuff[i] );
    } // for
    cout << endl;
}

/********* Printer::printTabs *********
 * Print the tabs before the specified buffer.
 * Tabs are not printed if the buffer is empty (to avoid trailing whitespace).
 * Note that numMissingTabs is passed by reference and updated accordingly.
 * buff: buffer to be printed
 * numMissingTabs: number of missing tabs
 ***************************/
void Printer::printTabs( SingleBuff& buff, int& numMissingTabs ) {
    if ( !buff.empty ) { // buffer non-empty?
        // print number of missing tabs
        for ( int i = 0; i < numMissingTabs; i += 1) {
            cout << "\t";
        } // for
        numMissingTabs = 0; // reset count
    }    
    numMissingTabs += 1; // need 1 tab to follow current output
}

/********* Printer::printUnique *********
 * Print the single buffer for Parent, Groupoff, WATCardOffice, 
 * NameServer, Truck, BottlingPlant object.
 * buff: single buffer to be printed
 * kind: object kind
 ***************************/
void Printer::printUnique( SingleBuff& buff, Kind kind) {
    if ( buff.empty ) { return; } // don't print if empty
    cout << buff.state; // always print state
    switch ( kind ) {
        case Parent:
            if ( buff.state == 'D' ) {
                printValue2( buff );
            }
            break;
        case Groupoff:
            if ( buff.state == 'D' ) {
                printValue1( buff );
            }
            break;
        case WATCardOffice:
            if ( buff.state == 'C' || buff.state == 'T' ) {
                printValue2( buff );
            }
            break;
        case NameServer:
            if ( buff.state == 'R' ) {
                printValue1( buff );
            } else if ( buff.state == 'N' ) {
                printValue2( buff );
            }
            break;
        case Truck:
            if ( buff.state == 'P' ) {
                printValue1( buff );
            } else if ( buff.state == 'd' || buff.state == 'U' || buff.state == 'D' ) {
                printValue2( buff );
            }
            break;
        case BottlingPlant:
            if ( buff.state == 'G' ) {
                printValue1( buff );
            }
            break;
        default:
            break; // should never be reached
    }
    buff.empty = true; // buffer is now empty
}

/********* Printer::printStudBuff *********
 * Print the single buffer for Student object.
 * buff: single buffer to be printed
 ***************************/
void Printer::printStudBuff( SingleBuff& buff ) {
    if ( buff.empty ) { return; } // don't print if empty
    cout << buff.state;
    if ( buff.state == 'V' ) {
        printValue1( buff );
    } else if ( buff.state == 'S' || buff.state == 'G' || buff.state == 'a' 
                || buff.state == 'B' || buff.state == 'A' ) {
        printValue2( buff );
    }
    buff.empty = true; // buffer is now empty
}

/********* Printer::printMachBuff *********
 * Print the single buffer for vending machine object.
 * buff: single buffer to be printed
 ***************************/
void Printer::printMachBuff( SingleBuff& buff ) {
    if ( buff.empty ) { return; } // don't print if empty
    cout << buff.state;
    if ( buff.state == 'S' ) {
        printValue1( buff );
    } else if ( buff.state == 'B' ) {
        printValue2( buff );
    }
    buff.empty = true; // buffer is now empty
}

/********* Printer::printCourBuff *********
 * Print the single buffer for courier object.
 * buff: single buffer to be printed
 ***************************/
void Printer::printCourBuff( SingleBuff& buff ) {
    if ( buff.empty ) { return; } // don't print if empty
    cout << buff.state;
    if ( buff.state == 'L' ) {
        printValue1( buff );
    } else if ( buff.state == 't' || buff.state == 'T' ) {
        printValue2( buff );
    }
    buff.empty = true; // buffer is now empty
}

/********* Printer::print *********
 * Called by objects when they want to print their state.
 * The kind of the object corresponds to it index (it is a unique object), 
 * otherwise they will use the print function with lid parameter.
 * kind: object kind
 * state: state of object
 ***************************/
void Printer::print( Kind kind, char state ) {
    SingleBuff& buff = arrayOfBuff[(int)kind];
    // flush if buffer not empty
    if ( !buff.empty ) { flush(); }
    buff.state = state;
    buff.empty = false;
}

/********* Printer::print *********
 * Called by objects when they want to print their state.
 * The kind of the object corresponds to it index (it is a unique object), 
 * otherwise they will use the print function with lid parameter.
 * kind: object kind
 * state: state of object
 * value1: specific information
 ***************************/
void Printer::print( Kind kind, char state, unsigned int value1 ) {
    SingleBuff& buff = arrayOfBuff[(int)kind];
    // flush if buffer not empty
    if ( !buff.empty ) { flush(); }
    buff.state = state;
    buff.value1 = value1;
    buff.empty = false;
}

/********* Printer::print *********
 * Called by objects when they want to print their state.
 * The kind of the object corresponds to it index (it is a unique object), 
 * otherwise they will use the print function with lid parameter.
 * kind: object kind
 * state: state of object
 * value1: specific information
 * value2: specific information
 ***************************/
void Printer::print( Kind kind, char state, unsigned int value1, unsigned int value2 ) {
    SingleBuff& buff = arrayOfBuff[(int)kind];
    // flush if buffer not empty
    if ( !buff.empty ) { flush(); }
    buff.state = state;
    buff.value1 = value1;
    buff.value2 = value2;
    buff.empty = false;
}

/********* Printer::print *********
 * Called by objects when they want to print their state.
 * kind: object kind
 * lid: local id
 * state: state of object
 ***************************/
void Printer::print( Kind kind, unsigned int lid, char state ) {
    unsigned int index = getIndex( kind, lid ); // get index using lid
    SingleBuff& buff = arrayOfBuff[index];
    // flush if buffer not empty
    if ( !buff.empty ) { flush(); }
    buff.state = state;
    buff.empty = false;
}

/********* Printer::print *********
 * Called by objects when they want to print their state.
 * kind: object kind
 * lid: local id
 * state: state of object
 * value1: specific information
 ***************************/
void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1 ) {
    unsigned int index = getIndex( kind, lid ); // get index using lid
    SingleBuff& buff = arrayOfBuff[index];
    // flush if buffer not empty
    if ( !buff.empty ) { flush(); }
    buff.state = state;
    buff.value1 = value1;
    buff.empty = false;
}

/********* Printer::print *********
 * Called by objects when they want to print their state.
 * kind: object kind
 * lid: local id
 * state: state of object
 * value1: specific information
 * value2: specific information
 ***************************/
void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 ) {
    unsigned int index = getIndex( kind, lid ); // get index using lid
    SingleBuff& buff = arrayOfBuff[index];
    // flush if buffer not empty
    if ( !buff.empty ) { flush(); }
    buff.state = state;
    buff.value1 = value1;
    buff.value2 = value2;
    buff.empty = false;
}

/********* Printer::~Printer *********
 * This is the destructor of the Printer class.
 * It prints the footer.
 ***************************/
Printer::~Printer() {
    flush();
    cout << "***********************" << endl;
}