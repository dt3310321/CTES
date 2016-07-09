int  buf1[10];
void test(char * buf, char *sp){
   char s[10]; //stack
   char s1[20];
   int *buf2=(int*)malloc(10*sizeof(int));//heap
   char * s2=(char*)malloc(20*sizeof(char));
   int i=10;
   int j=i;
   int k=buf1[9]; //1读操作
    buf1[j-1]=0;    //2 写操作
    i=10;
    j=i+10;  //20
    k=j;     //20
    k=k-20+i;//10
    buf1[k-1]=100;//3 变量下标操作
    buf2[k-11+1]=100;//4 表达式小于
    buf1[k+10-i-1]=20;//5 表达式大于
    scanf("%d",&j);//污点输入
   if(j<10&&j>=0)
   buf1[j]=20;  //6污点1
   if(j+k<20&&j+k>=0)
   buf2[j+k]=20;//7污点2
    for(i=0;i<20;i++){
    	s1[i]='a';//8.简单循环变量
    	s2[2*i+k-30]='b';//9.循环表达式
          if(j+k<20&&j+k>=0)
    	s1[j+k]='c';//10.循环里面的污点
    }
//note1
//end
   int *p=buf1;
   p=p+9;
   *p=100;  //14简单指针
   *(p+0)=200; //15指针运算
  //note 2
 
  //end
s[s1[0]]='a';//21
s[*s2]='c';//22
   free(buf2);
   free(s2);
  return 0;
}

