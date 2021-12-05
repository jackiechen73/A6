#ifndef WATCARDOFFICE_H
#define WATCARDOFFICE_H
#include <deque.h>
#include <memory.h>

_Task WATCardOffice {
	struct Job {							// marshalled arguments and return future
		unsigned int sid;
        unsigned int amount;
        WATCard::FWATCard result;			// return future
		Job( unsigned int sid, unsigned int amount ) : sid(sid), amount(amount) {}
	};

	_Task Courier { 
        unsigned int id;
        WATCardOffice * cardOffice;
        Printer& printer;
        Bank& bank;
        void main();
      public:
        Courier(unsigned int id, WATCardOffice * cardOffice, Printer& printer, Bank& bank);
    };					// communicates with bank

    std::deque<Job *> jobs;
    Printer& printer;
    Bank& bank;
    unsigned int numCouriers;
    Courier** couriers;

    unsigned int sid, amount; 
	void main();
  public:
	_Event Lost {};							// lost WATCard
	WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
	WATCard::FWATCard create( unsigned int sid, unsigned int amount );
	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card );
	Job * requestWork();
    ~WATCardOffice();
};

#endif
