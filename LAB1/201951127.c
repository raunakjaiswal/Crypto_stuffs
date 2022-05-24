 
// Raunak kumar jaiswal
//201951127

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdint.h>

char* take_input_string() //funtion to take string from input using dynamic memory allocation(malloc,realloc)
{
    char c;
    char *str;
    str=(char*)malloc(sizeof(char));
    int index = 0;
    int j = 1;
    while(scanf("%c", &c) )
    {
        if(c=='\n')
        break;
        str = (char*)realloc(str, j * sizeof(char));
        str[index] = c;
        index++;
        j++;
    }
    str[index] = '\0';
    return str;
}

void convertjtoi(int key_length, char* key) // funtion to convert any j to i
{
  for(int i=0;i<key_length;i++)
    {
        if(key[i]=='j')
        {
          key[i] = 'i';
        }
    }
}


// replacing j to i and if two adjacent character are same then adding 'x' in between them and at last if 
// length of string is odd then making it to even length by adding 'x' at last position.
void second_question_fun(char* str, char* plaintext, int len_string)
{
    int idx=0;
    int new_array_id= 0;
    while(idx<len_string)
    {
        if(plaintext[idx]=='j')
        {
            plaintext[idx] = 'i';
        }
        else if(idx==0)
        {
            str[new_array_id] = plaintext[idx];
            idx++;
            new_array_id++;
        }
        else if(plaintext[idx]==str[new_array_id-1])
        {
            str[new_array_id]='x';
            new_array_id++;
        }
        else
        {
            str[new_array_id] = plaintext[idx];
            new_array_id++;
            idx++;
        }
    }
    if(new_array_id%2!=0)
    {
      str[new_array_id] = 'x';
      str[++new_array_id] = '\0';
    }
    else
    {
         str[new_array_id] = '\0';
    }
}

void print_matrix(char matrix[5][5]) // funtion to print 5x5 key matrix
{
    for(int ll = 0;ll<5;ll++)
    {
        for(int kk=0;kk<5;kk++)
        {
            printf("%c ", matrix[ll][kk]);
        }
        printf("\n");
    }
    printf("\n");
}

// funtion to create 5x5 key matrix of the playfair cipher
void create_matrix(char matrix[5][5], int key_length,int arr[], int brr[], char key[]) 
{
     int  i = 0;
     int j = 0;
    for(int k=0;k<key_length;k++)
    {
        int char_idx= key[k]-'a';
        if(j==5)
        {
            i++;
            j = 0;
        }
        if(arr[char_idx]==-1)
        {
           matrix[i][j]=key[k];
           arr[char_idx] = i;
           brr[char_idx] = j;
           j++;
        }
    }
    for(int k=0;k<26;k++)
    {
        if(j==5)
        {
            i++;
            j=0;
        }
        if(k==9)
        continue;
        if(arr[k]==-1)
        {
           matrix[i][j] =  'a'+k;
             arr[k]= i;
             brr[k] = j;
            j++;
        }
    }
}

void caesar_encrypt(char c1[], int key, char* c2)
{
   int index =  0;
   while(c1[index]!='\0')
   {
       int in = c1[index]-'a';
       int new_in  = (in+key)%26;
       c2[index]  = new_in+'a';
       index++;
   }
   c2[index]='\0';
}

void caesar_decrypt(char c3de[], int key,char*c2de )
{
   int index =  0;
   while(c3de[index]!='\0')
   {
       int in = c3de[index]-'a';
       int new_in  = ((in-key)%26+26)%26;
       c2de[index]  = new_in+'a';
       index++;
   }
   c2de[index]='\0';
}

void affine_encrypt(char c2[], int key1,int key2, char* c3)
{
    int index =  0;
   while(c2[index]!='\0')
   {
       int in = c2[index]-'a';
       int new_in  = (key1*in+key2)%26;
       c3[index]  = new_in+'a';
       index++;
   }
   c3[index]='\0';
}

void affine_decrypt(char c3[], int key1,int key2,char* c3de)
{
   int a_inv;
   for(int i=0;i<26;i++)
   {
       if((key1*i)%26 ==1)
       {
           a_inv = i;
           break;
       }
   }
   int index = 0;
   while(c3[index]!='\0')
   {
       int in  = c3[index]-'a';
       int new_in = (((a_inv)*((in-key2))%26 +26))%26;
       c3de[index]= new_in+'a';
       index++;
   }
   c3de[index]='\0';
}

