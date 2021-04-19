
#!/usr/bin/env python3
import numpy as np
import argparse
from random import randint

def main():
    parser = argparse.ArgumentParser(description='Genrator of matrix for test')
    parser.add_argument('num_of_mul', metavar='MUL_NUM', type=int, help= 'num of multiplication')
    parser.add_argument('rows1', metavar='ROWS', type=int, help='rows of fst matrix')
    parser.add_argument('cols1', metavar='COLS_N_ROWS', type=int, help='common rows and cols amount')
    parser.add_argument('cols2', metavar='COLS', type=int, help='cols of a sec matrix')

    args = parser.parse_args()


    if args.rows1<= 0 or args.cols1<= 0 or args.cols2<= 0:
        print('Incorrect size: ' + str(args.size) + '\nTry again')
        return

    print(2 * args.num_of_mul)

    tmp_cols = args.cols2
    cur_rows = args.rows1
    cur_cols = args.cols1

    for i in range(args.num_of_mul):
        print(cur_rows, cur_cols)
        print(cur_cols, tmp_cols)
        cur_rows = tmp_cols
        cur_cols = randint(1, 30)
        tmp_cols = randint(1, 30)

       



if __name__ == '__main__':
    main()