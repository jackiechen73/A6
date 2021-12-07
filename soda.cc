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

using namespace std;    // direct access to std namespace
MPRNG mprng;            // MPRNG object for random number generation

/********* ::main *********
 * This is the program main.
 * It processes the command line arguments.
 * It initializes the printer, bank, parent, WATCard office, groupoff, 
 * name server, vending machines, bottling plant, and students.
 * It waits for the students to terminate and then deletes the other objects.
 ***************************/
int main(int argc, char* argv[]) {
	// default parameters
    int processors = 1;     // number of processors
    int seed = getpid();    // mprng seed
    string configFile = "soda.config";      // config file name
    // try to parse the command line arguments
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
    
    // Create printer, bank, parent, WATCard office, groupoff, name server,
    // vending machines, bottling plant, and students.
    Printer printer( configParms.numStudents, configParms.numVendingMachines, configParms.numCouriers );
    Bank bank(configParms.numStudents);
    Parent parent(printer, bank, configParms.numStudents, configParms.parentalDelay);
    WATCardOffice cardoffice(printer, bank, configParms.numCouriers); 
    // couriers created by WATCardOffice
    Groupoff groupoff(printer, configParms.numStudents, configParms.sodaCost, configParms.groupoffDelay);
    NameServer nameserver(printer, configParms.numVendingMachines, configParms.numStudents);
    VendingMachine * VMs[configParms.numVendingMachines]; // array of machines
    for (unsigned int i = 0; i < configParms.numVendingMachines; i += 1) {
        // create each vending machine
        VMs[i] = new VendingMachine(printer, nameserver, i, configParms.sodaCost);
    } // for
    BottlingPlant * bottlingplant = new BottlingPlant(
        printer, nameserver, 
        configParms.numVendingMachines, configParms.maxShippedPerFlavour, 
        configParms.maxStockPerFlavour, configParms.timeBetweenShipments
    ); // truck created by bottlingplant
    Student * students[configParms.numStudents]; // array of students
    for (unsigned int i = 0; i < configParms.numStudents; i += 1 ) {
        // create each student
        students[i] = new Student(printer, nameserver, cardoffice, groupoff, i, configParms.maxPurchases);
    } // for    
   
    // wait for students to terminate
    for (unsigned int i = 0; i < configParms.numStudents; i++) {
        delete students[i];
    } // for
    delete bottlingplant; // delete bottling plant
    // delete vending machines
    for (unsigned int i = 0; i < configParms.numVendingMachines; i++) {
        delete VMs[i];
    } // for
    // destructors of other objects are implicitly called
}
