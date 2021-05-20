# 2 level
Program for calculating electricical
circuits.

## Install
### Requirements
- flex
- bison (3.7 or higher)
### Installation
After cloning, in root directory of a repo just do:
```bash
mkdir build
cd build
make 
cd ..
```
After that, in your current directory, you will see an executable `circ`.
Examples are located in `./tst/in/` directory.

## Usage
### Input format
Input file in format:
```
i -- j[, R]; [EDS V]
```
Where 
 * `i, j` - are the numbers of adjacent verticies of a circuit
* `R` - resistance of an edge `i -- j` in Ohms
* `EDS` - value of an eds in volts of an edge `i -- j`

### Output format 
Program writes answer in format:
```
i -- j : I A
```
Where `I` - value of a current of an edge `i -- j`.

### Execution
Program reads from stdin and writes to stdout.
```bash
./circ < FILE
```
