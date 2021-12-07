#include "bank.h"
#include "bottlingPlant.h"
#include "config.h"
#include "groupoff.h"
#include "nameServer.h"
#include "parent.h"
#include "printer.h"
#include "student.h"
#include "vendingMachine.h"
#include "WATCardOffice.h"
#include "MPRNG.h"
#include <iostream>
#include <string>

using namespace std; // direct access to std namespace
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
    ConfigParms configParms;
    processConfigFile(configFile.c_str(), configParms);

    mprng.set_seed( seed );         // initialize MPRNG 

    // create additional processors
    uProcessor p[processors - 1];   // number of kernel threads
    if ( processors == 1 ) uThisProcessor().setPreemption( 0 ); // turn off time-slicing for reproducibility
    
    /*
        printer, bank, parent, WATCard office, groupoff, name server,
        vending machines, bottling plant, and students. 
        The truck is created by the bottling plant; the couriers are created by
        the WATCard office.
    */
    Printer printer( configParms.numStudents, configParms.numVendingMachines, configParms.numCouriers );
    Bank bank(configParms.numStudents);
    Parent parent(printer, bank, configParms.numStudents, configParms.parentalDelay);
    WATCardOffice cardoffice(printer, bank, configParms.numCouriers);
    // couriers created by WATCardOffice
    Groupoff groupoff(printer, configParms.numStudents, configParms.sodaCost, configParms.groupoffDelay);
    NameServer nameserver(printer, configParms.numVendingMachines, configParms.numStudents);
    
    VendingMachine * VMs[configParms.numVendingMachines];
    // generate each vending machine
    for (unsigned int i = 0; i < configParms.numVendingMachines; i += 1) {
        VMs[i] = new VendingMachine(printer, nameserver, i, configParms.sodaCost);
    } // for

    BottlingPlant * bottlingplant = new BottlingPlant(printer, nameserver, configParms.numVendingMachines, 
        configParms.maxShippedPerFlavour, configParms.maxStockPerFlavour, configParms.timeBetweenShipments);
    // truck created by bottlingplant

    Student * students[configParms.numStudents];
    // generate each student
    for (unsigned int i = 0; i < configParms.numStudents; i += 1 ) {
        students[i] = new Student(printer, nameserver, cardoffice, groupoff, i, configParms.maxPurchases);
    } // for    
   
    /*
        The program terminates once all of the students have purchased their specified number of bottles.
        Note, there is one trick in closing down the system: delete the bottling plant before deleting the vending machines to
        allow the truck to complete its final deliveries to the vending machines; otherwise, a deadlock can occur.
    */
    // delete students
    for (unsigned int i = 0; i < configParms.numStudents; i++) {
        delete students[i];
    } // for
    delete bottlingplant; // delete bottling plant
    // delete vending machines
    for (unsigned int i = 0; i < configParms.numVendingMachines; i++) {
        delete VMs[i];
    } // for
}
