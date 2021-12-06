#ifndef BANK_H
#define BANK_H

_Monitor Bank {
    private:
        unsigned int numStudents;       // number of students
        unsigned int * balances;       // balance for each student
        uCondition * withdrawBench;    // block when you cannot withdraw 
    public:
        Bank( unsigned int numStudents );
        ~Bank();
        void deposit( unsigned int id, unsigned int amount );
        void withdraw( unsigned int id, unsigned int amount );
};

#endif