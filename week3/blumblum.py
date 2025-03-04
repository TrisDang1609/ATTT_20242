def blumblum (n, seed):
    return ((n * n) % seed), (n % 2) 

def num_to_bits(num):
    return bin(num)[2:]

def writeToBits(num_bits, init_n, init_seed):
    result = []
    next_n = 0
    bit_raw = 0
    while (len(result) < num_bits):
        next_n, bit_raw = blumblum(init_n, init_seed)
        init_n = next_n
        # print (next_n, bit_raw)
        result.append(num_to_bits(bit_raw))
    # print(result)
    return result

def writeToTxt(result, txt_path):
    with open(txt_path, 'w') as f:
        for item in result:
            f.write("%c" % item)

if __name__ == "__main__":
    num_bits = 1e6
    init_n = 7
    init_seed = 11
    txt_path = "week3/blumblum.txt"
    bitArr = writeToBits(num_bits, init_n, init_seed)
    writeToTxt(bitArr, txt_path)
    print("Done writing to file: ", txt_path)
