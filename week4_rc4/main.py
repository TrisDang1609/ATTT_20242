from src.rc4_class import rc4

def main():
    s_array = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    t_array = [1, 2, 3, 6]
    lenght_plain_text = 6
    result = rc4(s_array, t_array, lenght_plain_text)
    print(result.rc4_process())

if __name__ == '__main__':
    main()