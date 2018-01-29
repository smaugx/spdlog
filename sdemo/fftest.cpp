#include <fstream>
#include<string>
#include<map>
#include <iostream>
using namespace std;

int main ()
{
  map<string,string> cpo;
  cpo.insert(make_pair("中国","86"));
  cpo.insert(make_pair("浙江","001"));
  cpo.insert(make_pair("上海","002"));
  cpo.insert(make_pair("天津","003"));
  cpo.insert(make_pair("山东","004"));

  ofstream outfile("./outfile.txt",std::ofstream::out);

  for(map<string,string>::iterator i = cpo.begin(); i != cpo.end(); i++)
  {
    outfile<<i->first;
    outfile<<"\n";
    outfile<<i->second;
    outfile<<"\n";
  }
  outfile.close();


  ifstream infile("./outfile.txt");
  map<string,string> *pcpo = new map<string,string>();
  char chinese[128];
  char code[64];
  while( !infile.eof())
  {
    infile.getline(chinese,sizeof(chinese));
    infile.getline(code,sizeof(code));
    pcpo->insert(make_pair(chinese,code));
  }


  for(map<string,string>::iterator i = pcpo->begin(); i != pcpo->end(); i++)
  {
    cout<<i->first<<"  "<<i->second<<endl;
  }
    return 0;
  }
