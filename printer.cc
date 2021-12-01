#include "Printer.h"
#include <iostream>         // access cout

#define NUM_UNIQUE  5

using namespace std;        // direct access to std

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers )
: numStud( numStudents ), numMach( numVendingMachine ), numCour( numCouriers ), 
arrayOfBuff( new SingleBuff[NUM_UNIQUE + numStudents + numVendingMachines + numCouriers] ) {
    cout << "Parent\tGropoff\t WATOff\tNames\tTruck\tPlant";
    for ( unsigned int i = 0; i < numStud; i += 1 ) {
        cout << "\tStud" << i;
    } // for
    for ( unsigned int i = 0; i < numMach; i += 1 ) {
        cout << "\tMach" << i;
    } // for
    for ( unsigned int i = 0; i < numCour; i += 1 ) {
        cout << "\tCour" << i;
    } // for
    cout << endl;
    for ( unsigned int i = 0; i < NUM_UNIQUE + numStud + numMach + numCour - 1; i += 1 ) {
        cout << "*******\t";
    } // for
    cout << "*******" << endl;
}

unsigned int Printer::getIndex( Kind kind, unsigned int lid ) {
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

void Printer::printValue1( SingleBuff& buff ) {
    cout << buff.value1;
}

void Printer::printValue2( SingleBuff& buff ) {
    cout << buff.value1 << "," << buff.value2;
}

void Printer::flush() {
    unsigned int i = 0;
    int numMissingTabs = 0
    for ( ; i < NUM_UNIQUE; i += 1 ) {
        printTabs( arrayOfBuff[i], numMissingTabs );
        printUnique( arrayOfBuff[i], i );
    } // for
    for ( ; i < numStud; i += 1 ) {
        printTabs( arrayOfBuff[i], numMissingTabs );
        printStudBuff( arrayOfBuff[i] );
    } // for
    for ( ; i < numMach; i += 1 ) {
        printTabs( arrayOfBuff[i], numMissingTabs );
        printMach( arrayOfBuff[i] );
    } // for
    for ( ; i < numCour; i += 1 ) {
        printTabs( arrayOfBuff[i], numMissingTabs );
        printCourBuff( arrayOfBuff[i] );
    } // for
    cout << endl;
}

void Printer::printTabs( SingleBuff& buff, int& numMissingTabs ) {
    if ( buff.empty ) { // buffer is empty?
        numMissingTabs += 1; 
        return; 
    } // if
    // print number of missing tabs
    for ( int i = 0; i < numMissingTabs; i += 1) {
        cout << "\t";
    } // for
    numMissingTabs = 0; // reset number of missing tabs
}

void Printer::printUnique( SingleBuff& buff, Kind kind) {
    if ( buff.empty ) { return; } // if
    cout << buff.state;
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
    buff.empty = true;
}

void Printer::printStudBuff( SingleBuff& buff ) {
    if ( buff.empty ) { return; } // if
    cout << buff.state;
    if ( buff.state == 'V' ) {
        printValue1( buff );
    } else if ( buff.state == 'S' || buff.state == 'G' || buff.state == 'a' 
                || buff.state == 'B' || buff.state == 'A' ) {
        printValue2( buff );
    }
    buff.empty = true;
}

void Printer::printMachBuff( SingleBuff& buff ) {
    if ( buff.empty ) { return; } // if
    cout << buff.state;
    if ( buff.state == 'S' ) {
        printValue1( buff );
    } else if ( buff.state == 'B' ) {
        printValue2( buff );
    }
    buff.empty = true;
}

void Printer::printCourBuff( SingleBuff& buff ) {
    if ( buff.empty ) { return; } // if
    cout << buff.state;
    if ( buff.state == 'L' ) {
        printValue1( buff );
    } else if ( buff.state == 't' || buff.state == 'T' ) {
        printValue2( buff );
    }
    buff.empty = true;
}

void Printer::print( Kind kind, char state ) {
    SingleBuff& buff = arrayOfBuff[(int)kind];
    // flush if buffer not empty
    if ( !buff.empty ) { flush(); } // if
    buff.state = state;
    buff.empty = false;
}
void Printer::print( Kind kind, char state, unsigned int value1 ) {
    SingleBuff& buff = arrayOfBuff[(int)kind];
    // flush if buffer not empty
    if ( !buff.empty ) { flush(); } // if
    buff.state = state;
    buff.value1 = value1;
    buff.empty = false;
}
void Printer::print( Kind kind, char state, unsigned int value1, unsigned int value2 ) {
    SingleBuff& buff = arrayOfBuff[(int)kind];
    // flush if buffer not empty
    if ( !buff.empty ) { flush(); } // if
    buff.state = state;
    buff.value1 = value1;
    buff.value2 = value2;
    buff.empty = false;
}
void Printer::print( Kind kind, unsigned int lid, char state ) {
    unsigned int index = getIndex( kind, lid );
    SingleBuff& buff = arrayOfBuff[index]
    // flush if buffer not empty
    if ( !buff.empty ) { flush(); } // if
    buff.state = state;
    buff.empty = false;
}
void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1 ) {
    unsigned int index = getIndex( kind, lid );
    SingleBuff& buff = arrayOfBuff[index]
    // flush if buffer not empty
    if ( !buff.empty ) { flush(); } // if
    buff.state = state;
    buff.value1 = value1;
    buff.empty = false;
}
void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 ) {
    unsigned int index = getIndex( kind, lid );
    SingleBuff& buff = arrayOfBuff[index]
    // flush if buffer not empty
    if ( !buff.empty ) { flush(); } // if
    buff.state = state;
    buff.value1 = value1;
    buff.value2 = value2;
    buff.empty = false;
}