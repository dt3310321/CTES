extern "C" {
#include "clang-c/Index.h"
}

#include "llvm/Support/CommandLine.h"
#include "calculate.h"
#include <iostream>
#include  <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include <set>
using namespace llvm;

//可以用 clang_getCursorLocation来判断是否停止啊 从定义处 到赋值处 一直visit 每次都用location判断一下 等于时 就break
void clear_pair(std::pair<std::string,std::string> &pp);
void clear_info();
void set_flag();
bool Rule_Paser(int size,int index);
bool equalCXlocation(const CXSourceLocation &lhs,const CXSourceLocation &rhs);
bool compareCXlocation(const CXSourceLocation &lhs,const CXSourceLocation &rhs);
void get_info(CXCursor &cursor,CXCursor &parent);
std::string get_var_value(CXCursor &cur_end,const std::string &a);
std::string get_final_point(const std::string &exp);
std::string get_point_expr(const std::string &exp);
enum CXChildVisitResult visitloopifNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitwhileNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitloopNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitifNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitifinfoNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitcopyNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitcopynNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitdereferenceNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitmallocNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
//enum CXChildVisitResult visitBinaryNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitExprNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitLiteralNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitRefNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitSubscriptNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitLoopSubscriptNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitLoopNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitLoopBreakNameNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitLoopBreakNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitPointerNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
enum CXChildVisitResult visitPointerNameNode(CXCursor cursor,CXCursor parent,CXClientData client_data);
static cl::opt<std::string> FileName(cl::Positional,cl::desc("input file"),cl::Required);
std::map<CXSourceLocation,std::string,decltype(compareCXlocation)*> Token(compareCXlocation);
std::map<std::string,std::vector<CXSourceLocation> > var_info;
std::map<std::string,std::string>   pointer_info;
std::set<std::string>   if_info;
std::set<std::string>   loop_if_info;
int if_call_count=0;
std::set<std::string>   follow_if_info;
std::map<std::string,std::string> Arrayinfo;//全局变量用来保存数组信息
std::pair<std::string,std::string> Array_pair;
std::pair<std::string,std::string> loop_break;
std::pair<std::string,std::string> pointer_pair;
std::string func_name;
std::string literal;
std::string  ltr="";  //常量的2种形式
unsigned Array_size=0;
int  Array_index=0;
int  Plus_index=0;
//std::string Pointer_name=" ";
int countx=0;
int cpcount=0;
int sum=0;
int loop_flag=0;
//int count_loop=0;-
int flag_loop=0;
int flag[3]={0,0,0};
int input_flag=0;
extern int calculate_flag;
bool unsolved;

CXCursor point_cur;
CXTranslationUnit translationunit;
std::vector<CXSourceLocation> * pointer_var;
std::string var_name;
std::string message;
CXSourceLocation  cur_begin;
CXSourceLocation  cur_break;

