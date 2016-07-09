
void test(char * buf, char *result,int n){
   char s[10]={0}; //stack
   char s1[20]={0};
   int *buf2=(int*)malloc(10*sizeof(int));//heap
   char * s2=(char*)malloc(20*sizeof(char));

    char * pbuf=buf;
    char *end;
     char c;
/*
    while((c=*pbuf++)!='\0'){
       if(c=="("||c==")"){
       if(s2<end)
       *s2++=' ';
      }    //1
       else{
        if(s2<end)
      *s2++=c;      //2
     }
       if(c==" "){
        if(s2<end)
       *s2++='a'; //3
}
   }
*/
   *s2='\0';  //4
  if(strlen(s2)>n)
  return;
  strcpy(result,s2);//5
  if(s[0]=='0'){
   if(strlen(result)+1>n)
  return;
  strcat(result,";"); //6
  }
   else if (s[0]=='1'){
     if(strlen(result)+strlen(buf)>n)
  return;
   strcat(result,buf); //7
   }
    else
        if(n<strlen("123456"))
  return;
   strcpy(result,"123456");//8
        if(3+strlen(result)>n)
  return;
   strcat(result,"123");//9
  if(strlen(result)+strlen(s2)>n)
   return ;
   strcat(result,s2);//10
           if(n>10)
  return;
   memcpy(s1,buf,n);//11
   free(buf2);
   free(s2);
  return 0;
}

int main(){
return 0;
}
