#define min(x,y,z) ((x < y) ? ((z < x)? z : x) : ((z < y)? z : y))
#define check_char(x,y) (((x == y) || ((x == y + 32) && (x <= 'z' && x >= 'a' && y >= 'A' && y <= 'Z'))  || ((x == y - 32) && (x <= 'Z' && x >= 'A' && y >= 'a' && y <= 'z'))) ? 1 : 0)

Datum jaccard_index(PG_FUNCTION_ARGS)
{
 const text* t1 = PG_GETARG_TEXT_P(0);
 const text* t2 = PG_GETARG_TEXT_P(1);

 char* s1 = text_to_cstring(t1); 
 char* s2 = text_to_cstring(t2);

 int l1 = strlen(s1);
 int l2 = strlen(s2); 

 int countu = 0;
 int countn = 0;
 int i, j, k;

 for(i = 0; i <= l1; i ++){
  if(i == 0 || i == l1) countn ++;
  else{
   for(j = 1; j < i; j ++){
    if(check_char(s1[j - 1], s1[i - 1]) && check_char(s1[j], s1[i])) break;
   }
   if(j == i) countn ++;
  }
 }

    for(i = 0; i <= l2; i ++){
        if(i == 0){
            if(check_char(s1[0],s2[0])) countu ++;
            else countn ++;
        }
        else if(i == l2){
            if(check_char(s1[l1 - 1],s2[l2 - 1])) countu ++;
            else countn ++;
        }
        else{
            for(j = 1; j < i; j ++){
                if(check_char(s2[j - 1], s2[i - 1]) && check_char(s2[j], s2[i])) break;
            }
            if(j == i){
                for(k = 0; k < l1 - 1; k ++){
                    if(check_char(s1[k], s2[i - 1]) && check_char(s1[k + 1], s2[i])){
                        countu ++;
                        break;
                    }
                }
                if(k == l1 - 1) countn ++;
            }
        }
    }
 PG_RETURN_FLOAT4((float)countu / countn);

}