//quicksort and static 

#include <stdbool.h>
#define min(x,y,z) ((x < y) ? ((z < x)? z : x) : ((z < y)? z : y))
#define check_char(x,y) (((x == y) || ((x == y + 32) && (x <= 'z' && x >= 'a' && y >= 'A' && y <= 'Z'))  || ((x == y - 32) && (x <= 'Z' && x >= 'A' && y >= 'a' && y <= 'z'))) ? 1 : 0)


bool less(char x, char y){
    if(x <= 'Z' && x >= 'A' && y >= 'a' && y <= 'z')
            return x < y - 'a' + 'A';
    else if(x <= 'z' && x >= 'a' && y >= 'A' && y <= 'Z')
            return x < y - 'A' + 'a';
    else
            return x < y;
}


bool greater(char x, char y){
    if(x <= 'Z' && x >= 'A' && y >= 'a' && y <= 'z')
        return x > y - 'a' + 'A';
    else if(x <= 'z' && x >= 'a' && y >= 'A' && y <= 'Z')
        return x > y - 'A' + 'a';
    else
        return x > y;
}
            
static int sort1[100];
static int sort2[100];

void swap_one(int a,int b)
{
    int temp;
    temp = sort1[a];
    sort1[a] = sort1[b];
    sort1[b] = temp;
}


void swap_two(int a,int b)
{
    int temp;
    temp = sort2[a];
    sort2[a] = sort2[b];
    sort2[b] = temp;
}


void quickSort_one(const char* s, int start, int end){
    int Base, arrMiddle;
    int tempStart = start, tempEnd = end;
    if(tempStart >= tempEnd) return;
    Base = sort1[start];
    while(start < end){
        while(start < end && (greater(s[sort1[end]], s[Base]) || (check_char(s[sort1[end]], s[Base]) && greater(s[sort1[end] + 1], s[Base + 1]))))
            end --;
        if(start < end){
            swap_one(start, end);
            start ++;
        }
        while(start < end && (less(s[sort1[start]], s[Base]) || (check_char(s[sort1[start]], s[Base]) && less(s[sort1[start] + 1], s[Base + 1]))))
            start ++;
        if(start < end){
            swap_one(start, end);
            end --;
        }
    }
    sort1[start] = Base;
    arrMiddle = start;
    quickSort_one(s, tempStart, arrMiddle - 1);
    quickSort_one(s, arrMiddle + 1, tempEnd);
    
}

void quickSort_two(const char* s, int start, int end){
    int Base, arrMiddle;
    int tempStart = start, tempEnd = end;
    if(tempStart >= tempEnd) return;
    Base = sort2[start];
    while(start < end){
        while(start < end && (greater(s[sort2[end]], s[Base]) || (check_char(s[sort2[end]], s[Base]) && greater(s[sort2[end] + 1], s[Base + 1]))))
            end --;
        if(start < end){
            swap_two(start, end);
            start ++;
        }
        while(start < end && (less(s[sort2[start]], s[Base]) || (check_char(s[sort2[start]], s[Base]) && less(s[sort2[start] + 1], s[Base + 1]))))
            start ++;
        if(start < end){
            swap_two(start, end);
            end --;
        }
    }
    sort2[start] = Base;
    arrMiddle = start;
    quickSort_two(s, tempStart, arrMiddle - 1);
    quickSort_two(s, arrMiddle + 1, tempEnd);
    
}

Datum jaccard_index(PG_FUNCTION_ARGS)
{
    const text* t1 = PG_GETARG_TEXT_P(0);
    const text* t2 = PG_GETARG_TEXT_P(1);

    char* s1 = text_to_cstring(t1); 
    char* s2 = text_to_cstring(t2);
  
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    int i, j;
    int countu = 0;
    int countn = 0;
    
    for(i = 0; i < l1 - 1; i ++)
        sort1[i] = i;
    for(i = 0; i < l2 - 1; i ++)
        sort2[i] = i;

    quickSort_one(s1, 0, l1 - 2);
    quickSort_two(s2, 0, l2 - 2);
    
    for(i = 0,j = 0; ;){
        while(i < l1 - 2 && check_char(s1[sort1[i]],s1[sort1[i + 1]]) && check_char(s1[sort1[i] + 1], s1[sort1[i + 1] + 1])) i ++;
        while(j < l2 - 2 && check_char(s2[sort2[j]],s2[sort2[j + 1]]) && check_char(s2[sort2[j] + 1], s2[sort2[j + 1] + 1])) j ++;
        if(i != l1 - 1 && j != l2 - 1 && check_char(s1[sort1[i]],s2[sort2[j]]) && check_char(s1[sort1[i] + 1], s2[sort2[j] + 1])){
            countu ++;
            countn ++;
            i ++;
            j ++;
        }
        else if(i != l1 - 1 && j != l2 - 1 && (less(s1[sort1[i]],s2[sort2[j]]) || (check_char(s1[sort1[i]],s2[sort2[j]]) && less(s1[sort1[i] + 1], s2[sort2[j] + 1])))){
            countn ++;
            i ++;
        }
        else if(i != l1 - 1 && j != l2 - 1){
            countn ++;
            j ++;
        }
        else if(i == l1 - 1 && j != l2 - 1){
            countn ++;
            j ++;
        }
        else if(i != l1 - 1 && j == l2 - 1){
            countn ++;
            i ++;
        }
        else break;
    }
    
    if(check_char(s1[0],s2[0])){
        countu ++;
        countn ++;
    }
    else countn += 2;
    
    if(check_char(s1[l1 - 1],s2[l2 - 1])){
        countu ++;
        countn ++;
    }
    else countn += 2;
    
    PG_RETURN_FLOAT4((float)countu / countn);

}

