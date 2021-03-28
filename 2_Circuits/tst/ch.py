r = 1.0

print('1 -- 2, 0; 100V')

for i in range(1, 100, 2):
    print(f'{i} -- {i+2}, 0;')
    print(f'{i+1} -- {i+3}, {r};')
    print(f'{i+2} -- {i+3}, {r};')