int main(int argc,char **argv){
cl::ParseCommandLineOptions(argc,argv,"source file to diagnose");
CXIndex index=clang_createIndex(1,0);  //这个为1表示忽略头文件 PCH
const char * args[2]={"-I/usr/include/","-I."};
translationunit=clang_parseTranslationUnit(index,FileName.c_str(),args,2,NULL,0,CXTranslationUnit_None);
//先进行词法的token
CXFile file=clang_getFile(translationunit,FileName.c_str());
std::ifstream file_input(FileName.c_str());
file_input.seekg(0,std::ios::end); //调整文件指针到文件结束位置
unsigned file_length=file_input.tellg();//返回指针所在位置
CXSourceLocation loc_start=clang_getLocationForOffset(translationunit,file,0);
CXSourceLocation loc_end=clang_getLocationForOffset(translationunit,file,file_length);  //这个值还是要控制一下哦！！！！ 太大了就糗了
CXSourceRange range=clang_getRange(loc_start,loc_end);
unsigned numTokens=0;
CXToken *tokens=NULL;
clang_tokenize(translationunit,range,&tokens,&numTokens);
        for(unsigned i=0;i<numTokens;++i){
                    enum CXTokenKind kind=clang_getTokenKind(tokens[i]);
                  //  if(kind==CXToken_Literal){
                               std::string name=clang_getCString(clang_getTokenSpelling(translationunit,tokens[i]));
                                CXSourceLocation loc=clang_getTokenLocation(translationunit,tokens[i]);
                                Token.insert({loc,name});
                         //   }
        }
/*
std::cout<<"Token表："<<std::endl;
        for(auto it:Token){
                std::cout<<it.second<<std::endl;
        }
*/
//std::cout<<"-----------------------"<<std::endl;
//AST 分析
CXCursor cur=clang_getTranslationUnitCursor(translationunit);
clang_visitChildren(cur,visitNode,NULL);
//std::cout<<"----------------------"<<std::endl;
//std::cout<<"数组信息："<<std::endl;
/*
      for(auto it:Arrayinfo){
                std::cout<<it.first<<" "<<it.second<<std::endl;
}
std::cout<<"----------------------"<<std::endl;
std::cout<<"指针信息："<<std::endl;
        for(auto it:pointer_info){
                std::cout<<it.first<<" "<<it.second<<std::endl;
}*/

std::cout<<"SUM:"<<sum<<std::endl;
clang_disposeTranslationUnit(translationunit);
clang_disposeIndex(index);
return 0;
}
void set_flag(){
flag[0]=1;
flag[1]=1;
flag[2]=1;
}
bool Rule_Paser(int size,int index){
return (index<size&&index>=0);
}
std::string get_final_point(std::string &exp){
if(exp.find('+')!=std::string::npos){
auto pos=exp.find('+');
std::string str1=exp.substr(0,pos);
std::string str2=exp.substr(pos+1);
if(judge_nub(str1)){
Plus_index=atoi(str1.c_str());
return str2;
}
else{
Plus_index=atoi(str2.c_str());
return str1;
}
}
else if(exp.find('-')!=std::string::npos){
auto pos=exp.find('-');
std::string str1=exp.substr(0,pos);
std::string str2=exp.substr(pos+1);
if(judge_nub(str1)){
Plus_index=-(atoi(str1.c_str()));
return str2;
}
else{
Plus_index=-(atoi(str2.c_str()));
return str1;
}
}
else{
return exp;
}
}
std::string get_point_expr(const std::string &exp){
if(exp[0]=='&')
    return exp;
int i=0;
for( i=0;i<exp.size();i++){
if(!isalpha(exp[i]))
  break;
}
std::string tmp="0"+exp.substr(i);
std::ostringstream ss;
ss<<exp.substr(0,i);
int tt=evaMidExpression(const_cast<char*>(tmp.c_str()));
if(tt>0){
ss<<"+"<<tt;
}
else if(tt<0){
ss<<tt;
}
return ss.str();
}
//我明白了 写在这里  static 每次调用 都会 保留 新的一轮下标查询时     flag 还有 i j 全部都为 0 0 1 真是坑爹啊！
enum CXChildVisitResult visitExprNode(CXCursor cursor,CXCursor parent,CXClientData client_data){
if(flag[0]){
if(clang_getCursorKind(cursor)==CXCursor_FunctionDecl){
auto fname=clang_getCString(clang_getCursorSpelling(cursor));
//std::cout<<fname<<std::endl;
if(fname==func_name){
flag[0]=0;
return CXChildVisit_Recurse;
}
}
return CXChildVisit_Continue;
}
if(flag[1]){
        flag[1]=0;
        return CXChildVisit_Recurse;        //这里是有很大的问题的！！！！！！！！！！！！！！！！！ 有include  就会出错
}
if(flag[2]){
        if(equalCXlocation(cur_begin,clang_getCursorLocation(cursor))){

        flag[2]=0;
}
/*

     std::cout<<clang_getCString(clang_getCursorSpelling(cursor))<<std::endl;
*/
//return CXChildVisit_Recurse;
//return CXChildVisit_Recurse;
return CXChildVisit_Continue;
}
else{
            if(compareCXlocation(cur_break,clang_getCursorLocation(cursor)))
                return CXChildVisit_Break;
            if(clang_getCursorKind(cursor)==CXCursor_BinaryOperator){//还有一个条件 那就是赋值=
                auto Loc=clang_getCursorLocation(cursor);
                unsigned line1,col1;
                clang_getPresumedLocation(Loc,NULL,&line1,&col1);
   // std::cout<<line1<<" "<<col1<<std::endl;
                auto it=Token.find(Loc);
                it++;
            if(Token.at(Loc)==var_name&&it->second=="="){
                    pointer_var->push_back(Loc);
                }
return CXChildVisit_Continue;
}
else if(clang_getCursorKind(cursor)==CXCursor_CallExpr){
    if(clang_getCString(clang_getCursorSpelling(cursor))==std::string("scanf")){
    auto it=Token.find(clang_getCursorLocation(cursor));
    while(it->second!="&")
          it++;
          it++;
          if(it->second==var_name){
          //std::cout<<"呵呵打"<<std::endl;
                pointer_var->push_back(clang_getCursorLocation(cursor));
          }
    return CXChildVisit_Continue;
    }
    return CXChildVisit_Continue;
}
/*
auto xx=clang_getCursorSpelling(cursor);
    std::cout<<clang_getCString(xx)<<std::endl;
     unsigned line1,col1;
     clang_getPresumedLocation(clang_getCursorLocation(cursor),NULL,&line1,&col1);

    std::cout<<i<<std::endl;
    i++;
    */
return CXChildVisit_Recurse;
}
}

