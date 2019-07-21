#define min(x,y,z) ((x < y) ? ((z < x)? z : x) : ((z < y)? z : y))
#define check_char(x,y) (((x == y) || ((x == y + 32) && (x <= 'z' && x >= 'a' && y >= 'A' && y <= 'Z'))  || ((x == y - 32) && (x <= 'Z' && x >= 'A' && y >= 'a' && y <= 'z'))) ? 1 : 0)
#define M 53
/* by value */

int hash(char c){
    return (((c > 'a' && c < 'z') ? c - 'a' : c - 'A') + 'a' ) % M;
}


static int ref1[M];
static int ref2[M];
static int index1[100];
static int index2[100];

Datum jaccard_index(PG_FUNCTION_ARGS)
{
	text * t1 = PG_GETARG_DATUM(0);
	text * t2 = PG_GETARG_DATUM(1);
	char * s1 = text_to_cstring(t1);
	char * s2 = text_to_cstring(t2);
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    
    memset(ref1,0,sizeof(ref1));
    memset(ref2,0,sizeof(ref2));
    memset(index1,0,sizeof(index1));
    memset(index2,0,sizeof(index2));
    
    int start1,start2,i,j,k;
    int countu = 0;
    int countn = 0;
  
    for(i = 0; i <= l1; i ++){
        if(i == 0 || i == l1) countn ++;
        else{
            start1 = ref1[hash(s1[i - 1])];
            if(start1 == 0) {ref1[hash(s1[i - 1])] = i; countn ++;}
            else{
                for(j = start1; ; j = index1[j - 1]){
                    if(j == 0 || (check_char(s1[j - 1], s1[i - 1]) && check_char(s1[j], s1[i])))
                        break;
                }
                if(j == 0){
                    index1[i - 1] = start1;
                    ref1[hash(s1[i - 1])] = i;
                    countn ++;
                }
            }
        }
    }
    
    for(k = 0; k <= l2; k ++){
        if(k == 0){
            if(check_char(s2[0], s1[0])) countu ++;
            else countn ++;
        }
        else if(k == l2){
            if(check_char(s1[l1 - 1], s2[l2 - 1])) countu ++;
            else countn ++;
        }
        else{
            start1 = ref1[hash(s2[k - 1])];
            start2 = ref2[hash(s2[k - 1])];
            if(start2 == 0) {ref2[hash(s2[k - 1])] = k;}
            else{
                for(j = start2; ; j = index2[j - 1]){
                    if(j == 0 || (check_char(s2[j - 1], s2[k - 1]) && check_char(s2[j], s2[k]))) break;
                }
                if(j == 0){
                    index2[k - 1] = start2;
                    ref2[hash(s2[k - 1])] = k;
                }
            }
            if(ref2[hash(s2[k - 1])] == k){
                if(start1 != 0){
                    for(j = start1; ; j = index1[j - 1]){
                        if(j == 0) {
                            countn ++;
                            break;
                        }
                        else if (check_char(s1[j - 1], s2[k - 1]) && check_char(s1[j], s2[k])){
                            countu ++;
                            break;
                        }
                    }
                }
            }
        }
    }
    
	PG_RETURN_FLOAT4((float)countu / countn);
}


