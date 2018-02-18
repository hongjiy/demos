#Author Hongji Yang 02/2018

import sys

if __name__ == "__main__":
    n, m = raw_input().strip().split(' ')
    n, m = [int(n), int(m)]
    
    array = [0] * (n + 1)
    new_array = []
    
    for a0 in xrange(m):
        a, b, k = raw_input().strip().split(' ')
        a, b, k = [int(a), int(b), int(k)]
        
        array[a - 1] += k
        array[b] -= k
        
    for i in xrange(0, n + 1):
        
        if not array[i] == 0:
            
            new_array.append(array[i])
    
    final_len = len(new_array)
    final_array = [0] * final_len
    max_num = 0
    sum_num = 0
 
    for i in xrange(0, final_len):
        
        sum_num += new_array[i]
        if max_num < sum_num:
        
            max_num = sum_num     
        
    print max_num