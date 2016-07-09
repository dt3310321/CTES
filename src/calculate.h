#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <cstdlib>
#include <cctype>
#include <cstring>
using namespace std;

int caculate_flag=0;

bool judge_nub(string & str);

vector<string> preParse(char *str)   //对中缀表达式进行预处理，分离出每个token
{
    vector<string> tokens;
    int len = strlen(str);
    char *p = (char *)malloc((len+1)*sizeof(char));  //注意不要用 char *p = (char *)malloc(sizeof(str))来申请空间
    int i=0,j=0;
    while(i<len)          //去除表达式中的空格
    {
        if(str[i]==' ')
        {
            i++;
            continue;
        }
        p[j++] = str[i++];
    }
    p[j]='\0';
    j=0;
    len = strlen(p);
    while(j<len)
    {
        char temp[2];
        string token;
        switch(p[j])
        {
            case '+':
            case '*':
            case '/':
            case '(':
            case ')':
                {
                    temp[0] =p[j];
                    temp[1] = '\0';
                    token=temp;
                    tokens.push_back(token);
                    break;
                }
            case '-':
                {
                    if(p[j-1]==')'||isdigit(p[j-1]))  //作为减号使用
                    {
                        temp[0] =p[j];
                        temp[1] = '\0';
                        token=temp;
                        tokens.push_back(token);
                    }
                    else    //作为负号使用
                    {
                        temp[0] ='#';
                        temp[1] = '\0';
                        token=temp;
                        tokens.push_back(token);
                    }
                    break;
                }
            default:     //是数字
                {
                    i = j;
                    while(isdigit(p[i])&&i<len)
                    {
                        i++;
                    }
                    if(i!=j){
                    char *opd = (char *)malloc(i-j+1);
                    strncpy(opd,p+j,i-j);
                    opd[i-j]='\0';
                    token=opd;
                    tokens.push_back(token);
                    j=i-1;
                    free(opd);
                        break;
                    }
                      else{
                      tokens.clear();
                      return tokens;
                      }
                }
        }
        j++;
    }
    free(p);
    return tokens;
}

int getPriority(string opt)
{
    int priority;
    if(opt=="#")
        priority = 3;
    else if(opt=="*"||opt=="/")
        priority = 2;
    else if(opt=="+"||opt=="-")
        priority = 1;
    else if(opt=="(")
        priority = 0;
    return priority;
}


void calculate(stack<int> &opdStack,string opt)
{
    if(opt=="#")  //进行负号运算
    {
        int opd = opdStack.top();
        int result = 0-opd;
        opdStack.pop();
        opdStack.push(result);
      //  cout<<"操作符:"<<opt<<" "<<"操作数:"<<opd<<endl;
    }
    else if(opt=="+")
    {
        int rOpd = opdStack.top();
        opdStack.pop();
        int lOpd = opdStack.top();
        opdStack.pop();
        int result = lOpd + rOpd;
        opdStack.push(result);

      //  cout<<"操作符:"<<opt<<" "<<"操作数:"<<lOpd<<" "<<rOpd<<endl;
    }
    else if(opt=="-")
    {
        int rOpd = opdStack.top();
        opdStack.pop();
        int lOpd = opdStack.top();
        opdStack.pop();
        int result = lOpd - rOpd;
        opdStack.push(result);
    //    cout<<"操作符:"<<opt<<" "<<"操作数:"<<lOpd<<" "<<rOpd<<endl;
    }
    else if(opt=="*")
    {
        int rOpd = opdStack.top();
        opdStack.pop();
        int lOpd = opdStack.top();
        opdStack.pop();
        int result = lOpd * rOpd;
        opdStack.push(result);
     //   cout<<"操作符:"<<opt<<" "<<"操作数:"<<lOpd<<" "<<rOpd<<endl;
    }
    else if(opt=="/")
    {
        int rOpd = opdStack.top();
        opdStack.pop();
        int lOpd = opdStack.top();
        opdStack.pop();
        int result = lOpd / rOpd;
        opdStack.push(result);
       //cout<<"操作符:"<<opt<<" "<<"操作数:"<<lOpd<<" "<<rOpd<<endl;
    }
}

int evaMidExpression(char *str)   //中缀表达式直接求值
{
    caculate_flag=0;
    vector<string> tokens = preParse(str);
     if(0==tokens.size()){
     //std::cout<<"clear"<<std::endl;
     caculate_flag=1;
     return 0;
    }
    int i=0;
    int size = tokens.size();

    stack<int> opdStack;     //存储操作数
    stack<string> optStack;   //存储操作符
    for(i=0;i<size;i++)
    {
        string token = tokens[i];
        if(token=="#"||token=="+"||token=="-"||token=="*"||token=="/")
        {
            if(optStack.size()==0)   //如果操作符栈为空
            {
                optStack.push(token);
            }
            else
            {
                int tokenPriority = getPriority(token);
                string topOpt = optStack.top();
                int topOptPriority = getPriority(topOpt);
                if(tokenPriority>topOptPriority)
                {
                    optStack.push(token);
                }
                else
                {
                    while(tokenPriority<=topOptPriority)
                    {
                        optStack.pop();
                        calculate(opdStack,topOpt);
                        if(optStack.size()>0)
                        {
                            topOpt = optStack.top();
                            topOptPriority = getPriority(topOpt);
                        }
                        else
                            break;

                    }
                    optStack.push(token);
                }
            }
        }
        else if(token=="(")
        {
            optStack.push(token);
        }
        else if(token==")")
        {
            while(optStack.top()!="(")
            {
                string topOpt = optStack.top();
                calculate(opdStack,topOpt);
                optStack.pop();
            }
            optStack.pop();
        }
        else
        {
            opdStack.push(atoi(token.c_str()));
        }
    }
    while(optStack.size()!=0)
    {
        string topOpt = optStack.top();
        calculate(opdStack,topOpt);
        optStack.pop();
    }
    return opdStack.top();
}

bool judge_variable(string & str){
int len=str.length();
if(0==len)
  return false;
  if(1==len){
     if(isalpha(str[0]))
        return true;
          return false;
  }
  for(int i=0;i<len;i++){
  if(!isdigit(str[i]))
    return true;
  }
   return false;
   }

bool judge_nub(string & str){
  int len=str.length();
  for(int i=0;i<len;i++){
  if(!isdigit(str[i]))
    return false;
  }
   return true;
   }