//我要维持一个变量 map 让他越来越大- -
std::string get_var_value(CXCursor &cur_end,const std::string &a){
    //expr_flag=1;
   // std::cout<<"1"<<std::endl;
    CXCursor cur_beginx=clang_getCursorReferenced(cur_end);
    auto var_find=var_info.find(a);
    std::vector<CXSourceLocation>::reverse_iterator it;
    if(var_find==var_info.end()){   //如果变量不在map里面
    //std::cout<<"2wa"<<std::endl;
    vector<CXSourceLocation>  var_loc;
    var_loc.push_back(clang_getCursorLocation(cur_beginx));
    pointer_var=&var_loc;
    var_name=a;
    cur_begin=clang_getCursorLocation(cur_beginx);
    cur_break=clang_getCursorLocation(cur_end);
    set_flag();
        clang_visitChildren(clang_getTranslationUnitCursor(translationunit),visitExprNode,NULL); // 这个是只能访问child的 哎 没办法 只能从最顶层开始
    var_info.insert(std::make_pair(a,var_loc));
 //  std::cout<<"SIZE:"<<var_loc.size()<<std::endl;
    it=var_info.find(a)->second.rbegin();   //擦 这里是个局部变量  ---  离开以后销毁！！！！！！！！！！！！！
    }
    else{  //如果变量在map里面
    it=var_find->second.rbegin();
    while(it!=var_find->second.rend()){
    if(compareCXlocation(*it,clang_getCursorLocation(cur_end))&&!equalCXlocation(*it,clang_getCursorLocation(cur_end)))  //就是保证当前的这个写操作的地址比现在的要计算的变量的地址那个要小;
    break;
    it++;
    }
    }
/*
      unsigned line1,col1;
     clang_getPresumedLocation(clang_getCursorLocation(cur_begin),NULL,&line1,&col1);
     std::cout<<line1<<" "<<col1<<std::endl;
*/
  //  std::cout<<"2"<<std::endl;
    //好现在得到了一个要计算的变量的最后的地址了
    std::string  temp;
    auto xx=Token.find(*it);
    if(xx->second=="scanf"){
    unsigned line,col;
    clang_getPresumedLocation(*it,NULL,&line,&col);
    std::ostringstream ss;
    ss<<"External input unreachable!Occur in line:"<<line<<" col: "<<col;
    message=ss.str();
    input_flag=1;
    return std::string(" ");
    }
    xx++;
    xx++;
    while( xx->second!=";"){
       if(judge_variable(xx->second)){ //如果是个变量的话就去找它的值啊
        CXSourceLocation locc=xx->first;
        CXCursor cx=clang_getCursor(translationunit,locc);
        if(0==caculate_flag&&0==input_flag){
        temp+=get_var_value(cx,xx->second);
        }
        else{   //如果检测到上一次计算已经不可达了 就跳出迭代的计算了 得到一个不完整的表达式
        break;
        }
        }
        else
        temp+=xx->second;
        xx++;
        }
        //如果是由于中途跳出 则会变成1 返回一个垃圾
        if(caculate_flag||input_flag){
        std::string tmp=" ";
        return tmp;
        }
        else{
        //std::cout<<temp<<std::endl;
        int t=evaMidExpression(const_cast<char*>(temp.c_str()));
        std::stringstream ss;
        ss<<t;
        std::string tmp=ss.str();
       return tmp;
     }
    }
     // 如果是个常数  就直接 return好了;
    //如果是个变量  递归调用 get_var_value;
    //如果是个表达式 expression 的方法则再调用一遍表达式的方法;
    //不 通通 按照 表达式 来处理 就好了；

void clear_pair(std::pair<std::string,std::string> &pp){
pp={" "," "};
}

void clear_info(){
Plus_index=0;
Array_size=0;
Array_index=0;
}

void get_loc(CXCursor &cursor){
        CXSourceLocation location=clang_getCursorLocation(cursor);
        CXString Fname;
        unsigned line=0,col=0;
        clang_getPresumedLocation(location,&Fname,&line,&col);
        std::cout<<clang_getCString(Fname)<<":"<<line<<":"<<col<<std::endl;
}

