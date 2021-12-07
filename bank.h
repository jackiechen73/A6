#ifndef BANK_H
#define BANK_H

/***** Bank *****
 * The bank manages the balance of each students' account.
 * Parents deposit money to students.
 * Couriers withdraw money  for the WATCard office. 
 ****************************/
_Monitor Bank {
    private:
        unsigned int numStudents;       // number of students
        unsigned int * balances;        // balance for each student
        uCondition * withdrawBench;     // block when you cannot withdraw 
    public:
        Bank( unsigned int numStudents );   // constructor
        ~Bank();                            // destructor
        void deposit( unsigned int id, unsigned int amount ); // deposit amount
        void withdraw( unsigned int id, unsigned int amount ); // withdraw amount
};

#endif // BANK_H