from src.fairplay_src import fairPlay, key_matrix

# KEY: NHUGIOTDBKACEFLMPQRSVWXYZ
# TEXT: TOITHICHHOCCHUONGTRINHIOT

"""
Trong bài toán này thì em xin quy định input như sau:
    1. Key có thể được nhập trong từng bài toán chứ không cố định
    2. Text cũng có thể nhập trong từng bài toán

"""



def main():
    init_key = input("Enter the key: ")
    init_text = input("Enter the text: ")
    
    process = fairPlay(init_text, init_key)

    print("The encrypted text is: ", process.encrypt())

if __name__ == "__main__":
    main()