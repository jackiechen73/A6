#ifndef STUDENT_H
#define STUDENT_H
_Task Student {
    Printer& printer;
    NameServer& nameServer;
    WATCardOffice& cardOffice;
    GroupOff& groupoff;
    unsigned int id;
    unsigned int maxPurchases;
	void main();
  public:
	Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases );
};
#endif
