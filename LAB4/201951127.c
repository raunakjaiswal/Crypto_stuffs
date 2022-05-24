// 201951127
// Raunak kumar jaiswal

#include<stdio.h>
#include<stdlib.h>
#include <inttypes.h>
#include<time.h>
#include<stdint.h>
int prime=101;
// function for additive-inverse
int addinv(int val)
{
    return (prime - val)%prime;
}
int gcdExtended(int a, int b, int* x, int* y);

int modInverse(int a)
{
    int x, y;
    int g = gcdExtended(a, prime, &x, &y);
    if (g != 1)
        return -1;
    else
    {
        int res = (x % prime + prime) % prime;
        return res;
    }
}
int gcdExtended(int a, int b, int* x, int* y)
{
    if (a == 0)
    {
        *x = 0, *y = 1;
        return b;
    }
    int x1, y1;
    int gcd = gcdExtended(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return gcd;
}

// function to add two points of elliptic curve 
int *add(int px,int py,int qx,int qy)//to perform addition operation on the curve with 2 points
{
   // int m;
    int *ans = (int *)malloc(2 * sizeof(int));

  // void add(int p[2], int q[2], int r[2])
//{

    if(px==0&&py==0)
    {
        ans[0]=qx;
        ans[1]=qy;
        return ans;
    }
    if(qx==0&&qy==0)
    {
        ans[0]=px;

        ans[1]=py;
        return ans;
    }

    // int x1 = p[0];
    // int y1 = p[1];
    // int x2 = q[0];
    // int y2 = q[1];
     int x1 = px;
    int y1 = py;
    int x2 = qx;
    int y2 = qy;
    if (x1 == x2 && y1 == addinv(y2))
    {
        ans[0] = 0;
        ans[1] = 0;
        return ans;
    }
    int m=0;
    if (x1 != x2 && y1 != y2)
    {

        m = ((y2 + addinv(y1)) * modInverse(x2 + addinv(x1))) % prime;
    }
    else if (x1 == x2 && y1 == y2)
    {

        m = ((3 * x1 * x1 + 25) *modInverse(2 * y1)) % prime;
    }
    ans[0] = ((m * m) + addinv(x1) + addinv(x2)) % prime;

    ans[1] = (m * (ans[0] + addinv(x1)) + y1) % prime;
    ans[1] = addinv(ans[1]) % prime;
    return ans;
    //////////////////////
}

int *addn(int px,int py,int n)//to add a given point n times
{
     int *ans = (int *)malloc(2 * sizeof(int));
     // (int *)malloc(sizeof(int) * len);
     int *temp=(int *)malloc(2 * sizeof(int));
    if(n==1)
     {
            ans[0]=px;
            ans[1]=py;
            return ans;
     }
    if(n==2)
    {
        return add(px,py,px,py);
    }
    temp = addn(px, py, n - 1);
    return add(temp[0], temp[1], px, py);
}
// fucntion Diffie-Hellman key exchange

// void find_shared_key(int n, int al[2], int a, int mod, int shared_key[2])
// {

//    int alpha[2] = {al[0], al[1]};
   
//     int ans[2] = {al[0], al[1]};
//     for(int i =  0;i<n-1;i++)
//     {
//         addpoints(alpha, ans,ans,mod,a);
//         printf("%d %d\n", ans[0], ans[1]);
//     }
//     shared_key[0] = ans[0];
//     shared_key[1]=ans[1];
// }

// function to find alpha from eliiptic curve
int *find_alpha(int a, int b, int mod)
{
    int cnt = 0;
   for (int x = 0; x < mod; x++)
    {
        for (int y = 0; y < mod; y++)
        {
            int g = ((x * x * x) + a * x + b) % mod;
            int gg = ((y * y) % mod+ addinv(g)) % mod;
            if (gg == 0)
            {
                cnt++;
                // change cnt for different alpha :)
                if(cnt==8)
                {
                    int pp[2] = {x,y};
                    int *pointer = pp;
                    return pointer;
                    break;
               
                }
            }
        }
    }
}
//// SHA-256

// function to initialize 8bit array dynamically
uint8_t *initialize_input(int num_words)
{
   uint8_t *temp;
   temp = (uint8_t *)malloc(sizeof(uint8_t) * num_words);
    return temp;
}

// function to initialize 32bit array dynamically
uint32_t *initialize_input32(int num_words)
{
   uint32_t *temp;
   temp = (uint32_t *)malloc(sizeof(uint32_t) * num_words);
    return temp;
}

// to perform shift right operation
uint32_t ShR(uint32_t A, int n)
{
    return (A>>n);
}

// to perform circular right shift
uint32_t RotR(uint32_t A, int n)
{
    return (A >> n)|(A << (32 - n)) & 0xFFFFFFFF;
}


uint32_t sigma_0(uint32_t x)
{
     uint32_t out = RotR(x,2) ^ RotR(x,13) ^ RotR(x,22);
     return out;
}
uint32_t sigma_1(uint32_t x)
{
     uint32_t out = RotR(x,6) ^ RotR(x,11) ^ RotR(x,25);
     return out;
}
uint32_t maz(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ (x & z) ^ (y & z);
}

uint32_t ch(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ ((~x) & z) ;
}

uint32_t roww_0(uint32_t x)
{
     uint32_t out = RotR(x,7) ^ RotR(x,18) ^ ShR(x,3);
     return out;
}
uint32_t roww_1(uint32_t x)
{
     uint32_t out = RotR(x,17) ^ RotR(x,19) ^ ShR(x,10);
     return out;
}

// sha k constant
uint32_t k[64]={0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

// fucntion to calculate SHA-256 key
uint32_t * hash_computation(uint32_t *w, int n)
{
    uint32_t h[8] = {0x6a09e667 , 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab,  0x5be0cd19};
    
      for(int  i= 0;i<n;i++)
      {
          uint32_t var[8] = {h[0], h[1], h[2], h[3],h[4],h[5],h[6],h[7]};
              for(int  j = 0;j<64;j++)
              {
                  uint32_t t1  = (var[7] + sigma_1(var[4]) + ch(var[4], var[5], var[6]) + k[j]+ w[(64*i) +j])%(0x100000000);
                  uint32_t t2 = (sigma_0(var[0]) + maz(var[0], var[1], var[2])) %(0x100000000);
                  var[7] = var[6];
                  var[6] = var[5];
                  var[5]= var[4];
                  var[4]= (var[3]+t1)% (0x100000000);
                  var[3]= var[2];
                  var[2] = var[1];
                  var[1] = var[0];
                  var[0] = (t1+t2) % (0x100000000);
              }
              for(int j = 0;j<8;j++)
              {
                  h[j] = (h[j] + var[j]) %(0x100000000);
              }
      }
      uint32_t *sha_hash = initialize_input32(8);
      for(int  i = 0;i<8;i++)
      {
          sha_hash[i] = h[i];
      }
  return sha_hash;
}


// function to convert 8bit array into 32bit array
uint32_t *convert8bit_to_32bitblock(uint8_t *block, int block_size)
{
    int block_32_size = block_size/4;
   uint32_t *block_32  = initialize_input32(block_32_size);
   int cnt = 0;
   for(int i = 0;i<block_32_size;i++)
   {
       uint32_t temp = 0;
       for(int  j = 3;j>=0;j--)
       {
          uint32_t tm = block[cnt];
           tm = (tm<< (j*8));
           temp = temp | tm;
           cnt++;
       }
       block_32[i] = temp;
   }
   return block_32;
}

// SHA block decomposition function
uint32_t * block_decomposition(int num_words,uint8_t *block_512)
{
   int  l = num_words*8;
   int  k = ((448 - (l+1)%512)+512)%512;
   k = k -7;
   int bit8_block = num_words +1+(k/8)+8;
   uint32_t *block_32 =  convert8bit_to_32bitblock(block_512, bit8_block);
   int num_512block_size = bit8_block/64;
   uint32_t *block_decom = initialize_input32(num_512block_size*64);
   int cnt= 0;
   for(int  i = 0;i<num_512block_size;i++)
   {
       for(int j = 0;j<16;j++)
         {
             block_decom[cnt] = block_32[16*i+j]; 
             cnt++;
         }
         for(int j  = 16;j<64;j++)
         {
             block_decom[cnt]  = roww_1(block_decom[64*i + j-2])+ block_decom[64*i + j-7]+roww_0(block_decom[64*i + j-15]) + block_decom[64*i+ j-16];
             cnt++;
         }
   } 

 return hash_computation(block_decom, num_512block_size);
 
}

// function to add padding to make it multiples of 512
uint8_t *padding(int num_words, uint8_t *input)
{
   int  l = num_words*8;
   int  k = ((448 - (l+1)%512)+512)%512;
    
    uint8_t first_box = 1;
    first_box = (first_box<<7);
    k = k - 7;
    int block_required = k/8;
    int total_block =num_words + 1 + block_required+8;
    uint8_t *after_padding =  initialize_input(total_block);
    int  i = 0;
    while(i<num_words)
    {
       after_padding[i] =input[i];
       i++;
    }
    after_padding[i] = first_box;
    i++;
    while(i<total_block-8)
    {
        after_padding[i] = 0;
        i++;
    }
    uint64_t len= l;
    int start = 63;
    while(i<total_block)
    {
        uint8_t pp = 0;
        for(int j = 7;j>=0;j--)
        {
            int bit = ((len>>start) & 1 )==1 ? 1:0;
             pp = pp | (bit<<j);
            start--;
        }
      after_padding[i] = pp;
      i++;
    }

     return after_padding;
   
}


uint32_t *find_sha_256(int num_words, uint8_t *input)
{
    uint8_t *after_padding =  padding(num_words, input);
   uint32_t *hhh = block_decomposition(num_words,after_padding);
   return hhh;
}

// function  that convert 32bit array to 8bit array
uint8_t *convert32bit_to_8bitblock(uint32_t *k)
{  
    uint8_t *out = initialize_input(32);
    int cnt = 0;
    for(int  i = 0;i<8;i++)
    {
        uint32_t temp = k[i];
        int st = 31;
        for(int j = 0;j<4;j++)
        {
            uint8_t pp = 0;
            for(int ll = 7;ll>=0;ll--)
            {
               int bit  = (temp>>st)&1 ? 1:0;
               pp = pp| (bit<<ll);
               st--;
            }
            out[cnt] = pp;

           cnt++;
        }
    }
    return out;
}
// DONE SHA-256

// AES - 256
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

// function to calculte xor between two 8bit 2d array using pointer
uint8_t **xor_two_mat(uint8_t **p, uint8_t **key)
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

// function to create 60 word each of 32 bit using (10 round constant)
//  and (input 256bit key)
 void make_w(uint32_t w[60], uint8_t key[32])
 {
    uint32_t rcon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000,
     0x40000000, 0x80000000, 0x1b000000, 0x36000000};
    for(int i =  0;i<=7;i++)
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
    for(int  i = 8;i<60;i++)
    {
        uint32_t temp  = w[i-1];
        if(i%8 == 0)
        {
            uint32_t  rot_word = rotword(temp);
           
            uint32_t sub_word = subword(rot_word);
            temp = (subword(rotword(temp))) ^ (rcon[(i/8)-1]);
        }
        else if(i%8==4)
        {
            temp = subword(temp);
        }
        w[i] = (w[i-8]^ temp);
    }
   
 }

// function to create (15 round keys each of 128bit) 
// using (60 word each of 32bit)
 void make_round_keys(uint8_t round_keys[15][4][4], uint32_t w[60])
 {
     int cnt = 0;
     for(int  i = 0;i<15;i++)
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
uint8_t **encryption(uint8_t round_keys[15][4][4], uint8_t **plaintext)
{
  uint8_t  **tm1  = plaintext;
    for(int  i = 0;i<15;i++)
    {
        uint8_t **after_xor = xor_two_matrix(tm1, round_keys[i]);
        if(i==14)
        {
            return after_xor;
        }
        if(i==13)
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

// function to perform decryption using 15 round keys each of 128bit and 128bit ciphertext
// return 128 bit decrypted text(plaintext)
uint8_t **decryption(uint8_t round_keys[15][4][4], uint8_t **ciphertext)
{
    uint8_t **tm =  ciphertext;
   for(int  i = 14;i>=0;i--)
   {     
        uint8_t **xor = xor_two_matrix(tm, round_keys[i]);
        if(i==0)
        {
            return xor;
        }
       if(i==14)
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

// DOne AES-256


uint8_t *xor_between_256bit_num(uint8_t *k, uint8_t num)
{
    uint8_t *temp = initialize_input(32);
    for(int  i = 0;i<32;i++)
    {
        temp[i]  = k[i];
    }
    temp[31] = (temp[31] ^ num);
    return temp;
}
int main()
{
    //ques 1

    int mod = 101; 
    
    int a = 25; int b = 31;

   //ques 2 & 3
    int al[2];
    int *alpha = find_alpha(a,b,mod);
    al[0] = alpha[0];
    al[1] =alpha[1];
    printf("\n---------->Alpha:\n %d %d \n", alpha[0], alpha[1]);
    
  //ques 4  
    int alice_private_key;
    int bob_private_key;
    printf("\nEnter Alice private key between [1,100]\n");
    scanf("%d", &alice_private_key);
    printf("\nEnter Bob private key between [1,100]\n");
    scanf("%d", &bob_private_key);
  
  //qies 5

    int *alice_public_key ;
    alice_public_key= addn(al[0],al[1],alice_private_key);
    int *bob_public_key;
    bob_public_key=addn(al[0],al[1],bob_private_key);
   printf("------> Alice public key:\n %d %d\n", alice_public_key[0], alice_public_key[1]);
     printf("------> Bob public key:\n %d %d\n", bob_public_key[0], bob_public_key[1]);
     int *shared_key;
     shared_key=addn(alice_public_key[0],alice_public_key[1],bob_private_key);
        printf("\nShared key: %d %d\n", shared_key[0], shared_key[1]);


     // SHa-256

//ques 6
    int alice_words = 2;
    uint8_t *input =  initialize_input(alice_words);
    input[0] = shared_key[0];
    input[1] = shared_key[1];


    uint32_t *ka = find_sha_256(alice_words,input);

//ques  7
    uint32_t *kb = find_sha_256(alice_words, input);
    

//ques 8

 uint8_t *k_a_32bytes  =convert32bit_to_8bitblock(ka);
 uint8_t *k_b_32bytes  =convert32bit_to_8bitblock(kb);

printf("\n\n----------->KA:  \n\n");
  for(int  i=0;i<32;i++)
  {
       printf("%"PRIx8" ", k_a_32bytes[i]);
  }

printf("\n\n---------->KB:  \n\n");
  for(int  i=0;i<32;i++)
  {
       printf("%"PRIx8" ", k_b_32bytes[i]);
  }


  // AES - 256

//ques 9
    printf("\n\n Enter Alices 256-bit message(Ma)-------->\n\n");
    uint8_t *Ma = initialize_input(32);
    for(int  i = 0;i<32;i++)
    {
        scanf("%"SCNx8, &Ma[i]);
    }

//ques 10
    uint8_t **plaintext1 = initialize_matrix();
    uint8_t **plaintext2 = initialize_matrix();

    uint8_t **IV = initialize_matrix();
    int cnt1 = 0;
    for(int  i=0;i<4;i++)
    {
        for(int  j=0;j<4;j++)
        {
            plaintext1[j][i] = Ma[cnt1++];
        }
    }
    for(int  i=0;i<4;i++)
    {
        for(int  j=0;j<4;j++)
        {
            plaintext2[j][i]  = Ma[cnt1++];
        }
    }
    
    for(int  i=0;i<4;i++)
    {
        for(int  j=0;j<4;j++)
        {
            IV[j][i] = 0;
        }
    }

   uint8_t keyA[32];
    for(int i=0;i<32;i++)
    {
        keyA[i] = k_a_32bytes[i];
    }

// array to store 60 word each of 32 bit
 uint32_t w[60];

// creating 60 words 
 make_w(w, keyA);

// 3d array to store 15 round keys each of 128 bit
 uint8_t round_keys[15][4][4];

// creating 15 round keys using 60 word
 make_round_keys(round_keys, w);

//performing encryption
uint8_t **after_xor_with_IV = xor_two_mat(plaintext1, IV);
uint8_t **ciphertext1 = encryption(round_keys, after_xor_with_IV);
uint8_t **xor_cit1_pl2 = xor_two_mat(ciphertext1,plaintext2);
uint8_t **ciphertext2 = encryption(round_keys, xor_cit1_pl2);
uint8_t *CA = initialize_input(32);


cnt1 = 0;
for(int  i = 0;i<4;i++)
{
    for(int  j = 0;j<4;j++)
    {
        CA[cnt1++] = ciphertext1[j][i];
    }
}
for(int  i = 0;i<4;i++)
{
    for(int  j = 0;j<4;j++)
    {
         CA[cnt1++] = ciphertext2[j][i]; 
    }
}

//ques 11
uint8_t *ka_xor_2 = xor_between_256bit_num(k_a_32bytes, 2);
uint8_t *ka_xor_1 = xor_between_256bit_num(k_a_32bytes, 1);
 uint8_t *ka_2_ma =  initialize_input(64);
 for(int  i = 0;i<32;i++)
 {
     ka_2_ma[i] = ka_xor_2[i];
 }
 for(int  i = 32;i<64;i++)
 {
     ka_2_ma[i] = Ma[i-32];
 }
   uint32_t *sha256_ka_2_ma = find_sha_256(64,ka_2_ma);
   uint8_t *sha256_ka_2_ma_in_8bit = convert32bit_to_8bitblock(sha256_ka_2_ma);
   uint8_t *ka_1_sha256_ka_2_ma= initialize_input(64);
   for(int  i =  0;i<32;i++)
    {
      ka_1_sha256_ka_2_ma[i] =  ka_xor_1[i];
    }
    for(int  i = 32;i<64;i++)
    {
        ka_1_sha256_ka_2_ma[i]  = sha256_ka_2_ma_in_8bit[i-32];
    }
   uint32_t *MACa =  find_sha_256(64, ka_1_sha256_ka_2_ma);


//ques 12

 printf("\n\n----------->Ca: \n\n");
 for(int i  = 0;i<32;i++)
 {
    printf("%"PRIx8" ", CA[i]);  
 }

 uint8_t *MACa_8 = convert32bit_to_8bitblock(MACa);
 printf("\n\n ----------->MACa: \n\n");

    for(int i = 0;i<32;i++)
    {
         printf("%"PRIx8" ", MACa_8[i]); 
    }

   //ques 13
     uint8_t *bob_passed_ca = initialize_input(32);
     for(int i = 0;i<32;i++)
     {
         bob_passed_ca[i] = CA[i];
     }
     uint32_t *bob_passed_MACa  = initialize_input32(8);
     for(int  i = 0;i<8;i++)
     {
         bob_passed_MACa[i] = MACa[i];
     }

     uint8_t **bob_passed_IV = initialize_matrix();
      for(int  i=0;i<4;i++)
        {
            for(int  j=0;j<4;j++)
            {
                bob_passed_IV[j][i] = IV[j][i];
            }
        }
//ques 14  
        uint8_t keyB[32];
            for(int i=0;i<32;i++)
            {
                keyB[i] = k_b_32bytes[i];
            }

        // array to store 60 word each of 32 bit
        uint32_t wb[60];

        // creating 60 words 
        make_w(wb, keyB);

        // 3d array to store 15 round keys each of 128 bit
        uint8_t round_keysB[15][4][4];

        // creating 15 round keys using 60 word
        make_round_keys(round_keysB, wb);

        uint8_t **decrypted_text1 =  decryption(round_keysB, ciphertext1);
        uint8_t **plaintext1_decrypted = xor_two_mat(decrypted_text1, IV);

        uint8_t **decrypted_text2 =  decryption(round_keysB, ciphertext2);
        uint8_t **plaintext2_decrypted = xor_two_mat(decrypted_text2, ciphertext1);

       uint8_t *Mb = initialize_input(32);
       cnt1 = 0;
        for(int  i = 0;i<4;i++)
        {
            for(int  j = 0;j<4;j++)
            {
                Mb[cnt1++] =  plaintext1_decrypted[j][i];
            }
        }

        for(int  i = 0;i<4;i++)
        {
            for(int  j = 0;j<4;j++)
            {
                 Mb[cnt1++] =  plaintext2_decrypted[j][i];
            }
        }
         
//ques 15

uint8_t *kb_xor_2 = xor_between_256bit_num(k_b_32bytes, 2);
uint8_t *kb_xor_1 = xor_between_256bit_num(k_b_32bytes, 1);

 uint8_t *kb_2_mb =  initialize_input(64);
 for(int  i = 0;i<32;i++)
 {
     kb_2_mb[i] = kb_xor_2[i];
 }
 for(int  i = 32;i<64;i++)
 {
     kb_2_mb[i] = Mb[i-32];
 }

  
    uint32_t *sha256_kb_2_mb = find_sha_256(64,kb_2_mb);
   uint8_t *sha256_kb_2_mb_in_8bit = convert32bit_to_8bitblock(sha256_kb_2_mb);
   uint8_t *kb_1_sha256_kb_2_mb= initialize_input(64);
   for(int  i =  0;i<32;i++)
    {
      kb_1_sha256_kb_2_mb[i] =  kb_xor_1[i];
    }
    for(int  i = 32;i<64;i++)
    {
        kb_1_sha256_kb_2_mb[i]  = sha256_kb_2_mb_in_8bit[i-32];
    }

   uint32_t *MACb =  find_sha_256(64, kb_1_sha256_kb_2_mb);

//ques 16
    printf("\n\n---------->Mb: \n\n");
    for(int i  = 0;i<32;i++)
    {
        printf("%"PRIx8" ", Mb[i]);  
    }
    uint8_t *MACb_8 = convert32bit_to_8bitblock(MACb);
    printf("\n\n ----------> MACb: \n\n");
    for(int i = 0;i<32;i++)
    {
         printf("%"PRIx8" ", MACb_8[i]); 
    }
     
}

