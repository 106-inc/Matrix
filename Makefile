.PHONY: all clean

RELEASE_OPTIONS = -O2
WRNNG_OPTNS = -Wall -Wextra
CXX = g++ --std=c++2a

FB_BLD = FB_BLD
BLD = PCL_BLD

FNB = grammar
DRVR = driver
PRSR = parser


all: start

start:

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
	bison -d -o $(FB_BLD)/compiler.tab.cc $(FNB)/compiler.y
	$(CXX) -c -o $(BLD)/compiler.o $(FB_BLD)/compiler.tab.cc

driver.o: $(DRVR)/driver.cc
	$(CXX) $(WRNNG_OPTNS) -c -o $(BLD)/driver.o $(DRVR)/driver.cc

parser.o: $(PSR)/parser.cc
	$(CXX) $(WRNNG_OPTNS) -c -o $(BLD)/parser.o $(PRSR)/parser.cc

main.o: main.cc
	$(CXX) $(WRNNG_OPTNS) -c -o $(BLD)/main.o main.cc



