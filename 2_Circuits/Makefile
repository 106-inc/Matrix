.PHONY: all clean

WRNNG_OPTNS = -Wall -Wextra -Wunused -Wno-old-style-cast -Waggressive-loop-optimizations -Wnon-virtual-dtor -Wuseless-cast
CXX = g++ --std=c++2a -O2 -g

FB_BLD = FB_BLD
BLD = BLD_DIR

FNB = grammar/rules
DRVR = grammar/driver
PSR = grammar/parser
CRC = circuits
LA = lin_alg


all: start

start: bld_dir fb_bld matrix.o circuits.o compiler.o lexer.o parser.o driver.o main.o
	$(CXX) -o $(BLD)/circ $(BLD)/matrix.o $(BLD)/circuits.o $(BLD)/parser.o $(BLD)/lexer.o $(BLD)/compiler.o $(BLD)/driver.o $(BLD)/main.o
	
clean:
	rm -rf $(BLD) $(FB_BLD)

bld_dir:
	mkdir -p $(BLD)

fb_bld:
	mkdir -p $(FB_BLD)

lexer.o: $(FNB)/lexer.l
	flex --c++ -o $(FB_BLD)/lex.yy.cc $(FNB)/lexer.l
	$(CXX) -c -o $(BLD)/lexer.o $(FB_BLD)/lex.yy.cc

compiler.o: $(FNB)/compiler.y
	bison -d -o $(FB_BLD)/compiler.tab.cc $(FNB)/compiler.y
	$(CXX) -c -o $(BLD)/compiler.o $(FB_BLD)/compiler.tab.cc

driver.o: $(DRVR)/driver.cc
	$(CXX) $(WRNNG_OPTNS) -c -o $(BLD)/driver.o $(DRVR)/driver.cc

parser.o: $(PSR)/parser.cc
	$(CXX) $(WRNNG_OPTNS) -c -o $(BLD)/parser.o $(PSR)/parser.cc

circuits.o: circuits/circuits.cc
	$(CXX) $(WRNNG_OPTNS) -c -o $(BLD)/circuits.o $(CRC)/circuits.cc

matrix.o: $(LA)/matrix.cc
	$(CXX) $(WRNNG_OPTNS) -c -o $(BLD)/matrix.o $(LA)/matrix.cc

main.o: main.cc
	$(CXX) $(WRNNG_OPTNS) -c -o $(BLD)/main.o main.cc



