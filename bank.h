#ifndef BANK_H
#define BANK_H

#include <memory>

_Monitor Bank {
    std::unique_ptr<unsigned int []> balances;
    std::unique_ptr<uCondition []> withdrawBench;
    unsigned int numStudents; 
  public:
	Bank( unsigned int numStudents );
	void deposit( unsigned int id, unsigned int amount );
	void withdraw( unsigned int id, unsigned int amount );
};

#endif