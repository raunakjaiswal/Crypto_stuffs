//201951127
// Raunak kumar jaiswal
// AES  Implementation in (C language)

#include<stdio.h>
#include<stdlib.h>
#include <inttypes.h>
#include<time.h>

// subbytes box for encryption (16 x 16)
int s_box[16][16] = {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

// inverse subbytes box for decryption (16 x 16)
int s_inverse_box[16][16] = {
        {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},
        {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb},
        {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e},
        {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25},
        {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92},
        {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84},
        {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06},
        {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b},
        {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73},
        {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e},
        {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b},
        {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4},
        {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f},
        {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef},
        {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61},
        {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
    }};

// function to initialize (4 x 4) matrix dynamically using malloc and
// return pointer to initialized matrix
uint8_t **initialize_matrix()
{
   uint8_t **temp;
   temp = malloc(sizeof(uint8_t*) * 4);
    for(int i = 0; i < 4; i++) {
        temp[i] = malloc(sizeof(uint8_t*) * 4);
    }
     return temp;
}

// function that helps to perform xor between two matrix (4 x 4) 
// and return pointer of output matrix
uint8_t **xor_two_matrix(uint8_t **p, uint8_t key[4][4])
{
   uint8_t **temp;
       temp = malloc(sizeof(uint8_t*) * 4);
    for(int i = 0; i < 4; i++) {
        temp[i] = malloc(sizeof(uint8_t*) * 4);
    }
   for(int  i = 0;i<4;i++)
   {
       for(int  j = 0;j<4;j++)
       {
           temp[i][j] = (p[i][j] ^ key[i][j]);
       }
   }
   return temp;
}

// function to  find subbytes of 8bit using s_box by 
// spliting 8bit to 4bit(row) and 4bit(column)
uint8_t subbytes(uint8_t s)
{
   uint8_t si = s;
   si = (si>>4);
   uint8_t sj = s;
   sj = (sj &15);
   uint8_t s_dash = s_box[si][sj];
   return s_dash;
 }

// (function to perform rotation) 
// convert (b0 b1 b2 b3)  ->  (b1 b2 b3 b0) where each "b" is of 8bit
uint32_t rotword(uint32_t rr)
{
    uint32_t tm1   = rr;
    tm1 = (tm1>>24);
    rr = (rr<<8);
    rr = (rr | tm1);
   return rr;
}

// function to  find inverse_subbytes of 8bit using s_inverse_box by 
// spliting 8bit to 4bit(row) and 4bit(column)
uint8_t inverse_subbytes(uint8_t s)
{
   uint8_t si = s;
   si = (si>>4);
   uint8_t sj = s;
   sj = (sj &15);
   uint8_t s_dash = s_inverse_box[si][sj];
   return s_dash;
 }


// function to find subword 
// convert (b0  b1  b2  b3) -> (b0'  b1'  b2'  b3') where b' = subbytes(b)
uint32_t subword(uint32_t ss)
{
     uint32_t sub = 0;
     for(int i  = 3;i>=0;i--)
     {
          uint32_t t1 = ss;
          t1 = (t1>>(i*8));
          t1 = (t1 & 0xFF);
          uint8_t tm= t1;
          uint8_t s1 = subbytes(tm);
          sub = sub| s1;
          if(i>=1)
          {
              sub  = (sub<<8);
          }
     }
     return sub;
}

// function to create 44 word each of 32 bit using (10 round constant)
//  and (input 128bit key)
 void make_w(uint32_t w[44], uint8_t key[16])
 {
    uint32_t rcon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000,
     0x40000000, 0x80000000, 0x1b000000, 0x36000000};
    for(int i =  0;i<=3;i++)
    {
        w[i] = 0;
        w[i] = (key[4*i]);
        w[i] = (w[i]<<8);
        w[i] = (w[i] | (key[4*i+1]) );
        w[i] = (w[i]<<8);
        w[i] = (w[i] | (key[4*i+2]));
        w[i] = (w[i]<<8);
        w[i] = (w[i] | (key[4*i+3]));   
    }
    for(int  i = 4;i<44;i++)
    {
        uint32_t temp  = w[i-1];
        if(i%4 == 0)
        {
            uint32_t  rot_word = rotword(temp);
           
            uint32_t sub_word = subword(rot_word);
            temp = (subword(rotword(temp))) ^ (rcon[(i/4)-1]);
        }
        w[i] = (w[i-4]^ temp);
    }
   
 }

// function to create (11 round keys each of 128bit) 
// using (44 word each of 32bit)
 void make_round_keys(uint8_t round_keys[11][4][4], uint32_t w[44])
 {
     int cnt = 0;
     for(int  i = 0;i<11;i++)
     {
         for(int  j  = 0;j<4;j++)
         {
             uint32_t ww  =w[cnt];
             for(int k = 0;k<4;k++)
             {
                  uint32_t temp = ww;
                  temp = (temp>>(8*(3-k)));
                  temp = (temp & 0xFF);
                  uint8_t tm = temp;
                  round_keys[i][k][j] = tm;
             }
             cnt++;
         }
     }
 }

