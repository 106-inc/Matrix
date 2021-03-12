.PHONY: all clean

RELEASE_OPTIONS = -O2
WRNNG_OPTNS = -Wall -Wextra
CXX = g++ --std=c++2a

FB_BLD = FB_BLD
BLD = BLD_DIR

FNB = grammar
DRVR = driver
PSR = parser


all: start

start: bld_dir fb_bld bison flex parser.o driver.o circuits.o
	$(CXX) -o $(BLD)/curc main.cc $(BLD)/lexer.o $(BLD)/compiler.o $(BLD)/parser.o $(BLD)/driver.o $(BLD)/circuits.o

clean:
	rm -rf $(BLD) $(FB_BLD)

bld_dir:
	mkdir -p $(BLD)

fb_bld:
	mkdir -p $(FB_BLD)

flex: $(FNB)/lexer.l
	flex --c++ -o $(FB_BLD)/lex.yy.cc $(FNB)/lexer.l
	$(CXX) -c -o $(BLD)/lexer.o $(FB_BLD)/lex.yy.cc

bison: $(FNB)/compiler.y
	bison -d -Wcounterexamples -o $(FB_BLD)/compiler.tab.cc $(FNB)/compiler.y
	$(CXX) -c -o $(BLD)/compiler.o $(FB_BLD)/compiler.tab.cc

driver.o: $(DRVR)/driver.cc
	$(CXX) $(WRNNG_OPTNS) -c -o $(BLD)/driver.o $(DRVR)/driver.cc

parser.o: $(PSR)/parser.cc
	$(CXX) $(WRNNG_OPTNS) -c -o $(BLD)/parser.o $(PSR)/parser.cc

circuits.o: circuits.cc
	$(CXX) $(WRNNG_OPTNS) -c -o $(BLD)/circuits.o circuits.cc

main.o: main.cc
	$(CXX) $(WRNNG_OPTNS) -c -o $(BLD)/main.o main.cc



