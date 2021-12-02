#include "config.h"
#include "MPRNG.h"
#include "printer.h"
#include <iostream>
#include <string>

using namespace std;
MPRNG mprng; // MPRNG object for random number generation

int main(int argc, char* argv[]) {
	// default parameters
    int processors = 1;     // number of processors
    int seed = getpid();    // mprng seed
    string configFile = "soda.config";      // config file name
    try {
        switch ( argc ) {
            case 4:
                if ( strcmp( argv[3], "d") != 0 ) { // default?
                    processors = stoi( argv[3] ); if ( processors <= 0 ) throw 1;
                } // if
            case 3:
                if ( strcmp( argv[2], "d") != 0 ) { // default?
                    seed = stoi( argv[2] ); if ( seed <= 0 ) throw 1;
                } // if
            case 2:
                if ( strcmp( argv[1], "d") != 0 ) { // default?
                    configFile = argv[1];
                } // if
            case 1: break; // use all defaults
            default: throw 1;
        } // switch
    } catch ( ... ) {
        cerr << "Usage: " << argv[0] << " [ config-file | 'd' [ seed (> 0) | 'd' [ processors (> 0) | 'd' ] ] ]" << endl;
        exit( EXIT_FAILURE );       // terminate program
    } // try

    // call processConfigFile
    ConfigParms configParams;
    processConfigFile(configFile.c_str(), configParams);
    /*
    cout << configParams.sodaCost << endl;
    cout << configParams.numStudents << endl;
    cout << configParams.maxPurchases << endl;
    cout << configParams.numVendingMachines << endl;
    cout << configParams.maxStockPerFlavour << endl;
    cout << configParams.maxShippedPerFlavour << endl;
    cout << configParams.timeBetweenShipments << endl;
    cout << configParams.groupoffDelay << endl;
    cout << configParams.parentalDelay << endl;
    cout << configParams.numCouriers << endl;
    */

    mprng.set_seed( seed );         // initialize MPRNG 

	// create additional processors
	uProcessor p[processors - 1];   // number of kernel threads
	if ( processors == 1 ) uThisProcessor().setPreemption( 0 ); // turn off time-slicing for reproducibility
    
    Printer printer( configParams.numStudents, configParams.numVendingMachines, configParams.numCouriers );
    /*
        printer, bank, parent, WATCard office, groupoff, name server,
        vending machines, bottling plant, and students. 
        The truck is created by the bottling plant; the couriers are created by
        the WATCard office.
    */
   
    /*
        The program terminates once all of the students have purchased their specified number of bottles.
        Note, there is one trick in closing down the system: delete the bottling plant before deleting the vending machines to
        allow the truck to complete its final deliveries to the vending machines; otherwise, a deadlock can occur.
    */
}
