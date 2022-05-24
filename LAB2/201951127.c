#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// #include <stdint.h>
#include <inttypes.h>

// function to initialize Initial permutation(IP)
void initialize_IP(int IP[8][8])
{
    int num = 58;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            IP[i][j] = (num-(j*8));
        }
        if(i==3)
            num = 57;
        else
            num =num+2;
    }
}

// function to initialize Inverse Initial permutation(IP)
void initialize_IP_inv(int IP_inv[8][8])
{
    int num1 = 40;
    int num2 = 8;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(i%2==0)
            IP_inv[j][i] = (num1-j);
            else
            IP_inv[j][i] = (num2-j);
        }
        if(i%2==0)
            num1= num1 + 8;
        else
            num2 = num2 + 8;
    }
}

void print_8x8( int IP[8][8])
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            printf("%d ",IP[i][j]);
        }
        printf("\n");
    }
}

// function to convert planetext(64 bit) to IP(64 bit)
uint64_t convert_text_to_IP(uint64_t plane_text,int IP[8][8])
{
   uint64_t plane_text_to_IP= 0;
   int count = 63;
   for(int i=0;i<8;i++)
   {
       for(int j=0;j<8;j++)
       {
          int bitposition = 64 - IP[i][j];
          int bit = (plane_text>>bitposition)& 1;
          unsigned long long int neww = bit;
          neww = (neww<<count);
          plane_text_to_IP = plane_text_to_IP | neww;
          count--;
       }
   }
return plane_text_to_IP;
}

// function to convert final key(64 bit) to IP_inv(64 bit)
uint64_t convert_text_to_IP_inv(uint64_t final_round,int IP_inv[8][8])
{
   uint64_t cipher_text= 0;
   int count = 63;
   for(int i=0;i<8;i++)
   {
       for(int j=0;j<8;j++)
       {
          int bitposition = 64 - IP_inv[i][j];
          int bit = (final_round>>bitposition)& 1;
          unsigned long long int neww = bit;
          neww = (neww<<count);
          cipher_text = cipher_text| neww;
          count--;
       }
   }
return cipher_text;
}

// function to initialize exapnsion function
void initialize_Expansion(int E[8][6])
{
    int num = 0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<6;j++)
        {
           if(i==0 && j==0)
           {
               E[i][j] = 32;
           }
           else
           {
               E[i][j] = num+j;
           }
        }
        num =num+4;
       
    }
    E[7][5] = 1;
}

// function to print E (8X6)
void print_Expansion( int E[8][6])
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<6;j++)
        {
            printf("%d ",E[i][j]);
        }
        printf("\n");
    }
}

//function  to expand (32 bit to 48 bit)
uint64_t expansion_function(uint32_t right, int E[8][6])
{
    uint64_t bit48= 0;
    int count = 47;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<6;j++)
        {
          int bitposition = 32-E[i][j];
          int bit = (right>>bitposition)& 1;
          unsigned long long int neww = bit;
          neww = (neww<<count);
          bit48 = bit48 | neww;
          count--;
        }
    }
return bit48;
}

//function to convert 48bit to 8 block each of 6 bit
void bit48_to_each_6bit(uint64_t  bit48, uint8_t bit6[8])
{
   
   for(int i=0;i<8;i++)
   bit6[i] = 0;
   int idx =  0;
   for(int i=47;i>=0;i=i-6)
   {
        int count = 5;
        for(int j=i;j>i-6;j--)
        {
           int bitposition = j;
           int bit= (bit48>>bitposition)&1;
           unsigned long long int neww=bit;
           neww = (neww<<count);
           bit6[idx] = bit6[idx] | neww;
           count--;
        }
        idx++;
   }
  
} 

uint8_t bit6_to_bit4(int s_box[8][4][16], uint8_t bit6,int s_index)
{
   uint8_t bit4= 0;
   int b1 = ((1<<5) & bit6)>0 ? 1:0;
   int b6 = ((1<<0) & bit6)>0 ? 1:0;
   int r = (2*b1+b6);
   int c = 0;
   int two  = 1;
   for(int i=1;i<=4;i++)
   {
       int bit = ((1<<i) & bit6)>0 ?1:0;
       c = c  + bit*(two);
       two = two*2;
   }
   bit4 = s_box[s_index][r][c];
   return bit4;
}

// function to covert array of 6bit to array of 4bit
void bit6_array_to_each_4bit(uint8_t bit6[8], int s_box[8][4][16], uint8_t bit4[8])
{
   for(int i=0;i<8;i++)
   {
       bit4[i] = bit6_to_bit4(s_box, bit6[i], i);
   }
}

