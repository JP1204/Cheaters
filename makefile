#makefile

CPP  = g++
RES  =
OBJ  = hash.o driver.o $(RES)
LINKOBJ  = hash.o driver.o $(RES) 
BIN  = plagiarismCatcher 
CXXFLAGS = $(CXXINCS)
CFLAGS = $(INCS)
RM = rm -f


all: plagiarismCatcher 

clean:
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o plagiarismCatcher $(LIBS)

hash.o: hash.cpp
	$(CPP) -c hash.cpp -o hash.o $(CXXFLAGS)

driver.o: driver.cpp
	$(CPP) -c driver.cpp -o driver.o $(CXXFLAGS)
