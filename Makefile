CXX = u++					# compiler
CXXFLAGS = -g -multi -O2 -Wall -Wextra -MMD
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = printer.o config.o soda.o#bank.o bottlingPlant.o config.o groupoff.o nameServer.o parent.o printer.o student.o soda.o truck.o vendingMachine.o WATCard.o WATCardOffice.o # list of object files
EXEC = soda

DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"

#############################################################

.PHONY : all clean

all : ${EXEC}					# build all executables

${EXEC} : ${OBJECTS}				# link step 1st executable
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.o *.d ${OBJECTS} ${EXEC}