bool equalCXlocation(const CXSourceLocation &lhs,const CXSourceLocation &rhs){
unsigned line1,col1,line2,col2;
clang_getPresumedLocation(lhs,NULL,&line1,&col1);
clang_getPresumedLocation(rhs,NULL,&line2,&col2);
return (line1==line2);
}
bool compareCXlocation(const CXSourceLocation &lhs,const CXSourceLocation &rhs){
unsigned line1,col1,line2,col2;
clang_getPresumedLocation(lhs,NULL,&line1,&col1);
clang_getPresumedLocation(rhs,NULL,&line2,&col2);
        if(line1<line2)
return true;
        else if(line2<line1)
return false;
        else{
return col1<col2;
    }
}

void GetLiteral(CXCursor cursor){
CXSourceLocation location=clang_getCursorLocation(cursor);
literal=Token.at(location);
}

enum CXChildVisitResult visitLiteralNode(CXCursor cursor,CXCursor parent,CXClientData client_data){
if(clang_getCursorKind(cursor)==CXCursor_IntegerLiteral){
        GetLiteral(cursor);
        return CXChildVisit_Break;
        }
        return CXChildVisit_Recurse;
}


enum CXChildVisitResult visitRefNode(CXCursor cursor,CXCursor parent,CXClientData client_data){
if(clang_getCursorKind(cursor)==CXCursor_DeclRefExpr){
        CXCursor Rcursor=clang_getCursorReferenced(cursor);
        CXSourceLocation location=clang_getCursorLocation(Rcursor);
        CXString Fname;
        unsigned line=0,col=0;
        clang_getPresumedLocation(location,&Fname,&line,&col);
        literal=" ";//要用literal了
        clang_visitChildren(Rcursor,visitLiteralNode,NULL);
     //   std::cout<<"我找到了我引用的定义"<<clang_getCString(Fname)<<":"<<line<<":"<<col<<std::endl;
//找到定义以后再访问它的常量值 再更新map中的大小
//问题是只能找到一开始的定义而找不到之后的赋值............
//从头找DeclRefExpr 一直找到数组定义前的位置的最新的值就是我们要找的
//数组的定义一般都是直接写好的 -----
//就是下标一般是在变的------------
        return CXChildVisit_Break;
    }
        return CXChildVisit_Recurse;
}



