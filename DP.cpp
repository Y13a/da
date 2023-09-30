#include<fstream>
#include<iostream>
#include<cstring>
#include<sstream>
#include<stdlib.h>
#include<cmath>
#include<algorithm>
#include<bitset>
#include<fstream>
using namespace std;
string base(int n){
	string s;
	while(n!=0){
		if(n%2==0){
			s=s+"0";
		}
		else{
			s=s+"1";
		}
		n=n/2;
	}
	reverse(s.begin(),s.end());
	return s;
}//自己做的一个把正整数表示成二进制01串的函数，为了便于理解用string返回结果，比如8 ，返回的是“1000”
string solidbase(int n,int m){
	string l1=base(n);
	int f=l1.length();
	reverse(l1.begin(),l1.end());
	while(l1.length()!=m){
		l1=l1+"0";
	}
	reverse(l1.begin(),l1.end());
	return l1;
}//这个是规定好用几位表示的函数，比如solidbase(8,7),
//8用7位二进制表示，就返回“0001000”，这是为了后面变位存储准备的
int ToInt(string s){
	
	int n;
	stringstream stream;
	stream<<s;
	stream>>n;
	return n;
}//查资料写的一个把数字字符串直接转成整数值的函数
//比如“168”，直接返回168
//自带的我没有找到，自己写

//动态规划 图像压缩问题
int p[262145];//第一位p[0]用0填充，让程序更好理解
string result="";//最终想得到的东西
const int N=262145;
int s[N],l[N],b[N];
int length(int i);
void Compress(int n,int p[],int s[],int l[],int b[]);
void Output(int s[],int l[],int b[],int n,string result);//非教材的输出 
int o=1;
bitset<2060400>*bit=NULL;//这里的2060400是之前跑程序得到压缩后的结果，单位是bit,本来是不知道的

int main()

{
	
	
	ifstream ifs;
	ifs.open("pixel.txt",ios::in);
	if(!ifs.is_open()){
		cout<<"文件打开失败"<<endl;

	}
	
	p[0]=0;
	int i=1;

	string temp;
	while(getline(ifs,temp)){
		
		p[i]=ToInt(temp);//用到了转整数的函数，因为getline得到的是字符串
		
		i++;
		
	}//用c++读取raw文件有点麻烦而且慢，这里分成两步，读取raw格式文件在pycharm上面写了
	//都在压缩包里面，pycharm程序把二进制文件读取的灰度值转成整数形式存在了out.txt里面。
	//然后C++运行主要的算法程序，用一个字符串把所有pixel读进来，再转成整数



	/*
		s[]
		l[]
		b[]
	*/
   
 

	
	
	
 
	Compress(N-1,p,s,l,b); 

	Output(s,l,b,N-1,result);
	system("pause");
	
	return 0;

}



void Compress(int n,int p[],int s[],int l[],int b[])
{//s[i]：1<=i<=n,是像素序列{p1,...,pi}的最优分段所需存储位数。
// l[i]：1<=i<=n,是像素序列{p1,...,pi}的最优分段的最后一段的长度。
//b[i]:一段每个像素的表示 
//递归公式:s[i]=min(1<=k<=min(i,256)) (s[i-k]+k*bmax(i-k+1,i)+11.(k：表示像素序列{p1,...,pi}的最优分段的最后一段的长度) 
	int Lmax = 256,header = 11,bmax;
	s[0] = 0; //初值
	for(int i=1; i<=n; i++)//按照s[1] -> s[2] ->....s[n]的顺序求解，s[n]即最终的最优解。
	{
		b[i] = length(p[i]);//计算像素点p需要的存储位数
		bmax = b[i];
		
		s[i] = s[i-1] + bmax;//当k=1的情况下的值
		l[i] = 1;
 
		for(int k=2; k<=i && k<=Lmax;k++) //求k取2--->min(i,256)时是否有更小的值,有更小的则更新s[i]。 
		{
			if (bmax < length(p[i-k+1])) {
			bmax=length(p[i-k+1]);
		}
			
			if(s[i]>s[i-k]+k*bmax)
			{
				s[i] = s[i-k] + k*bmax;
				l[i] = k;
				//!!!!!!注：这里没有按照课件写的那样做，因为b[i]=bmax这一步我写在在output函数里面了
				
			}
		}
	  	s[i] += header;
     	
	
	}
}
 
int length(int i)
{//返回灰度值i需要的存储位数 
	
	int count=0;
	count=floor(log2(i))+1;
	return count;
}
 int Maxb(int b[],int i,int j )//找该段最大的位数 
{
	int bmax=0;
	bmax=b[i];
	for(int k=i+1;k<=j;k++)
	{
		if(bmax<b[k])
		   bmax=b[k];
	}	
	return bmax;	
}

void Output(int s[],int l[],int b[],int n,string result)
{
	cout<<"图像压缩后的最小空间为："<<s[n]<<endl;
	int m = 0,t;
	t=n;//序列长度 
    while(t)
    {
    	m++;
    	t=t-l[t];
    }//m：记录共分为几段 
	cout<<"将原灰度序列分成"<<m <<"段序列段"<<endl;
	
	int *length=new int[m+1];//存储每段的长度值 
	int *bmax=new int[m+1];//存储每段的灰度值占用的位数
 
    t=n;
	for(int j=m; j>=1; j--)
	{
		length[j] = l[t];		
		bmax[j]= Maxb(b,(t-l[t]+1),t);
		t=t-l[t];
	}
	
	for(int j=1; j<=m; j++)
	{
		cout<<"段长度："<<length[j]<<",所需存储位数:"<<bmax[j]<<endl;
		int g=length[j];
		int f=bmax[j];
		result=result+solidbase(g-1,8)+solidbase(f-1,3);//以为有的段数含有的pixel值达到256，规定一下把段长度-1存进8位里面
		//后面解码的时候再加回来就可以
		//需要的存储位数也是，8位表示成111就行。
		
		
		for( int i=o;i<o+length[j];i++){
			
			result=result+solidbase(p[i],bmax[j]);//把在这个段里面的pixel值，按照算出来的位数bmax[j]，转成011010这种字符串
			//加到result上面。
			//貌似有更高级的方法，但是我习惯这种，字符串很方便拼接
		
		}
		o+=g;
		
		cout<<endl;
}
//为了方便直接把code的过程写在这个output函数里面了，当做子函数
//code:
bit=new bitset<2060400>(result);//把所有0101表示的字符串直接存到bitset里面，减少很多空间，string里面是2060400个位应该有的0或1的值
//实际上string 占得空间达到了2060400*8位
//但是只是因为不熟悉写法，为了理解用string先记下来，为了真正压缩空间，用bitset结构把string表达的一个二进制流写进去
ofstream out("trouble.txt",ios::binary);
out.write((char *)bit,257550);//写进文件，要注意字符的顺序在文件里是倒着的，因为用字符串构造bitset的时候，是从右向左加入bitset的
//这点很烦，后面需要另外处理。
out.close();

 
}

	

		
		
		
		


