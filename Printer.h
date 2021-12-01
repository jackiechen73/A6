#ifndef PRINTER_H
#define PRINTER_H

_Monitor / _Cormonitor Printer {
    struct SingleBuff { 
        bool empty = true;          // is the buffer empty?
        char state;                 
        unsigned int value1;
        unsigned int value2;
    }; // SingleBuff
    unsigned int numStud;
    unsigned int numMach;
    unsigned int numCour;
    std::unique_ptr<SingleBuff []> arrayOfBuff; 

    unsigned int getIndex( Kind kind, unsigned int lid );
    void printValue1( SingleBuff& buff );
    void printValue2( SingleBuff& buff );
    void flush();
    void printTabs( SingleBuff& buff, int& numMissingTabs );
    void printUnique( SingleBuff& buff, Kind kind);
    void printStudBuff( SingleBuff& buff );
    void printMachBuff( SingleBuff& buff );
    void printCourBuff( SingleBuff& buff );
    
  public:
	enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
	Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
	void print( Kind kind, char state );
	void print( Kind kind, char state, unsigned int value1 );
	void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
	void print( Kind kind, unsigned int lid, char state );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );
};

#endif