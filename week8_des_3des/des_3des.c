#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Bit masks for bit manipulation operations
#define BIT_MASK_32   0x00000001           // 32-bit mask to get the lowest bit
#define BIT_MASK_64   0x0000000000000001   // 64-bit mask to get the lowest bit
#define LOWER_32_MASK 0x00000000ffffffff   // Get the lower 32 bits from a 64-bit number
#define UPPER_32_MASK 0xffffffff00000000   // Get the upper 32 bits from a 64-bit number

/* Initial Permutation (IP) Table */
static char IP_Table[] = {
    58, 50, 42, 34, 26, 18, 10,  2, 
    60, 52, 44, 36, 28, 20, 12,  4, 
    62, 54, 46, 38, 30, 22, 14,  6, 
    64, 56, 48, 40, 32, 24, 16,  8, 
    57, 49, 41, 33, 25, 17,  9,  1, 
    59, 51, 43, 35, 27, 19, 11,  3, 
    61, 53, 45, 37, 29, 21, 13,  5, 
    63, 55, 47, 39, 31, 23, 15,  7
};

/* Inverse Initial Permutation (IP^-1) Table */
static char Inverse_IP_Table[] = {
    40,  8, 48, 16, 56, 24, 64, 32, 
    39,  7, 47, 15, 55, 23, 63, 31, 
    38,  6, 46, 14, 54, 22, 62, 30, 
    37,  5, 45, 13, 53, 21, 61, 29, 
    36,  4, 44, 12, 52, 20, 60, 28, 
    35,  3, 43, 11, 51, 19, 59, 27, 
    34,  2, 42, 10, 50, 18, 58, 26, 
    33,  1, 41,  9, 49, 17, 57, 25
};

/* Expansion Table (E) - expands 32-bit to 48-bit */
static char Expansion_Table[] = {
    32,  1,  2,  3,  4,  5,  
     4,  5,  6,  7,  8,  9,  
     8,  9, 10, 11, 12, 13, 
    12, 13, 14, 15, 16, 17, 
    16, 17, 18, 19, 20, 21, 
    20, 21, 22, 23, 24, 25, 
    24, 25, 26, 27, 28, 29, 
    28, 29, 30, 31, 32,  1
};

/* Permutation Function (P) Table - applied after S-Boxes */
static char Permutation_Table[] = {
    16,  7, 20, 21, 
    29, 12, 28, 17, 
     1, 15, 23, 26, 
     5, 18, 31, 10, 
     2,  8, 24, 14, 
    32, 27,  3,  9, 
    19, 13, 30,  6, 
    22, 11,  4, 25
};

/* S-Box Tables - each table converts 6-bit input to 4-bit output */
static char S_Box[8][64] = {{
    /* S1 */
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,  
     0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,  
     4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0, 
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
},{
    /* S2 */
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,  
     3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,  
     0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15, 
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
},{
    /* S3 */
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,  
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,  
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
     1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
},{
    /* S4 */
     7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,  
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,  
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
     3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
},{
    /* S5 */
     2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9, 
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6, 
     4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14, 
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
},{
    /* S6 */
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
     9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
     4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
},{
    /* S7 */
     4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
     1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
},{
    /* S8 */
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
     1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
     7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
     2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
}};

/* Permuted Choice 1 (PC1) Table - converts 64-bit key to 56-bit */
static char PC1_Table[] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    
    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

