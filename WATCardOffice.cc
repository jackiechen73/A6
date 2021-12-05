#include "WATCardOffice.h"

WATCardOffice::Courier::Courier(unsigned int id, WATCardOffice * cardOffice, Printer& printer, Bank& bank)
:id(id), cardOffice(cardOffice), printer(printer), bank(bank) {};

void WATCardOffice::Courier::main() {
    printer.print(Printer::Courier, id, 'S');
    for (;;) {
        Job * job = office->requestWork(); // blocked until there is work
        if ( job == nullptr ) break; // no more jobs
        unsigned int sid = job->sid;
        unsigned int amount = job->amount;
        WATCard * card = new WATCard();
        printer.print(Printer::Courier, id, 't', sid, amount); 
        bank.withdraw( sid, amount ); // withdraw new amount from bank
        if ( mprng(5) == 0 ) { // lost card?
            job->result.exception( new Lost() );
            delete card; // delete newly created card
            printer.print(Printer::Courier, id, 'L', sid);
        } else {
            card->deposit(amount);
            job->result.delivery( card );
            printer.print(Printer::Courier, id, 'T', sid, amount);
        } // if
        delete job;
    } // for
    printer.print(Printer::Courier, id, 'F');
}

WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers )
: printer(prt), bank(bank), numCouriers(numCouriers), couriers(new Couriers*[numCouriers]) {}

WATCardOffice::~WATCardOffice(){
    for(unsigned int i = 0; i < numCouriers; i++){
        delete couriers[i];
    } // for
    delete [] couriers;
}

void WATCardOffice::main() {
    for (int i = 0; i < numCouriers; i += 1) {
        couriers[i] = new Courier(i, &this, printer, bank);
    } // for
    for (;;) {
        _Accept(~WATCardOffice) {
            // notify couriers about shutdown
            for ( int i == 0; i < numCouriers; i += 1 ) {
                _Accept(requestWork);
            } // for
            break;
        } or When (!jobs.empty()) _Accept(requestWork) {
        } or _Accept(create) {
            printer->print(Printer::Kind::WATCardOffice, 'C', sid, amount);
        } or _Accept(transfer) {
            printer->print(Printer::Kind::WATCardOffice,'T', sid,  amount);
        }
    } // for
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
	WATCardOffice::sid = sid;
    WATCardOffice::amount = amount;
    Job * job = new Job(sid, amount);
	jobs.push(job);
	return job->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
    WATCardOffice::sid = sid;
    WATCardOffice::amount = amount;
    Job * job = new Job(sid, amount + card->getBalance());
    delete watcard;
	jobs.push(job);
	return job->result;
}

Job * requestWork() {
    if (jobs.size() == 0) return nullptr;
    Job *job = jobs.front();
    jobs.pop_front();
    return job;
}