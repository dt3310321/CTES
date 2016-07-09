
void test(char * buf, char *result,int n){
   char s[10]={0}; //stack
   char s1[20]={0};
   int *buf2=(int*)malloc(10*sizeof(int));//heap
   char * s2=(char*)malloc(20*sizeof(char));

    char * pbuf=buf;
     char c;
    while((c=*pbuf++)!='\0'){
       if(c=="("||c==")")
       *s2++=' ';    //1
       else{
      *s2++=c;      //2
     }
       if(c==" ")
       *s2++='a'; //3
   }
   *s2='\0';  //4
  strcpy(result,s2);//5
  if(s[0]=='0'){
  strcat(result,";"); //6
  }
   else if (s[0]=='1'){
   strcat(result,buf); //7
   }
    else
   strcpy(result,"123456");//8
   strcat(result,"123");//9
   strncpy(result,s2,n);//10
   memcpy(s1,buf,n);//11
   free(buf2);
   free(s2);
  return 0;
}

int main(){
return 0;
}
