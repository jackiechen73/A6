#include "VendingMachine.h"

VendingMachine::VendingMachine(Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost) :
  printer(prt), nameServer(nameServer), id(id), sodaCost(sodaCost) {}

void VendingMachine::main() {
  
}

void VendingMachine::VendingMachine(Flavour flavour, WATCard & card) {

}

unsigned int *  VendingMachine::inventory() {
  return sodaInventory;
}

void VendingMachine::restocked() {}

unsigned int VendingMachine::cost() const {
  return sodaCost;
}

unsigned int VendingMachine::getId() {
  return id;
}