//function to combine 8 block of 4 bit each into 32 bit single block
uint32_t array_of_4bit_to_32_bit(uint8_t bit4[8])
{
  uint32_t s = 0;
  int idx = 31;
  for(int i=0;i<8;i++)
  {
     uint8_t each_bit4 = bit4[i];
     for(int j=1;j<=4;j++)
     {  
        int bit = ((1<<(4-j))&each_bit4)>0 ? 1:0;
        if(bit==1)
        {
            unsigned long long int tm= (1<<idx);
            s = s|tm;
        }
       idx--;
     }
  }
return s;
}

// function s (32bit) to P(32 bit)
uint32_t permutation_of_s(int P[8][4], uint32_t s)
{
   uint32_t p = 0;
    int count = 31;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<4;j++)
        {
          int bitposition = 32-P[i][j];
          int bit = (s>>bitposition)& 1;
          unsigned long long int neww = bit;
          neww = (neww<<count);
          p = p | neww;
          count--;
        }
    }
   return p;
}

uint64_t pc1_function(int pc1[8][7], uint64_t key)
{
    uint64_t t=0;
    int  count=  55;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<7;j++)
        {
           int bitposition = pc1[i][j];
           bitposition = 63 - bitposition;
           int shiftt = bitposition/8;
           bitposition = bitposition-shiftt;
           int bit = (key>>bitposition)& 1;
           unsigned long long int neww = bit;
            if(bit==1)
            {
               neww = (neww<<count);
               t= t | neww;
            }
          count--;
        }
    }
    return t;

}


uint32_t left_circular_shift(uint32_t left , int by)
{
  uint32_t after_rotation = 0;
  after_rotation= (left<<by) | (left>>(28-by));
  after_rotation = after_rotation & 0xFFFFFFF;
  return after_rotation;
}

// 56 bit to 48 bit
uint64_t pc2_function(int pc2[8][6], uint64_t k)
{
    uint64_t key= 0;
    int count = 47;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<6;j++)
        {
            int bitposition = pc2[i][j];
            bitposition = 56  - bitposition;
            int bit=  (k>>bitposition)& 1;
            unsigned long long int neww = bit;
            if(bit==1)
            {
               neww = (neww<<count);
               key= key | neww;
            }
           count--;
        }
    }
return key;
}

void find_all_c_and_d(uint32_t c[17], uint32_t d[17], uint64_t k[17])
{
  for(int i=1;i<=16;i++)
  {
      int v = 2;
      if(i==1 || i==2 || i==9|| i==16)
      v = 1;
      c[i]  = left_circular_shift(c[i-1], v);
      d[i] = left_circular_shift(d[i-1],v);
      k[i] = 0;
      k[i]  = c[i];
      k[i] =(k[i]<<28);
      k[i] = k[i] | d[i];

  }
}

uint32_t main_fun(int E[8][6],int s_box[8][4][16],int P[8][4], uint32_t r, uint64_t key )
{

    uint64_t new_r48bit =  expansion_function(r, E); // 32 to 48
    uint64_t xor48bit =   (new_r48bit ^ key) ;  //xor with key
    //substitution box (48 bit to 32 bit)
    uint8_t bit6[8];
    bit48_to_each_6bit(xor48bit, bit6);
    uint8_t bit4[8];
    bit6_array_to_each_4bit(bit6, s_box, bit4);
    uint32_t s= array_of_4bit_to_32_bit(bit4);
 // permuattion
   uint32_t p =  permutation_of_s(P, s);
return p;
}

//  des encryption round function
uint64_t round_recursion_function(int round_no, uint32_t r, uint32_t l, uint64_t finalkey[17], int E[8][6], int s_box[8][4][16], int P[8][4])
{
  if(round_no==16)
  {
      uint32_t l1 = main_fun(E,s_box,P, r,finalkey[round_no])^l;
      uint32_t r1 = r;
      uint64_t round= l1;
     round = (round<<32) + r1;

     printf("Round %d: %"PRIu64"\n", round_no,round);
     return round;
  }
  else
  {
     uint32_t l1= r;
     uint32_t r1 = main_fun(E,s_box,P, r,finalkey[round_no])^l;
     uint64_t round= l1;
     round = (round<<32) + r1;
     printf("Round %d: %"PRIu64"\n", round_no,round);
     if(round_no<=15)
    return round_recursion_function(round_no+1, r1,l1,finalkey, E, s_box,P);
  }
}

