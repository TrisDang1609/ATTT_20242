class rc4:
    def __init__(self, _s_array, _t_array, lenght_plain_text):
        self.s_array = _s_array
        self.t_array = _t_array
        self.len_plain_text = lenght_plain_text 
        self.result  = []
    
    def rc4_t_array_process(self):
        temp_array = []
        temp_mod = len(self.t_array)
        for i in range(0, len(self.s_array)):
            temp_array.append(self.t_array[i % temp_mod])
        self.t_array = temp_array
        # print(self.t_array)

    def rc4_first_permutation(self):
        j = 0
        len_s_array = len(self.s_array)

        for i in range(0, len_s_array):
            j = ((j + self.s_array[i] + self.t_array[i]) % len_s_array)
            self.s_array[i], self.s_array[j] = self.s_array[j], self.s_array[i]
            print(self.s_array)

    def rc4_process(self):

        self.rc4_t_array_process()
        self.rc4_first_permutation()

        i = j = 0
        len_s_array = len(self.s_array)

        while(True):
            i = (i+1) % len_s_array
            j = ((j + self.s_array[i]) % len_s_array)
            self.s_array[i], self.s_array[j] = self.s_array[j], self.s_array[i]
            self.result.append(self.s_array[(self.s_array[i] + self.s_array[j]) % len_s_array])
            self.len_plain_text -= 1
            if self.len_plain_text == 0:
                break
        return self.result