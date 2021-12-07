#include "nameServer.h"

/***** NameServer::NameServer *****
 * This is the constructor of the NameServer class.
 * It assigns each student a vending machine.
 * printer: shared printer
 * numVendingMachines: number of vending machines
 * numStudents: number of students
 ****************************/
NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents )
: printer( prt ), numVendingMachines( numVendingMachines ), numStudents( numStudents ), 
studentAssignments( new unsigned int[numStudents] ), machineList( new VendingMachine*[numVendingMachines] ) {
    // assign each student a vending machine
    for ( unsigned int i = 0; i < numStudents; i += 1 ) {
        studentAssignments[i] = i % numVendingMachines;
    } // for
}

/***** NameServer::~NameServer *****
 * This is the constructor of the NameServer class.
 * It assigns each student a vending machine.
 * printer: shared printer
 * numVendingMachines: number of vending machines
 * numStudents: number of students
 ****************************/
NameServer::~NameServer() {
    delete [] studentAssignments;
    // machines will already be terminated by soda.cc
    delete [] machineList;
}

/***** NameServer::main *****
 * This is taks main.
 * It starts by accepting all machine registrations.
 * Then, it allows calls to getMachineList and getMachine.
 * getMachineList is used by the truck to know which machines to deliver to.
 * getMachine is used by students to get a new machine.
 ****************************/
void NameServer::main() {
    printer.print( Printer::NameServer, 'S' ); // start
    // wait for all machines to register
    for ( unsigned int i = 0 ; i < numVendingMachines; i += 1 ) {
        _Accept( VMregister );
        // registered machine
        printer.print( Printer::NameServer, 'R', VMidx );
    } // for
    for (;;) {
        _Accept( ~NameServer ) {
            break; // break out of for loop
        } or _Accept( getMachineList ) {
        } or _Accept( getMachine ) { 
            printer.print(
                Printer::NameServer, 
                'N', 
                studId, 
                studentAssignments[studId]
            );
            // increment vending machine assignment for next call to getMachine
            studentAssignments[studId] = ( studentAssignments[studId] + 1 ) % numVendingMachines;
        } 
    } // for
    printer.print( Printer::NameServer, 'F' ); // finish
}

/***** NameServer::VMregister *****
 * Registers the vending machine by adding the machine to the machine list.
 * Vending machines initially call VMregister to register themselves with the nameserver.
 ****************************/
void NameServer::VMregister( VendingMachine * vendingmachine ) {
    VMidx = vendingmachine->getId(); // used in main
    // place machine at index corresponding to id
    machineList[VMidx] = vendingmachine;
}

/***** NameServer::getMachine *****
 * Returns a new machine id for the specified student.
 * id: id of student
 ****************************/
VendingMachine * NameServer::getMachine( unsigned int id ) {
    studId = id; // used in main
    return machineList[studentAssignments[id]];
}

/***** NameServer::getMachineList *****
 * Returns machine list.
 ****************************/
VendingMachine ** NameServer::getMachineList() {
    return machineList;
}