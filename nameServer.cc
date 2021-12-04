#include "NameServer.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents )
: printer(prt), numVendingMachines(numVendingMachines), numStudents(numStudents), 
studentAssignments( new unsigned int[numStudents]), machineList(new VendingMachine*[numVendingMachines]) {
    for (unsigned int i = 0; i < numStudents; i += 1) {
        studentAssignments[i] = i % numVendingMachines;
    } // for
}

void NameServer::main() {
    printer.print(Printer::NameServer, 'S');
    for (int i == 0 ; i < numVendingMachines; i += 1 ) {
        _Accept(VMregister);
        VMidx += 1;
    } // for
    for (;;) {
        _Accept(~NameServer) {
            break;
        } or _Accept(getMachine) { 
            printer.print(Printer::NameServer, 'N', studId, studentAssignments[studId]);
            studentAssignments[studId] = (studentAssignments[studId] + 1) % numVendingMachines;
        } or _Accept(getMachineList) {}
    } // for
    printer.print(Printer::NameServer, 'F');
}

void NameServer::VMregister( VendingMachine * vendingmachine ) {
    vendingMachines[VMidx] = vendingmachine;
    printer.print(Printer::NameServer, 'R', vendingmachine->getId());
}

VendingMachine * NameServer::getMachine( unsigned int id ) {
    studId = id;
    return vendingMachines[studentAssignments[id]];
}

VendingMachine ** NameServer::getMachineList() {
    return machineList;
}