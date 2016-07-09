int  buf1[10];
void test(){
   char s[10]; //stack
   char s1[20];
   int *buf2=(int*)malloc(10*sizeof(int));//heap
   char * s2=(char*)malloc(20*sizeof(char));
   int i=10;
   int j=i;
   int k=10; //1读操作
    buf1[j]=0;    //2 写操作
    i=10;
    j=i+10;  //20
    k=j;     //20
    k=k-20+i;//10
    buf1[k]=100;//3 变量下标操作
    buf2[k-11]=100;//4 表达式小于
    buf1[k+10-i]=20;//5 表达式大于
    scanf("%d",&j);//污点输入
   buf1[j]=20;  //6污点1
   buf2[j+k]=20;//7污点2
    for(i=0;i<20;i++){
    	s1[i]='a';//8.简单循环变量
    	s2[2*i+100+k]='b';//9.循环表达式
    	s1[j+k+100]='c';//10.循环里面的污点
    }
//note1
    
//end
 int *p=buf1;
   p=p+10;
   *p=100;  //14简单指针
   *(p+10)=200; //15指针运算
s[buf1[0]]='a';//21
//s[*p]='c';//22
   free(buf2);
   free(s2);
  return 0;
}