char* playfair_encrypt(char matrix[5][5] , int x_index[], int y_index[], char firstchar, char secondchar, char* ans)
{
   int x1 = x_index[firstchar-'a'];
   int y1 = y_index[firstchar-'a'];
   int x2 = x_index[secondchar-'a'];
   int y2 = y_index[secondchar-'a'];
  
  char pp1;
  char pp2;
   if(y1==y2)
   {
       pp1=matrix[(x1+1)%5][y1];
       pp2=matrix[(x2+1)%5][y2];
   }
   else if(x1==x2)
   {
        pp1=matrix[x1][(y1+1)%5];
        pp2=matrix[x2][(y2+1)%5];
   }
   else
   {
      pp1= matrix[x1][y2];
      pp2 = matrix[x2][y1];
   }
  ans[0] = pp1;
  ans[1] = pp2;
  return ans;
}

char* playfair_decrypt(char matrix[5][5] , int x_index[], int y_index[], char firstchar, char secondchar, char* ans)
{
   int x1 = x_index[firstchar-'a'];
   int y1 = y_index[firstchar-'a'];
   int x2 = x_index[secondchar-'a'];
   int y2 = y_index[secondchar-'a'];
   char pp1;
  char pp2;
   if(y1==y2)
   {
       pp1=matrix[(x1-1+5)%5][y1];
       pp2=matrix[(x2-1+5)%5][y2];
   }
   else if(x1==x2)
   {
        pp1=matrix[x1][(y1-1+5)%5];
        pp2=matrix[x2][(y2-1+5)%5];
   }
   else
   {
      pp1= matrix[x1][y2];
      pp2 = matrix[x2][y1];
   }
  ans[0] = pp1;
  ans[1] = pp2;
  return ans;
}
int main()
{
    // 1. Consider  word (without any space) as a plaintext.
    
    printf("Enter string without space\n");
    char *plaintext = take_input_string();
    int len_string = 0;
    while(plaintext[len_string]!='\0')
    {
        len_string++;
    }
    printf("plaintext: %s\n",plaintext);

//2. Adjust the length of the plaintext and handle the repetition of letter (if present) according to the
// rule of the Playfair cipher. If there is any J in the word convert it to I. Let the final word be ∆.

    int len = len_string*2+2;
    char str[len];
    second_question_fun(str, plaintext, len_string);

//3. Print the word ∆.
    printf("delta:   %s\n",str);

//4. Consider a word as input and convert J (if any) to I and consider that converted word as the key
// K1. (Input)

    printf("Enter key string \n");
    char *key = take_input_string();
    int key_length = 0;
    while(key[key_length]!='\0')
    {
        key_length++;
    }
    
    convertjtoi(key_length,key); //converting any j to i 
    printf("key k1: %s\n",key);

    //5. Generate the 5 × 5 key matrix of the Playfair cipher from K1 and print it. (Output)
    char matrix[5][5];
    int arr[26]={-1};
    int brr[26]={-1};
for(int kk=0;kk<26;kk++)
{
    arr[kk] =  -1;
    brr[kk]  =-1;
}
create_matrix(matrix,key_length,arr,brr, key);
printf("\n5x5 key matrix:\n");
print_matrix(matrix);   


//6. Encrypt ∆ using the Playfair cipher where the key is K1. Let the ciphertext be C1. Print the
// ciphertext C1. (Output)
char c1[len];
char ans[2];
int index1 =0;

while(str[index1]!='\0')
{
    char* ptr  = playfair_encrypt(matrix,arr,brr,str[index1], str[index1+1],ans);
    c1[index1]= ptr[0];
    c1[index1+1] = ptr[1];
    index1 = index1+2;
}
c1[index1]='\0';
printf("ciphertext c1: %s\n",c1);

//7. Encrypt C1 using the CAESAR cipher where the key is K2 = 3. Let the ciphertext be C2. Print
//the ciphertext C2. (Output)
char c2[len];
caesar_encrypt(c1,3,c2);
printf("ciphertext c2: %s\n",c2);

//8. Encrypt C2 using the Affine cipher where the key is K3 = (7, 5) ∈ Z26 × Z26. Let the ciphertext be
//C3. Print the ciphertext C3. (Output)
char c3[len];
affine_encrypt(c2,7,5,c3);
printf("ciphertext c3: %s\n",c3);


// decryption part 

char c3de[len]; 
affine_decrypt(c3,7,5,c3de);
printf("decrypttext d3: %s\n",c3de);

char c2de[len];
caesar_decrypt(c3de,3,c2de);
printf("decrypttext d2: %s\n",c2de);

char c1de[len];
char ans2[2];
int index2 =0;


while(c2de[index2]!='\0')
{
    char* ptr  = playfair_decrypt(matrix,arr,brr,c2de[index2], c2de[index2+1],ans2);
    c1de[index2]= ptr[0];
    c1de[index2+1] = ptr[1];
    index2 = index2+2;
}
c1de[index2]='\0';
printf("decrypttext d1: %s\n",c1de);
printf("d1 is equal to delta\n");

}