/* Permuted Choice 2 (PC2) Table - converts 56-bit key to 48-bit for each subkey */
static char PC2_Table[] = {
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

/* Left circular shift schedule for key generation (16 rounds) */
static char Shift_Schedule[] = {
    1, 1, 2, 2, 2, 2, 2, 2, 
    1, 2, 2, 2, 2, 2, 2, 1
};

/*
 * Function to perform the DES algorithm.
 * Parameters:
 *   - data: 64-bit data block (plaintext or ciphertext)
 *   - key: 64-bit initial key
 *   - mode: 'e' for encryption, 'd' for decryption
 *
 * Returns:
 *   - 64-bit result after DES processing.
 */
uint64_t des_algorithm(uint64_t data, uint64_t key, char mode) {
    int i, j;
    char row, col;  // Row and column indices for S-Box lookup
    
    // Variables for key schedule generation
    uint32_t keyLeft = 0;    // Left 28 bits of the key (C)
    uint32_t keyRight = 0;   // Right 28 bits of the key (D)
    
    // Variables for data processing (each half is 32 bits)
    uint32_t L = 0;          // Left half of the data block
    uint32_t R = 0;          // Right half of the data block
    uint32_t sBoxOutput = 0; // Output from S-Boxes (32 bits)
    uint32_t fFunctionResult = 0; // Result of the Feistel function f(R, K)
    uint32_t tempVal = 0;    // Temporary storage for swapping L and R
    
    // Array to store the 16 subkeys (48 bits each)
    uint64_t subKeys[16] = {0};
    // Expanded R block (from 32 bits to 48 bits)
    uint64_t expandedR = 0;
    
    // Key after applying Permuted Choice 1 (56 bits)
    uint64_t pc1Key = 0;
    // Combined key (C||D) 56 bits, used for subkey extraction
    uint64_t combinedKey = 0;
    
    // Variables for data permutation steps
    uint64_t initialPermResult = 0; // Result after Initial Permutation
    uint64_t inversePermResult = 0; // Result after Inverse Initial Permutation
    uint64_t preOutput = 0;         // Data block before final permutation
    
    /* 1. Initial Permutation: Permute the input data according to IP_Table */
    for (i = 0; i < 64; i++) {
        initialPermResult <<= 1;
        initialPermResult |= (data >> (64 - IP_Table[i])) & BIT_MASK_64;
    }
    
    // Split the permuted data into two 32-bit halves: L (left) and R (right)
    L = (uint32_t)(initialPermResult >> 32) & LOWER_32_MASK;
    R = (uint32_t) initialPermResult & LOWER_32_MASK;
    
    /* 2. Key Schedule Generation: Apply PC1 to the initial key to get 56 bits */
    for (i = 0; i < 56; i++) {
        pc1Key <<= 1;
        pc1Key |= (key >> (64 - PC1_Table[i])) & BIT_MASK_64;
    }
    
    // Split the 56-bit key into two 28-bit halves (C and D)
    keyLeft = (uint32_t)((pc1Key >> 28) & 0x0fffffff);
    keyRight = (uint32_t)(pc1Key & 0x0fffffff);
    
    /* 3. Generate 16 subkeys for the 16 DES rounds */
    for (i = 0; i < 16; i++) {
        // Perform left circular shifts on key halves according to Shift_Schedule
        for (j = 0; j < Shift_Schedule[i]; j++) {
            keyLeft = ((keyLeft << 1) & 0x0fffffff) | ((keyLeft >> 27) & 0x1);
            keyRight = ((keyRight << 1) & 0x0fffffff) | ((keyRight >> 27) & 0x1);
        }
        
        // Combine the two halves back into a 56-bit key
        combinedKey = (((uint64_t) keyLeft) << 28) | (uint64_t) keyRight;
        
        // Apply Permuted Choice 2 (PC2) to get the 48-bit subkey
        subKeys[i] = 0;
        for (j = 0; j < 48; j++) {
            subKeys[i] <<= 1;
            subKeys[i] |= (combinedKey >> (56 - PC2_Table[j])) & BIT_MASK_64;
        }
    }
    
    /* 4. 16 Rounds of DES Processing */
    for (i = 0; i < 16; i++) {
        expandedR = 0;
        // Expand R from 32 bits to 48 bits using the Expansion_Table
        for (j = 0; j < 48; j++) {
            expandedR <<= 1;
            expandedR |= (uint64_t)((R >> (32 - Expansion_Table[j])) & BIT_MASK_32);
        }
        
        // XOR the expanded R with the corresponding subkey
        // (Subkeys are used in reverse order for decryption)
        if (mode == 'd')
            expandedR ^= subKeys[15 - i];
        else
            expandedR ^= subKeys[i];
        
        sBoxOutput = 0;
        // Process the 48-bit result through the 8 S-Boxes
        for (j = 0; j < 8; j++) {
            // Extract the 6 bits for the j-th S-Box:
            // The first and last bits form the row index; the middle 4 bits form the column index
            row = (char)((expandedR & (0x0000840000000000 >> (6 * j))) >> (42 - 6 * j));
            row = (row >> 4) | (row & 0x01); // Combine outer bits for row
            col = (char)((expandedR & (0x0000780000000000 >> (6 * j))) >> (43 - 6 * j)); // Middle bits for column
            
            // Append the 4-bit S-Box output
            sBoxOutput <<= 4;
            sBoxOutput |= (uint32_t)(S_Box[j][16 * row + col] & 0x0f);
        }
        
        // Apply the Permutation P to the S-Box output
        fFunctionResult = 0;
        for (j = 0; j < 32; j++) {
            fFunctionResult <<= 1;
            fFunctionResult |= (sBoxOutput >> (32 - Permutation_Table[j])) & BIT_MASK_32;
        }
        
        // Update for the next round: New R = L XOR f(R, subkey), then New L = Old R
        tempVal = R;
        R = L ^ fFunctionResult;
        L = tempVal;
    }
    
    /* 5. Combine the two halves (note the swap of L and R) and apply Inverse IP */
    preOutput = (((uint64_t) R) << 32) | (uint64_t) L; // Swap L and R before final permutation
    
    // Apply Inverse Initial Permutation (IP^-1)
    for (i = 0; i < 64; i++) {
        inversePermResult <<= 1;
        inversePermResult |= (preOutput >> (64 - Inverse_IP_Table[i])) & BIT_MASK_64;
    }
    
    return inversePermResult;
}

int main(int argc, const char * argv[]) {
    int i;
    
    // 64-bit input data (sample plaintext)
    uint64_t inputData = 0x0123456789ABCDEF;
    // Three keys for 3DES: key1, key2, key3
    uint64_t keys[3] = {0x0123456789ABCDEF, 0x9474B8E8C73BCA7C, 0x9474B8E8C73BC97D};
    uint64_t result = inputData;
 	uint64_t result_des = 0; // Variable to store single DES result

    printf ("\nEncrypting using DES & 3-DES\n------------------------------\n");
    printf ("Input Data: %016llx\n", inputData);
    
    // Perform single DES encryption with key1
    result = des_algorithm(inputData, keys[0], 'e');
    result_des = result; // Store the single DES result
    printf ("DES Encryption: %016llx\n", result_des);
    
    // Perform 3DES encryption: Encrypt with key1, Decrypt with key2, Encrypt with key3
    // Note: The result from single DES is used as input for the next step
    result = des_algorithm(result, keys[1], 'd'); // Decrypt with key2
    result = des_algorithm(result, keys[2], 'e'); // Encrypt with key3
    printf ("3DES Encryption: %016llx\n", result);
    
    printf ("\nDecrypting using DES & 3-DES\n------------------------------\n");   
    // Perform 3DES decryption: Decrypt with key3, Encrypt with key2, Decrypt with key1
    result = des_algorithm(result, keys[2], 'd'); // Decrypt with key3
    
    // Perform single DES decryption (using the stored single DES result)
    result_des  = des_algorithm(result_des, keys[0], 'd'); // Decrypt with key1
    printf ("DES Decryption: %016llx\n", result_des);

    /* 
     * Complete 3DES decryption process (second and third steps)
     * Step 2: Encrypt with key2 (reverse of encryption process)
     * Step 3: Decrypt with key1
     * This completes the 3DES decryption sequence: D(key1, E(key2, D(key3, ciphertext)))
     */
    result = des_algorithm(result, keys[1], 'e'); // Encrypt with key2
    result = des_algorithm(result, keys[0], 'd'); // Decrypt with key1
    printf ("3DES Decryption: %016llx\n", result);
    
    /*
     * DES Algorithm Verification
     * --------------------------
     * Based on Ronald L. Rivest's test vectors
     * 
     * Test Case:
     * Initial value (X0): 9474B8E8C73BCA7D
     * Expected final value after 16 rounds (X16): 1B1A2DDB4C642438
     * 
     * Expected intermediate values after each encrypt/decrypt round:
     * Round 1  - E: 8da744e0c94e5e17
     * Round 1  - D: 0cdb25e3ba3c6d79
     * Round 2  - E: 4784c4ba5006081f
     * Round 2  - D: 1cf1fc126f2ef842
     * Round 3  - E: e4be250042098d13
     * Round 3  - D: 7bfc5dc6adb5797c
     * Round 4  - E: 1ab3b4d82082fb28
     * Round 4  - D: c1576a14de707097
     * Round 5  - E: 739b68cd2e26782a
     * Round 5  - D: 2a59f0c464506edb
     * Round 6  - E: a5c39d4251f0a81e
     * Round 6  - D: 7239ac9a6107ddb1
     * Round 7  - E: 070cac8590241233
     * Round 7  - D: 78f87b6e3dfecf61
     * Round 8  - E: 95ec2578c2c433f0
     * Round 8  - D: 1b1a2ddb4c642438 (final value X16)
     */
    
    /* Program completion */
    exit(0);
}