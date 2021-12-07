#ifndef WATCARDOFFICE_H
#define WATCARDOFFICE_H
#include "printer.h"
#include "bank.h"
#include "WATCard.h"
#include "MPRNG.h"
#include <deque>
#include <memory.h>

extern MPRNG mprng;     // access MPRNG object from program main

/***** WATCardOffice *****
 * The WATCardOffice manages the WATCards.
 * It is an administrator task used by students to transfer funds 
 * from their bank account to their WATCard.
 * It creates and deletes couriers which withdraw money from the bank 
 * and deposits it into the WATCards.
 ****************************/
_Task WATCardOffice {
        /***** WATCardOffice::Job *****
         * The Job object represents a queued task 
         * that is to be completed by the couriers.
         ****************************/
        struct Job { // marshalled arguments and return future
            unsigned int sid;       // student id
            unsigned int amount;    // amount to be transfered
            WATCard * card;         // destination WATCard
            WATCard::FWATCard result;           // return future
            Job( unsigned int sid, unsigned int amount, WATCard * card );
        }; // Job

        /***** WATCardOffice::Courier *****
         * The Courier is a worker task which transfers funds from the bank 
         * into a student's WATCard.
         ****************************/
        _Task Courier { 
            unsigned int id;            // courier id
            WATCardOffice * cardOffice; // shared WATCard office
            Printer & printer;          // shared printer
            Bank & bank;                // shared bank
            void main();                // task main
        public:
            Courier(unsigned int id, WATCardOffice * cardOffice, Printer& printer, Bank& bank);
        }; // courier		 			// communicates with bank

        Printer& printer;               // shared printer
        Bank& bank;                     // shared bank
        unsigned int numCouriers;       // number of couriers
        Courier** couriers;             // array of couriers
        std::deque<Job *> jobs;         // queue of jobs

        unsigned int sid, amount; // student id and amount (communication variables)
        
        void main();                    // task main
    public:
        _Event Lost {};							// lost WATCard
        WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
        WATCard::FWATCard create( unsigned int sid, unsigned int amount );
        WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card );
        Job * requestWork(); // courier requesting work
        ~WATCardOffice();
}; // WATCardOffice

#endif // WATCARDOFFICE_H
