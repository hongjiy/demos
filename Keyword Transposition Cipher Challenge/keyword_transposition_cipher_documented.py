# Author Hongji Yang, 2018
# Read input from STDIN. Print output to STDOUT
import sys

alphabet = ['A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z']




# ******************************************************************************
# 
#    Method: shift_char(char, alphabet)
#    Description:
#     This method shift target charater to the front of the input arrary
# 
#    Parameters: char: target charactor, alphabet: target arrary
# 
#    Return value: True: Shift successful
#                  False: Shift failed
# *****************************************************************************

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


# ******************************************************************************
# 
#    Method: bubble(bad_matrix)
#    Description:
#     This method ascending bubble sort input matrix column-wise
# 
#    Parameters: bad_matrix: a matrix of any size
# 
#    Return value: bad_matrix: sorted matrix
# *****************************************************************************
    
def bubble(bad_matrix):
    length = len(bad_matrix[0][:]) - 1
    height = len(bad_matrix)
    
    
    sorted = False

    while not sorted:
        sorted = True
        for i in range(length):

            
            if bad_matrix[0][i] > bad_matrix[0][i+1]:
                sorted = False
                
                for row in range(0, height):
                
                    bad_matrix[row][i], bad_matrix[row][i+1] = bad_matrix[row][i+1], bad_matrix[row][i]     

    return bad_matrix


# ******************************************************************************
# 
#    Method: rotate(bad_matrix)
#    Description:
#     This method rotate a matrix clock-wise by 90 degrees
# 
#    Parameters: bad_matrix: a matrix of any size
# 
#    Return value: bad_matrix: rotated matrix
# *****************************************************************************

def rotate(bad_matrix):
    
    
    length = len(bad_matrix[0][:])
    height = len(bad_matrix)
    
    
    new_matrix = [([-1] * height) for y in range (0, length)]
    
    for index_x in xrange(0, height):
        for index_y in xrange(0, length):
           
            new_matrix[index_y][index_x] = bad_matrix[index_x][index_y]
            

    return new_matrix


# ******************************************************************************
# 
#    Method: strip_zeros(bad_matrix)
#    Description:
#     This method strip all the zeros in the matrix and put all the rows in one
#     output array
# 
#    Parameters: bad_matrix: a matrix of any size
# 
#    Return value: final_cipher_al: an array contains all the none-zero values
#    in the input matrix
# *****************************************************************************

def strip_zeros(bad_matrix):
    
    length = len(bad_matrix[0][:])
    height = len(bad_matrix)
    
    final_cipher_al = []
    
    for index_x in xrange(0, height):
        for index_y in xrange(0, length):
            
            if bad_matrix[index_x][index_y] == 0:
                
                pass
            
            else:
                
                final_cipher_al.append(chr(bad_matrix[index_x][index_y]))
                
                
    return final_cipher_al
                
            
# ******************************************************************************
# 
#    Method: creat_cipher_alphabet(key)
#    Description:
#     This method creates a cipher alphabet with input key
# 
#    Parameters: key: Key to determine characters in the cipher alphabet
# 
#    Return value: cipher_alphabet: New cipher alphabet
# *****************************************************************************              
    
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
    

    for index_x in xrange(0, height):
        for index_y in xrange(0, width):
            
            matrix[index_x][index_y] = ord(cipher_alphabet[index_x * key_len + index_y])
            
            if (index_x * key_len + index_y) == al_len - 1:
                break
             
                
        if (index_x * key_len + index_y) == al_len - 1:
            break
    
    matrix = bubble(matrix)

    
    
    matrix = rotate(matrix)
    

    cipher_alphabet = strip_zeros(matrix)
    
    
    for index in xrange(0,len(cipher_alphabet)):
        
        if index % key_len == 0:
            pass
            
        
    return cipher_alphabet


# ******************************************************************************
# 
#    Method: create_new_dictionary(key)
#    Description:
#     This method creates a dictionary denoting cipher alphabet to alphabet
# 
#    Parameters: key: Key to determine charactor order in the cipher alphabet
# 
#    Return value: new_dictionary: New cipher dictionary
# *****************************************************************************     

def create_new_dictionary(key):
    
    global alphabet
    cipher_alphabet = creat_cipher_alphabet(key)
    
    new_dictionary = {}
        
    for index in xrange(0, len(alphabet)):
    
        new_dictionary[cipher_alphabet[index]] =  alphabet[index]
        
    return new_dictionary


# ******************************************************************************
# 
#    Method: translate(cipher, dictionary)
#    Description:
#     This method translate cipher text to plain text
# 
#    Parameters: cipher: cipher text, dictionary: cipher alphabet to alphabet
#    dictionary
# 
#    Return value: plain text
# ***************************************************************************** 

def translate(cipher, dictionary):
    
    decipher = []
    
    for index in xrange(0, len(cipher)):
    
    
        if cipher[index] == ' ':
            
            decipher.append(' ')
            continue

        decipher.append(dictionary[cipher[index]])
        

    decipher.append('\n')

    plain_text = ''.join(decipher)
        
    return plain_text

# ******************************************************************************
# 
#    Method: main()
#    Description:
#     This is the main function that performs decription
# 
#    Parameters: None
# 
#    Return value: None
# ***************************************************************************** 

    
def main():
    
    global alphabet
    
    line = sys.stdin.readline()         
    number_of_cases = int(line)
    
    
    for case in range(number_of_cases):

    
        key = sys.stdin.readline()   
        cipher = sys.stdin.readline()   
        
    
        new_dictionary = create_new_dictionary(key.rstrip())
        decipher = translate(cipher.rstrip(), new_dictionary)
        sys.stdout.write(decipher)   
  
    
    
if __name__ == '__main__':
    
    main()