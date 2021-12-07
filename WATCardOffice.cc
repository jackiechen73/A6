#include "WATCardOffice.h"

/***** WATCardOffice::Job::Job *****
 * This is the constructor for the Job object.
 * sid: student id
 * amount: amount to be transfered
 * card: WATCard to transfer funds into
 ****************************/
WATCardOffice::Job::Job( unsigned int sid, unsigned int amount, WATCard * card ) 
: sid( sid ), amount( amount ), card( card ) {}

/***** WATCardOffice::Courier::Courier *****
 * This is the constructor for the Courier object.
 * id: courier id
 * cardOffice: shared WATCard office
 * printer: shared printer
 * bank: shared bank
 ****************************/
WATCardOffice::Courier::Courier( unsigned int id, WATCardOffice * cardOffice, Printer & printer, Bank & bank )
: id( id ), cardOffice( cardOffice ), printer( printer ), bank( bank ) {};

/***** WATCardOffice::Courier::main *****
 * This is the task main for the Courier object.
 * It tries to request work from the WATCardOffice object.
 * Once a nullptr job is received, the courier will terminate.
 ****************************/
void WATCardOffice::Courier::main() {
    printer.print( Printer::Kind::Courier, id, 'S' ); // start
    for (;;) {
        Job * job = cardOffice->requestWork(); // blocked until there is work
        if ( job == nullptr ) break;        // no more jobs
        unsigned int sid = job->sid;        // student id
        unsigned int amount = job->amount;  // amount to be transfered
        WATCard * card = job->card;         // destination WATCard
        if (card == nullptr) { // no WATCard?
            card = new WATCard();
        } // if
        // start transfer 
        printer.print(Printer::Kind::Courier, id, 't', sid, amount);
        bank.withdraw( sid, amount ); // withdraw new amount from bank
        if ( mprng(5) == 0 ) { // lost card?
            job->result.exception( new Lost() );
            delete card; // delete newly created card
            printer.print(Printer::Courier, id, 'L', sid); // lost card
        } else {
            card->deposit( amount );
            job->result.delivery( card );
            // transfer complete
            printer.print( Printer::Kind::Courier, id, 'T', sid, amount );
        } // if
        delete job;
    } // for
    printer.print( Printer::Kind::Courier, id, 'F' ); // finish
}

/***** WATCardOffice::WATCardOffice *****
 * This is the constructor for the WATCardOffice object.
 * printer: shared printer
 * bank: shared bank
 * numCouriers: number of couriers
 ****************************/
WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers )
: printer( prt ), bank( bank ), numCouriers( numCouriers ), couriers( new Courier*[numCouriers] ) {}

/***** WATCardOffice::~WATCardOffice *****
 * This is the destructor for the WATCardOffice object.
 * Couriers are deleted by the WATCardOffice
 ****************************/
WATCardOffice::~WATCardOffice(){
    for ( unsigned int i = 0; i < numCouriers; i++ ) {
        delete couriers[i]; // delete each courier
    } // for
    delete [] couriers;
}

/***** WATCardOffice::main *****
 * This is the task main for the WATCardOffice object.
 * This administrator accepts non-blocking create and transfer calls.
 * When there are jobs, the administrator will accept requestWork calls 
 * from the couriers and assign them a job.
 ****************************/
void WATCardOffice::main() {
    printer.print( Printer::Kind::WATCardOffice, 'S' ); // start
    // create each courier
    for ( unsigned int i = 0; i < numCouriers; i += 1 ) {
        couriers[i] = new Courier( i, this, printer, bank );
    } // for
    for (;;) {
        _Accept( ~WATCardOffice ) {
            // delete all jobs
            while ( !jobs.empty() ) {
                delete jobs.front();
                jobs.pop_front();
            } // while
            // notify couriers about shutdown
            for ( unsigned int i = 0; i < numCouriers; i += 1 ) {
                _Accept(requestWork);
            } // for
            break;
        } or _When ( !jobs.empty() ) _Accept( requestWork ) { 
            // jobs available and requesting work?
            printer.print( Printer::Kind::WATCardOffice, 'W' );
        } or _Accept( create ) { // create call?
            printer.print( Printer::Kind::WATCardOffice, 'C', sid, amount );
        } or _Accept( transfer ) { // transfer call?
            printer.print(Printer::Kind::WATCardOffice,'T', sid, amount );
        }
    } // for
    printer.print( Printer::Kind::WATCardOffice, 'F' ); // finish
}

/***** WATCardOffice::create *****
 * Create a new WATCard for the specified student with the specified amount.
 * Returns a FWATCard which will be updated with the WATCard 
 * once the transaction is complete.
 * sid: student id
 * amount: amount to be transfered
 ****************************/
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    WATCardOffice::sid = sid;                       // used in main
    WATCardOffice::amount = amount;                 // used in main
    Job * job = new Job( sid, amount, nullptr );    // make new job
	jobs.push_back( job );                          // add job to queue
	return job->result;                             // return watcard future
}

/***** WATCardOffice::transfer *****
 * Transfer specified amount into specified WATCard.
 * Returns a FWATCard which will be updated with the WATCard 
 * once the transaction is complete.
 * sid: student id
 * amount: amount to be transfered
 * card: destination WATCard
 ****************************/
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
    WATCardOffice::sid = sid;                   // used in main
    WATCardOffice::amount = amount;             // used in main
    Job * job = new Job( sid, amount, card );   // make new job
	jobs.push_back( job );                      // add job to queue
	return job->result;                         // return watcard future
}

/***** WATCardOffice::requestWork *****
 * This is a blocking call which is called by couriers to request a new job.
 * It returns the next job from the queue once a job is available.
 ****************************/
WATCardOffice::Job * WATCardOffice::requestWork() {
    if ( jobs.size() == 0 ) return nullptr; // no more jobs (sentinel value)
    Job * job = jobs.front();               // get first job
    jobs.pop_front();                       // remove job from queue
    return job;
}