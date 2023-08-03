#include<stdio.h>

int main()
{
  char file[16];
  char func[16];
  int line;
  sprintf(file,__FILE__); //文件名
  sprintf(func,__FUNCTION__);//函数名
  printf("file=%s\n",file);
  printf("func=%s\n",func);
  printf("%05d\n",__LINE__);//行号
  return 0;
}