enum CXChildVisitResult visitcopyNode(CXCursor cursor,CXCursor parent,CXClientData client_data){     //strcpy strcat
if(cpcount<=1){
    cpcount++;
    return CXChildVisit_Continue;
    }
       else if(2==cpcount){
      // std::cout<<clang_getCString(clang_getCursorSpelling(cursor))<<std::endl;
       if(clang_getCursorKind(cursor)==CXCursor_DeclRefExpr){
       //  std::cout<<"copy visit"<<std::endl;
        auto name=Token.at(clang_getCursorLocation(cursor));
                 //std::cout<<name<<std::endl;
        if(Arrayinfo.find(name)!=Arrayinfo.end()){
            literal=Arrayinfo.at(name);
            return CXChildVisit_Break;
        }
       if(pointer_info.find(name)!=pointer_info.end()){
             int len=atoi(pointer_info.at(name).c_str());
             len=len-2;
         std::ostringstream ss;
          ss<<len;
                        literal=ss.str();
                   return CXChildVisit_Break;
       }
        literal="-1";  //这是个外面传进来的从参数
                   return CXChildVisit_Break;
       }
          else if(clang_getCursorKind(cursor)==CXCursor_StringLiteral){
          std::string str=Token.at(clang_getCursorLocation(cursor));
          int len=str.size()-2;
          std::ostringstream ss;
          ss<<len;
          literal=ss.str();
                             return CXChildVisit_Break;
          }
            return CXChildVisit_Recurse;
       }
}
enum CXChildVisitResult visitcopynNode(CXCursor cursor,CXCursor parent,CXClientData client_data){   //strncpy   //memcpy
if(cpcount<=2){
    cpcount++;
    return CXChildVisit_Continue;
    }
       else if(2==cpcount){
      // std::cout<<clang_getCString(clang_getCursorSpelling(cursor))<<std::endl;
       if(clang_getCursorKind(cursor)==CXCursor_DeclRefExpr){
       //  std::cout<<"copy visit"<<std::endl;
        auto name=Token.at(clang_getCursorLocation(cursor));
                 //std::cout<<name<<std::endl;
        if(Arrayinfo.find(name)!=Arrayinfo.end()){
            literal=Arrayinfo.at(name);
            return CXChildVisit_Break;
        }
       if(pointer_info.find(name)!=pointer_info.end()){
             int len=atoi(pointer_info.at(name).c_str());
             len=len-2;
         std::ostringstream ss;
          ss<<len;
                        literal=ss.str();
                   return CXChildVisit_Break;
       }
        literal="-1";  //这是个外面传进来的从参数
                   return CXChildVisit_Break;
       }
            return CXChildVisit_Recurse;
       }
}
enum CXChildVisitResult visitloopifNode(CXCursor cursor,CXCursor parent,CXClientData client_data){
if(clang_getCursorKind(cursor)==CXCursor_UnaryOperator){
std::string name=clang_getCString(clang_getCursorSpelling(cursor));
if(name=="*"){
//std::cout<<"addad"<<std::endl;
auto it=Token.find(clang_getCursorLocation(cursor));
it++;
auto pp=it->second;
if(it->second!="("){
it++;
if(it->second=="++"){
it++;
if(it->second=="="){
if(loop_if_info.find(pp)==loop_if_info.end()){
sum++;
std::cout<<"warning+:unsafe pointer"<<std::endl;
}
}
}
}
else{
while(it->second!=")")
  it++;
  it++;
  if(it->second=="++"){
  if(loop_if_info.find(pp)==loop_if_info.end()){
sum++;
std::cout<<"warning+:unsafe pointer"<<std::endl;
}
  }
}
}
return CXChildVisit_Continue;
}
return CXChildVisit_Recurse;
}
enum CXChildVisitResult visitifNode(CXCursor cursor,CXCursor parent,CXClientData client_data){
if(clang_getCursorKind(cursor)==CXCursor_CallExpr){
   if(clang_getCString(clang_getCursorSpelling(cursor))==std::string("strcpy")){
   if_call_count++;
    auto it=Token.find(clang_getCursorLocation(cursor));
            it++;
            it++;
            std::string fname=it->second;
            if(Arrayinfo.find(it->second)!=Arrayinfo.end())
            Array_size=atoi(Arrayinfo.at(it->second).c_str());
            else
            Array_size=-1;
            it++;
            it++;
            std::string sname=it->second;
           clang_visitChildren(cursor,visitcopyNode,NULL);
                        Array_index=atoi(literal.c_str());

                         std::ostringstream ss;
                         if(-1==Array_size)
      ss<<"第一个缓冲区taint"<<",";
      else
  ss<<"第一个缓冲区大小为"<<Array_size<<",";
  if(-1==Array_index)
        ss<<"第二个缓冲区taint"<<",";
        else
        ss<<"第二个缓冲区大小为"<<Array_index<<"  ";
        if(-1==Array_index||-1==Array_size){
         if(Array_index!=1){
         if(if_info.find(fname)!=if_info.end()||if_info.find(sname)!=if_info.end())  //safe
           return CXChildVisit_Continue;
           else{
                     get_loc(cursor);
         //std::cout<<ss.str();
         sum++;
                  std::cout<<"Warning+:buffer taint"<<std::endl;
                  }
                  }
   }
                  else if(Array_index>Array_size){
                  sum++;
                              get_loc(cursor);
                     //      std::cout<<ss.str();
                    std::cout<<"Error:strcpy unsafe"<<std::endl;
                    }
                       return CXChildVisit_Continue;
                }
   else if(clang_getCString(clang_getCursorSpelling(cursor))==std::string("strcat")){
      if_call_count++;
      return CXChildVisit_Continue;

}
}
return  CXChildVisit_Recurse;
}
enum CXChildVisitResult visitwhileNode(CXCursor cursor,CXCursor parent,CXClientData client_data){
if(clang_getCursorKind(cursor)==CXCursor_UnaryOperator){
auto it=Token.find(clang_getCursorLocation(cursor));
std::string name=it->second;
if(name=="*"){
//get_loc(cursor);
it++;
//std::cout<<it->second<<endl;
if(pointer_info.find(it->second)!=pointer_info.end()){
if(Arrayinfo.find(pointer_info.at(it->second))!=Arrayinfo.end()){
it++;
if(it->second=="="){
get_loc(cursor);
sum++;
std::cout<<"warning+:unsafe pointer"<<std::endl;
}
}
}
}
return CXChildVisit_Continue;
}
return CXChildVisit_Recurse;
}
enum CXChildVisitResult visitloopNode(CXCursor cursor,CXCursor parent,CXClientData client_data){
if(clang_getCursorKind(cursor)==CXCursor_UnaryOperator){
auto it=Token.find(clang_getCursorLocation(cursor));
std::string name=it->second;
//std::cout<<name<<std::endl;
if(name=="*"){
get_loc(cursor);
auto it=Token.find(clang_getCursorLocation(cursor));
it++;
auto pp=it->second;
if(it->second!="("){
it++;
if(it->second=="++"){
it++;
if(it->second=="="){
get_loc(cursor);
sum++;
std::cout<<"warning+:unsafe pointer"<<std::endl;
}
}
}
else{
//std::cout<<"() reach!"<<std::endl;
while(it->second!=")")
  it++;
it++;
if(it->second=="++"){
get_loc(cursor);
sum++;
std::cout<<"warning+:unsafe pointer"<<std::endl;
}
}
}
return CXChildVisit_Continue;
}
else if(clang_getCursorKind(cursor)==CXCursor_IfStmt){
loop_if_info.clear();
auto it=Token.find(clang_getCursorLocation(cursor));
auto bf=it;
it++;
auto af=it;
while(af->second!=")"){
if(af->second=="<"){
loop_if_info.insert(bf->second);
}
af++;
bf++;
}
if(loop_if_info.empty()){   //如果strlen约束为空  代表不安全 出去访问就好了
return CXChildVisit_Recurse;
}
else{
clang_visitChildren(cursor,visitloopifNode,NULL);
return CXChildVisit_Continue;
}
}
return CXChildVisit_Recurse;
}
enum CXChildVisitResult visitNode(CXCursor cursor,CXCursor parent,CXClientData client_data){
if (clang_getCursorKind(cursor) == CXCursor_VarDecl){
 //  std::cout<<"var_decl0"<<std::endl;
        CXType t=clang_getCursorType(cursor);
        if(t.kind==CXType_ConstantArray||t.kind==CXType_VariableArray){
                CXString name=clang_getCursorSpelling(cursor);//数组名字
                CXString kind=clang_getTypeSpelling(t);
                std::string sz;//数组大小
                if(t.kind==CXType_ConstantArray){
                                std::ostringstream ssx;
                                ssx<<clang_getArraySize(t);
                                sz=ssx.str();
                              auto ret=Arrayinfo.insert({clang_getCString(name),sz});//写入数组信息到map
                              if(!ret.second){
                              ret.first->second=sz;
                              }
                                //std::cout<<"我不是variable"<<std::endl;
                    }
                        else if(t.kind==CXType_VariableArray){
                /*
                std::string info=clang_getCString(kind);
                unsigned bg=info.find("[");
                std::string size=info.substr(bg+1,info.size()-bg-2);
                */
                //std::cout<<"我是variable"<<std::endl;
                           Array_pair.first=clang_getCString(name);
                           //用一个全局的临时pair
                            clang_visitChildren(cursor,visitRefNode,NULL); //找到了定义哦------ 找到了定义的地方 再找它的literal的值
                            Array_pair.second=literal;
                            auto ret=Arrayinfo.insert(Array_pair);
                              if(!ret.second){
                              ret.first->second=Array_pair.second;
                              }
                            clear_pair(Array_pair);
                            //到visitRefNode里面完成对数组信息的数据的写入；
                        }
                        //begin
//std::cout<<"数组类型："<<clang_getCString(kind)<<"   "<<"数组名："<<clang_getCString(name)<<"数组大小："<<sz<<std::endl;
return  CXChildVisit_Continue;//如果找到了数组定义，忽略它的所有儿子；
        }
           else if(t.kind==CXType_Pointer){
   //std::cout<<"pointer declarion"<<std::endl;
        clear_pair(pointer_pair);
        CXString name=clang_getCursorSpelling(cursor);
        pointer_pair.first=clang_getCString(name);
        auto it=Token.find(clang_getCursorLocation(cursor));
        it++;
        if(it->second=="="){
        string tmp="";
        it++;
        if(it->second=="("){
        it++;
        it++;
        it++;
        it++;
        if(it->second=="malloc"){
        clear_pair(Array_pair);
        Array_pair.first=pointer_pair.first;
        clang_visitChildren(cursor,visitLiteralNode,NULL);
        Array_pair.second=literal;
        Arrayinfo.insert(Array_pair);
        return CXChildVisit_Continue;
        }
        }   //写入动态数组
        auto cur=clang_getCursor(translationunit,it->first);
        if(clang_getCursorKind(cur)==CXCursor_StringLiteral){   //写入字符串
         string xx=it->second;
         pointer_pair.second=it->second;
           pointer_info.insert(pointer_pair);
                   return CXChildVisit_Continue;
        }
    while(it->second!=";"){
    if(pointer_info.find(it->second)!=pointer_info.end()){    //如果是指针之间的赋值的话
    tmp=pointer_info.at(it->second);
    break;
    }
    else{
    if(Arrayinfo.find(it->second)!=Arrayinfo.end()){
    tmp=it->second;
    break;
    }
    }
    it++;
    }
pointer_pair.second=tmp;
        }
                pointer_info.insert(pointer_pair);
        }
        return CXChildVisit_Continue;
}
else if(clang_getCursorKind(cursor)==CXCursor_IfStmt){
//std::cout<<"here"<<std::endl;
if_info.clear();
auto ite=Token.find(clang_getCursorLocation(cursor));
while(ite->second!=")"){
if(ite->second=="strlen"){
ite++;
ite++;
if_info.insert(ite->second);
ite++;
}
ite++;
}
if(if_info.empty()){   //如果strlen约束为空  代表不安全 出去访问就好了
//std::cout<<"empty"<<std::endl;
return CXChildVisit_Recurse;
}
else{
if_call_count=0;
clang_visitChildren(cursor,visitifNode,NULL);
if(if_call_count==0)  {
//说明这个safe condition是为外面的准备的
follow_if_info.clear();
for(auto ixt:if_info){
follow_if_info.insert(ixt);
}
}
return CXChildVisit_Continue;
}
}

else if(clang_getCursorKind(cursor)==CXCursor_ForStmt){
loop_flag=0;
auto it=Token.find(clang_getCursorLocation(cursor));
it++;//(
it++;
if(pointer_info.find(it->second)!=pointer_info.end())
loop_flag=1;
if(loop_flag){
clang_visitChildren(cursor,visitloopNode,NULL);
}
return CXChildVisit_Continue;
}

else if(clang_getCursorKind(cursor)==CXCursor_WhileStmt){
get_loc(cursor);
//std::cout<<"i come to while"<<std::endl;
loop_flag=0;
auto it=Token.find(clang_getCursorLocation(cursor));
it++;//(
it++;
while(it->second!=")"){
if(it->second=="*"){
loop_flag=1;
break;
}
it++;
}
if(loop_flag){
//std::cout<<"i come to flag"<<std::endl;
clang_visitChildren(cursor,visitloopNode,NULL);
}
else{
clang_visitChildren(cursor,visitwhileNode,NULL);
}
return CXChildVisit_Continue;
}

else if(clang_getCursorKind(cursor)==CXCursor_BinaryOperator){ //指针赋值

auto xx=Token.at(clang_getCursorLocation(cursor));
if(pointer_info.find(xx)==pointer_info.end()){
    return CXChildVisit_Recurse;
}
  else{
  //指针赋值！！！
auto it=Token.find(clang_getCursorLocation(cursor));
    it++;
 if(it->second=="="){
        string tmp="";
        it++;
        if(it->second=="("){
        it++;
        it++;
        it++;
        it++;
        if(it->second=="malloc"){
        clear_pair(Array_pair);
        Array_pair.first=pointer_pair.first;
        clang_visitChildren(cursor,visitLiteralNode,NULL);
        Array_pair.second=literal;
        pointer_info.erase(pointer_info.find(xx));
        Arrayinfo.insert(Array_pair);
        return CXChildVisit_Continue;
        }
        }   //写入动态数组
    while(it->second!=";"){
    if(pointer_info.find(it->second)!=pointer_info.end()){    //如果是指针之间的赋值的话
    tmp=pointer_info.at(it->second);
    break;
    }
    else{
    if(Arrayinfo.find(it->second)!=Arrayinfo.end()){
    tmp=it->second;
    break;
    }
    }
    it++;
    }
pointer_info[xx]=tmp;
        }
return CXChildVisit_Continue;
}
  }
      else if(clang_getCursorKind(cursor)==CXCursor_FunctionDecl){
        func_name=clang_getCString(clang_getCursorSpelling(cursor));
     //  std::cout<<func_name<<std::endl;
        return CXChildVisit_Recurse;//下一个
        }