// function to create subbytes matrix used in encryption 
// return pointer to output matrix(subytes matrix) (128bit-> 128bit)
uint8_t **subbytes_mat(uint8_t **temp)
{
    uint8_t **sub_mat = initialize_matrix();
    for(int  i = 0;i<4;i++)
    {
        for(int  j = 0;j<4;j++)
        {
            sub_mat[i][j] = subbytes(temp[i][j]);
        }
    }
    return sub_mat;
}

// function that perform shift rows operation on matrix used in encryption
// return pointer to output matrix (128bit -> 128bit)
uint8_t **shift_rows_mar(uint8_t **temp)
{
    uint8_t **shiftrows  = initialize_matrix();
    for(int  i = 0;i<4;i++)
    {
        for(int j = 0;j<4;j++)
        {
            int index=  (((j-i)%4)+4)%4;
            shiftrows[i][index] = temp[i][j];
        }
    }
    return shiftrows;
}

// function to perform 
uint8_t fun(uint8_t tp){
    uint8_t x;
    if((tp >> 7) == 0){
        x = tp << 1;
    }else{
        x = (tp << 1) ^ 27;
    }
    return x;
}

// function to find mixcolumn of matrix used in encryption
// return pointer to output matrix (128bit -> 128bit)
uint8_t **mixcolumn(uint8_t **s)
{
  uint8_t **r = initialize_matrix();
  for(int j = 0;j<4;j++)
  {  
    for(int i=0; i<4; i++)
    {
        uint8_t t1, t2;
        t1 = fun(s[i][j]);
        t2 = fun(s[(i+1)%4][j]);
        r[i][j] = t1^t2^s[(i+1)%4][j]^s[(i+2)%4][j]^s[(i+3)%4][j];
    }
  }
  return r;
}

// function to find inverse mixcolumn of matrix used in decryption
// return pointer to output matrix (128bit -> 128bit)
uint8_t **inverse_mixcolumn(uint8_t **s)
{
   uint8_t **r = initialize_matrix();
  for(int j = 0;j<4;j++)
  {  
    for(int i=0; i<4; i++)
    {
        uint8_t t11, t9, t13, t14;
        uint8_t x1 = s[(i+1)%4][j] ;
        uint8_t x2 = s[(i+3)%4][j] ;
        uint8_t x3= s[(i+2)%4][j];
        uint8_t x4= s[i][j];
        t14 =   fun(fun((fun(x4) ^ x4))^x4);
        t13 =    fun(fun(fun(x3)^x3)) ^ x3;
        t11  =   fun(fun(fun(x1)) ^ x1) ^ x1;
        t9 =    fun(fun(fun(x2))) ^ x2;
        r[i][j] = t14 ^ t13 ^ t11 ^ t9;
    }
  }
  return r;
}

//  this function used in encryption (9 times) 
// this fucntion performs 3 operation:
// subbytes operation on matrix -> shift  rows on matrix - > mix-column on matrix
// in given order.
uint8_t **function(uint8_t **temp)
{
   uint8_t **s1 = subbytes_mat(temp);
   uint8_t **s2 = shift_rows_mar(s1);
   uint8_t **s3 = mixcolumn(s2);
   return s3;
}


// function to create inverse subbytes matrix used in decryption 
// return pointer to output matrix(inverse subytes matrix) (128bit-> 128bit)
uint8_t **inverse_subbytes_mat(uint8_t **temp)
{
    uint8_t **inverse_sub_mat = initialize_matrix();
    for(int  i = 0;i<4;i++)
    {
        for(int  j = 0;j<4;j++)
        {
            inverse_sub_mat[i][j] = inverse_subbytes(temp[i][j]);
        }
    }
    return inverse_sub_mat;
}

// function that perform inverse shift rows operation on matrix used in decryption
// return pointer to output matrix (128bit -> 128bit)
uint8_t **inverse_shift_rows_mar(uint8_t **temp)
{
    uint8_t **shiftrows  = initialize_matrix();
    for(int  i = 0;i<4;i++)
    {
        for(int j = 0;j<4;j++)
        {
            int index=  (((j+i)%4)+4)%4;
            shiftrows[i][index] = temp[i][j];
        }
    }
    return shiftrows;
}

//  this function used in decryption (9 times) 
// this fucntion performs 3 operation:
// inverse-mix-column on matrix -> inverse-shift-rows on matrix -> inverse-subytes on matrix
// in given order.
uint8_t **inverse_function(uint8_t **temp)
{
   uint8_t **s3 = inverse_mixcolumn(temp);
   uint8_t **s2 = inverse_shift_rows_mar(s3);
   uint8_t **s1 = inverse_subbytes_mat(s2);
   return s1;
}

//  this function used in encryption (1 times) 
// this fucntion performs 2 operation:
// subbytes operation on matrix -> shift  rows on matrix
// in given order.
uint8_t **function10(uint8_t **temp)
{
   uint8_t **s1 = subbytes_mat(temp);
   uint8_t **s2 = shift_rows_mar(s1);
  return s2;
}

