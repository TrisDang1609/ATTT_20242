from .addfunc import case1, case1_return, case2, case2_return, case3_return

class key_matrix:
    def __init__(self, key):
        temp_matrix = {}
        for i in range(0, 5):
            for j in range(0, 5):
                temp_pos = i*5 + j
                temp_matrix[key[temp_pos]] = temp_pos

        self.key_matrix = temp_matrix
    
    def get_encripted(self, raw_text, key):
        key = list(key)
        raw_text = list(raw_text)
        
        if len(raw_text) < 2:
            return ''.join(raw_text)
        result = ''
        if raw_text[0] == raw_text[1]:
            
            result += (raw_text[0] + 'X')
            return self.get_encripted(result, key)  # added return for recursion
        else:
            if case1(self.key_matrix[raw_text[0]], self.key_matrix[raw_text[1]]) == 1:
                
                result += key[case1_return(self.key_matrix[raw_text[0]])]
                result += key[case1_return(self.key_matrix[raw_text[1]])]
        
            elif case2(self.key_matrix[raw_text[0]], self.key_matrix[raw_text[1]]) == 1:
                
                result += key[case2_return(self.key_matrix[raw_text[0]])]
                result += key[case2_return(self.key_matrix[raw_text[1]])]

            else:
                
                temp_a = self.key_matrix[raw_text[0]]
                temp_b = self.key_matrix[raw_text[1]]
                
                result += key[case3_return(temp_a, temp_b)]
                result += key[case3_return(temp_b, temp_a)]
        return result

class fairPlay:
    def __init__(self, init_text, key):
        self.plain_text = init_text
        self.key = key
        self.cipher_text = ''
    
    def encrypt(self):
        init_key_matrix = key_matrix(self.key)

        for i in range(0, len(self.plain_text), 2):
            
            temp = self.plain_text[i:i+2]
            temp = init_key_matrix.get_encripted(temp, self.key)
            self.cipher_text += ''.join(temp)

        if len(self.plain_text) % 2 == 1:
            self.cipher_text += init_key_matrix.get_encripted(self.plain_text[-1] + 'Q', self.key)

        return self.cipher_text