        //end

        else if(clang_getCursorKind(cursor)==CXCursor_CallExpr){
            clear_info();
            cpcount=0;
            if(clang_getCString(clang_getCursorSpelling(cursor))==std::string("strcpy")){
            auto it=Token.find(clang_getCursorLocation(cursor));
            it++;
            it++;
            std::string fname=it->second;
            if(Arrayinfo.find(it->second)!=Arrayinfo.end())
            Array_size=atoi(Arrayinfo.at(it->second).c_str());
            else
            Array_size=-1;
            it++;
            it++;
            std::string sname=it->second;
           clang_visitChildren(cursor,visitcopyNode,NULL);
                        Array_index=atoi(literal.c_str());

                         std::ostringstream ss;
                         if(-1==Array_size)
      ss<<"第一个缓冲区taint"<<",";
      else
  ss<<"第一个缓冲区大小为"<<Array_size<<",";
  if(-1==Array_index)
        ss<<"第二个缓冲区taint"<<",";
        else
        ss<<"第二个缓冲区大小为"<<Array_index<<"  ";
        if(-1==Array_index||-1==Array_size){
         if(Array_index!=1){
         if(follow_if_info.find(fname)!=follow_if_info.end()||follow_if_info.find(sname)!=follow_if_info.end()){
         follow_if_info.clear();
         return CXChildVisit_Continue;
         }
         else{
                     get_loc(cursor);
       //  std::cout<<ss.str();
         sum++;
                  std::cout<<"Warning+:buffer taint"<<std::endl;
                  }
                  }
        }
                  else if(Array_index>Array_size){
                  sum++;
                              get_loc(cursor);
                      //     std::cout<<ss.str();
                    std::cout<<"Error:strcpy unsafe"<<std::endl;
                    }

    return CXChildVisit_Continue;
    }
    else if  (clang_getCString(clang_getCursorSpelling(cursor))==std::string("strcat")){
                auto it=Token.find(clang_getCursorLocation(cursor));
            it++;
            it++;
            std::string fname=it->second;
            it++;
            it++;
        std::string sname=it->second;
              if(follow_if_info.find(fname)!=follow_if_info.end()||follow_if_info.find(sname)!=follow_if_info.end()){
         follow_if_info.clear();
         return CXChildVisit_Continue;
         }
            else{
    sum++;
    get_loc(cursor);
                        std::cout<<"Error:strcat unsafe"<<std::endl;
                            return CXChildVisit_Continue;
                            }
    }
    else if(clang_getCString(clang_getCursorSpelling(cursor))==std::string("strncpy")){
    //safe
                     return CXChildVisit_Continue;
    }
        else if(clang_getCString(clang_getCursorSpelling(cursor))==std::string("strncat")){
        //safe
            return CXChildVisit_Continue;
        }
            else if(clang_getCString(clang_getCursorSpelling(cursor))==std::string("memcpy")){
      sum++;
      get_loc(cursor);
      std::cout<<"unsafe"<<std::endl;
return CXChildVisit_Continue;
            }
        else if(clang_getCString(clang_getCursorSpelling(cursor))==std::string("sprintf")){  //多个参数  第一个是char*  第二个是const char * 后面是可选的；
                     return CXChildVisit_Continue;
            }
    }

return CXChildVisit_Recurse;//下一个
}


