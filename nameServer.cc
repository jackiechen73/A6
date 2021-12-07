#include "nameServer.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents )
: printer(prt), numVendingMachines(numVendingMachines), numStudents(numStudents), 
studentAssignments( new unsigned int[numStudents]), machineList(new VendingMachine*[numVendingMachines]) {
    // assign each student a vending machine
    for (unsigned int i = 0; i < numStudents; i += 1) {
        studentAssignments[i] = i % numVendingMachines;
    } // for
}

NameServer::~NameServer() {
    delete [] studentAssignments;
    // machines will already be terminated
    delete [] machineList;
}

void NameServer::main() {
    printer.print(Printer::NameServer, 'S'); // start
    for (unsigned int i = 0 ; i < numVendingMachines; i += 1 ) {
        _Accept(VMregister);
        printer.print(Printer::NameServer, 'R', VMidx);
        VMidx += 1;
    } // for
    for (;;) {
        _Accept(~NameServer) {
            break;
        } or _Accept(getMachineList) {
        } or _Accept(getMachine) { 
            printer.print(Printer::NameServer, 'N', studId, studentAssignments[studId]);
            // increment vending machine assignment for next call to getMachine
            studentAssignments[studId] = (studentAssignments[studId] + 1) % numVendingMachines;
        } 
    } // for
    printer.print(Printer::NameServer, 'F'); // finish
}

void NameServer::VMregister( VendingMachine * vendingmachine ) {
    machineList[VMidx] = vendingmachine;
}

VendingMachine * NameServer::getMachine( unsigned int id ) {
    studId = id;
    return machineList[studentAssignments[id]];
}

VendingMachine ** NameServer::getMachineList() {
    return machineList;
}