//  this function used in decryption (1 times) 
// this fucntion performs 2 operation:
// inverse-shift-rows on matrix ->inverse-subbytes on matrix
// in given order.
uint8_t **inverse_function10(uint8_t **temp)
{
   uint8_t **s2 = inverse_shift_rows_mar(temp);
   uint8_t **s1 = inverse_subbytes_mat(s2);
   return s1;
}

// function to perform encryption using 11 round keys each of 128bit and 128bit plaintext
// return 128 bit cipher text
uint8_t **encryption(uint8_t round_keys[11][4][4], uint8_t **plaintext)
{
  uint8_t  **tm1  = plaintext;
    for(int  i = 0;i<11;i++)
    {
        uint8_t **after_xor = xor_two_matrix(tm1, round_keys[i]);
        if(i==10)
        {
            return after_xor;
        }
        if(i==9)
        {
            uint8_t **fun10 = function10(after_xor);
            tm1 = fun10;
            continue;
        }
       uint8_t **funn = function(after_xor);
       tm1 = funn;
    }
    return tm1;
}

// function to perform decryption using 11 round keys each of 128bit and 128bit ciphertext
// return 128 bit decrypted text(plaintext)
uint8_t **decryption(uint8_t round_keys[11][4][4], uint8_t **ciphertext)
{
    uint8_t **tm =  ciphertext;
   for(int  i = 10;i>=0;i--)
   {     
        uint8_t **xor = xor_two_matrix(tm, round_keys[i]);
        if(i==0)
        {
            return xor;
        }
       if(i==10)
       {
           uint8_t **funn = inverse_function10(xor);
           tm = funn;
       }
       else
       {
           uint8_t **funn = inverse_function(xor);
           tm = funn;
       }
   }
}

int main()
{

    printf("Enter 128 bit plaintext\n");
    uint8_t **plaintext = initialize_matrix();

    for(int  i=0;i<4;i++)
    {
        for(int  j=0;j<4;j++)
        {
            scanf("%"SCNu8, &plaintext[j][i]);
        }
    }
    printf("Enter 128 bit key\n");

   uint8_t key[16];
    for(int i=0;i<16;i++)
    {
            scanf("%"SCNu8, &key[i]);
    }

// array to store 44 word each of 32 bit
 uint32_t w[44];

// creating 44 words 
 make_w(w, key);

// 3d array to store 11 round keys each of 128 bit
 uint8_t round_keys[11][4][4];

// creating 11 round keys using 44 word
 make_round_keys(round_keys, w);

//performing encryption
uint8_t **ciphertext = encryption(round_keys, plaintext);

printf("\ncipher-text,  h(m1||m2): \n");
for(int  i = 0;i<4;i++)
{
    for(int  j = 0;j<4;j++)
    {
       printf("%"PRIu8" ", ciphertext[j][i]); 
    }
}
printf("\n");

// performing decryption
//uint8_t **decrypted_text =  decryption(round_keys, ciphertext);


// second pre - image implementation

srand(time(0));

// array to store random keys(128 bit)
uint8_t random_key[16];

// creating random keys
for(int i = 0;i<16;i++)
{
    random_key[i] = rand()%256;
}


//printing random keys
printf("\nRandom key (m2'): \n");
for(int  i = 0;i<16;i++)
{
    printf("%"PRIu8" ", random_key[i]);
}
printf("\n");



 // array to store 44 word each of 32 bit for second-pre-image
 uint32_t w_rand[44];

 // creating 44 words for second-pre-image
 make_w(w_rand, random_key);

 
// 3d array to store 11 round keys each of 128 bit for second-pre-image
 uint8_t rand_round_keys[11][4][4];

 // creating 11 round keys using 44 word for second-pre-image
 make_round_keys(rand_round_keys, w_rand);

 // performing decryption for second pre-image by passing random round 11 keys and ciphtext
 uint8_t **pre_image_decrypted_text =  decryption(rand_round_keys, ciphertext);
 printf("\nm1': \n");
 for(int i = 0;i<4;i++)
 {
     for(int  j = 0;j<4;j++)
     {
         printf("%"PRIu8" ", pre_image_decrypted_text[j][i]); 
     }
 }
printf("\n");
 printf("\nm1'||m2' : \n");
 for(int  i = 0;i<16;i++)
{
    printf("%"PRIu8" ", random_key[i]);
}
 for(int i = 0;i<4;i++)
 {
     for(int  j = 0;j<4;j++)
     {
         printf("%"PRIu8" ", pre_image_decrypted_text[j][i]); 
     }
 }
 printf("\n");

// performing encryption for second-pre-image by passing random round 11 keys and
// above output(pre-image-decrpted-text)
 uint8_t **pre_plain = encryption(rand_round_keys, pre_image_decrypted_text);


 printf("\nh(m1'||m2') : \n");
 for(int i =  0;i<4;i++)
 {
     for(int  j = 0;j<4;j++)
     {
       printf("%"PRIu8" ", pre_plain[j][i]);   
     }
     
 }
printf("\n");
 printf("\nh(m1||m2) : \n");
 for(int i =  0;i<4;i++)
 {
     for(int  j = 0;j<4;j++)
     {
       printf("%"PRIu8" ", ciphertext[j][i]);   
     }
     
 }
}