enum CXChildVisitResult visitdereferenceNode(CXCursor cursor,CXCursor parent,CXClientData client_data){

if(clang_getCursorKind(cursor)==CXCursor_DeclRefExpr){
        clear_info();
auto name=clang_getCursorSpelling(cursor);
std::string pname=clang_getCString(name);
auto it=pointer_info[pname];
auto ary=get_final_point(it);
if(Arrayinfo.find(ary)!=Arrayinfo.end()){

Array_size=atoi(Arrayinfo[ary].c_str());
Array_index=Plus_index;

        if(!Rule_Paser(Array_size,Array_index)){
                get_loc(cursor);
  std::cout<<"数组大小的值是"<<Array_size<<",";
        std::cout<<"数组下标的值是"<<Array_index<<"  ";
                      std::cout<<"缓冲区溢出"<<std::endl;
                      }
}
return CXChildVisit_Break;
}
else if(clang_getCursorKind(cursor)==CXCursor_ParenExpr){
          clear_info();
           auto loc=clang_getCursorLocation(cursor);
          auto it=Token.find(loc);
          it++;
          std::string tmp="";
          while(it->second!=")"){
         tmp+=it->second;
         it++;
          }
          auto pname=get_final_point(tmp);
          Array_index=Plus_index;
          //std::cout<<Array_index<<std::endl;
         auto ary=get_final_point(pointer_info.at(pname));
if(Arrayinfo.find(ary)!=Arrayinfo.end()){
Array_size=atoi(Arrayinfo[ary].c_str());
        if(!Rule_Paser(Array_size,Array_index+Plus_index)){
                get_loc(cursor);
    std::cout<<"数组大小的值是"<<Array_size<<",";
        std::cout<<"数组下标的值是"<<Array_index+Plus_index<<"  ";
                      std::cout<<"缓冲区溢出"<<std::endl;
                      }

return CXChildVisit_Break;
}
}
return CXChildVisit_Recurse;//下一个
}





