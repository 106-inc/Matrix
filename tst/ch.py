r = 1.0

print('1 -- 2; 100V')

for i in range(1, 50, 2):
    print(f'{i} -- {i+2};')
    print(f'{i+1} -- {i+3}, {r};')
    print(f'{i+2} -- {i+3}, {r};')