//  des decryption round function
uint64_t dec_round_recursion_function(int round_no, uint32_t r, uint32_t l, uint64_t finalkey[17], int E[8][6], int s_box[8][4][16], int P[8][4])
{
  if(round_no==1)
  {
      uint32_t l1 = main_fun(E,s_box,P, r,finalkey[round_no])^l;
      uint32_t r1 = r;
      uint64_t round= l1;
     round = (round<<32) + r1;

    printf("Round %d: %"PRIu64"\n", (16-round_no+1),round);
     return round;
  }
  else
  {
     uint32_t l1= r;
     uint32_t r1 = main_fun(E,s_box,P, r,finalkey[round_no])^l;
     uint64_t round= l1;
     round = (round<<32) + r1;
    printf("Round %d: %"PRIu64"\n", (16-round_no+1),round);
     if(round_no>=2)
    return dec_round_recursion_function(round_no-1, r1,l1,finalkey, E, s_box,P);
  }
}

int main()
{
    uint64_t plane_text;
    printf("Enter a Plain text:\n");
    scanf("%lld",&plane_text);
    
    uint64_t key;
    printf("Enter 56 bit key:\n");
    scanf("%lld", &key);
    
    int IP[8][8];
    int IP_inv[8][8];
    initialize_IP(IP);
    initialize_IP_inv(IP_inv);

    uint64_t plane_text_to_IP;
     
    plane_text_to_IP = convert_text_to_IP(plane_text, IP);
    printf("plain text after initial permutation IP: %llu\n", plane_text_to_IP);

    uint32_t l0 = (plane_text_to_IP>>32);
    uint32_t r0 = (plane_text_to_IP &  0xFFFFFFFF);
    printf("L0: %llu\n",l0);
    printf("R0: %llu\n",r0);
    
//initializing function
int E[8][6];
initialize_Expansion(E);
int s_box[8][4][16] = { 
    {
    14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
    4,1,14,8,13,6,2,11,15,12,9,7,3,10, 5,0,15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
    },
    {
     15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
     0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
    },
    {
        10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
        13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
    },
    {
        7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
        10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
    },
    {
        2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
        4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
    },
    {
        12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
        9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
    },
    {
        4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
        1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
    },
    {
        13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
        7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
    }
   };
int P[8][4]={16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,
  10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};

// key sheduling algo

int PC1[8][7] = {
        57,49, 41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,
      63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4,  
};

uint64_t T;
T = pc1_function(PC1,key);

uint32_t c0 = (T>>28);
uint32_t d0 = (T & 0xFFFFFFF);
uint32_t c[17];
uint32_t d[17];
c[0] = c0;
d[0] = d0;
uint64_t k[17];
find_all_c_and_d(c,d, k);
int PC2[8][6] = {
    14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,
    30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32
};
uint64_t finalkey[17];
printf("\nKey generation started----\n");
for(int i=1;i<=16;i++)
{
    finalkey[i] = pc2_function(PC2,k[i]);
    printf("keys %d: %"PRIu64"\n",i ,finalkey[i]);
} 

printf("\n*********Encryption********\n");
printf("\n--DES encryption round---\n");
uint64_t  last_round =  round_recursion_function(1, r0,l0,finalkey, E, s_box,P);


uint64_t cipher_text = convert_text_to_IP_inv(last_round, IP_inv);
printf("cipher text:\n %"PRIu64"\n",cipher_text);



printf("\n*********Decryption*********\n");
// decryption
uint64_t ip_de= convert_text_to_IP(cipher_text, IP);
printf("\ncipher text after initial permutation:\n %"PRIu64"\n",ip_de);

uint32_t ll0 = (ip_de>>32);
uint32_t rr0 = (ip_de &  0xFFFFFFFF);

printf("L0: %llu\n",ll0);
printf("R0: %llu\n",rr0);
    
printf("---DES Decryption round---\n");
uint64_t tt_d=  dec_round_recursion_function(16,rr0, ll0, finalkey,E,s_box,P);


uint64_t pl_de= convert_text_to_IP_inv(tt_d, IP_inv);
printf("Decrypted cipher text: \n%"PRIu64"\n",pl_de);

printf("\n---Now verify--\n");
printf("\nplain text: %llu\n",plane_text);
printf("cipher text: %llu\n",cipher_text);
printf("Decrypted cipher text: %"PRIu64"\n",pl_de);


}