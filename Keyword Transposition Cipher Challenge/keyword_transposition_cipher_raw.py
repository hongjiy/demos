# Enter your code here. Read input from STDIN. Print output to STDOUT
import sys

alphabet = ['A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z']

def shift_char(char, alphabet):
    
    try:
    
        alphabet

        index_a = alphabet.index(char)

        for index in xrange(index_a, 0, -1):
        
            alphabet[index] = alphabet[index -1]
            
        alphabet[0] = char

        return True

    except:
        
        return False
    
    
def bubble(bad_matrix):
    length = len(bad_matrix[0][:]) - 1
    height = len(bad_matrix)
    
    #print length, height
    
    #print len (bad_matrix)
    #print bad_matrix[3]
    
    #print bad_matrix
    
    sorted = False

    while not sorted:
        sorted = True
        for i in range(length):
            
            #print i
            
            if bad_matrix[0][i] > bad_matrix[0][i+1]:
                sorted = False
                
                #bad_matrix[0][:]
                
                #bad_matrix[:][i], bad_matrix[:][i+1] = bad_matrix[:][i+1], bad_matrix[:][i]
                
                for row in range(0, height):
                
                    #print 'row', row
                    bad_matrix[row][i], bad_matrix[row][i+1] = bad_matrix[row][i+1], bad_matrix[row][i]     
                #temp = bad_matrix[][i+1]
               #bad_matrix[][i+1] = bad_matrix[:][i]
                #bad_matrix[][i] = temp
                
    return bad_matrix

def rotate(bad_matrix):
    
    
    length = len(bad_matrix[0][:])
    height = len(bad_matrix)
    
    
    new_matrix = [([-1] * height) for y in range (0, length)]
    
    for index_x in xrange(0, height):
        for index_y in xrange(0, length):
            
            #print 'index_x, index_y = ', index_x, index_y
            #print  index_x * key_len + index_y, al_len, cipher_alphabet[index_x * key_len + index_y]
            new_matrix[index_y][index_x] = bad_matrix[index_x][index_y]
            
    #print new_matrix
    
    return new_matrix

def strip_zeros(bad_matrix):
    
    length = len(bad_matrix[0][:])
    height = len(bad_matrix)
    
    final_cipher_al = []
    
    for index_x in xrange(0, height):
        for index_y in xrange(0, length):
            
            #print 'index_x, index_y = ', index_x, index_y
            #print  index_x * key_len + index_y, al_len, cipher_alphabet[index_x * key_len + index_y]

            if bad_matrix[index_x][index_y] == 0:
                
                pass
            
            else:
                
                final_cipher_al.append(chr(bad_matrix[index_x][index_y]))
                
                
    return final_cipher_al
                
            
                
    
def creat_cipher_alphabet(key):

    global alphabet

    cipher_alphabet = alphabet[:]
    
    for char in reversed(key):   
      
        shift_char(char, cipher_alphabet)
        
        
    key_len = len(list(set(key)))
    al_len = len(alphabet)
    
    width = key_len
    height = int(al_len/key_len + 1)
    
    matrix = [([0] * width) for y in range (0, height)]
    
    #print matrix
              
    for index_x in xrange(0, height):
        for index_y in xrange(0, width):
            
            #print  index_x * key_len + index_y, al_len, cipher_alphabet[index_x * key_len + index_y]
            matrix[index_x][index_y] = ord(cipher_alphabet[index_x * key_len + index_y])
            
            if (index_x * key_len + index_y) == al_len - 1:
               # print 'loop breaking'
                break
             
                
        if (index_x * key_len + index_y) == al_len - 1:
            #print 'loop breaking'
            break
    #print matrix
    #print matrix[0][:]
    
    matrix = bubble(matrix)
    #print matrix
    
    
    matrix = rotate(matrix)
    
    #print matrix
    
    cipher_alphabet = strip_zeros(matrix)
    
    #ca = chararray(3,3)
    
    for index in xrange(0,len(cipher_alphabet)):
        
        if index % key_len == 0:
            pass
            
        
    #print cipher_alphabet
    return cipher_alphabet

def create_new_dictionary(key):
    
    global alphabet
    cipher_alphabet = creat_cipher_alphabet(key)
    
    new_dictionary = {}
    
    #print cipher_alphabet
    
    #for char in cipher_alphabet:
    
        #new_dictionary[char] = alphabet[cipher_alphabet.index(char)]
        #print new_dictionary[char]
        
    for index in xrange(0, len(alphabet)):
    
        new_dictionary[cipher_alphabet[index]] =  alphabet[index]
        #print cipher_alphabet[alphabet.index(char)
        
        #print cipher_alphabet[index], alphabet[index]
        
    return new_dictionary

def translate(cipher, dictionary):
    
    #decipher = [list(cipher[:])]
    decipher = []
    
    #print 'decipher', decipher
    
   # print 'decipher len = ', len(cipher)
    
    #print decipher[0]
    
    #space = False
    for index in xrange(0, len(cipher)):
    
    
       # print cipher[index]
        if cipher[index] == ' ':
            
            decipher.append(' ')
            continue
        #print type(dictionary[cipher[index]])
        #print dictionary[cipher[index]]
        decipher.append(dictionary[cipher[index]])
        
        #print dictionary[cipher[index]], 'assigned'
        
        #print dictionary[decipher[index]]
    decipher.append('\n')
        
    return ''.join(decipher)

    
def main():
    
    global alphabet
    
    line = sys.stdin.readline()         
    number_of_cases = int(line)
    
    
    for case in range(number_of_cases):
        #print case
    
        key = sys.stdin.readline()   
        cipher = sys.stdin.readline()   
        
        #print type(key), type(cipher)
        #print key
        
        #print len(key)
        
        #print cipher
    
    
    #key = "SECRET"
    #cipher = "CDJOW EBINV RFKPX SAHMUZ TGLQY"
    
        new_dictionary = create_new_dictionary(key.rstrip())

        #print new_dictionary

        decipher = translate(cipher.rstrip(), new_dictionary)
        #print decipher
        
        
        #print decipher
        sys.stdout.write(decipher)   
  
    
    
if __name__ == '__main__':
    
    main()
    #sys.stdout.write('ILOVE SOLVI NGPRO GRAMM INGCH ALLEN GES')