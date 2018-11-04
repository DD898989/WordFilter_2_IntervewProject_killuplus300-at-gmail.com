//#include "stdafx.h"      //for Visual Studio: "Project -> Properties" -> "Configuration Properties -> C/C++ -> Precompiled Headers" , then change the "Precompiled Header" setting to "Not Using Precompiled Headers" option.
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
#include <codecvt>
#include <time.h>
#include <sstream>
#include <regex>
#include <iostream>
#include <queue>
#include <stdint.h>
#include <cwctype>

using namespace std;
vector<string>  m_vCmd;
static const int TREE_NUMBER = 800;

#pragma region 128 & 256 bits definition

#ifndef _STDBOOL_H
#define _STDBOOL_H
#ifndef __cplusplus

#define bool        _Bool
#define true        1
#define false        0

#else /* __cplusplus */

/* Supporting <stdbool.h> in C++ is a GCC extension.  */
#define _Bool        bool
#define bool        bool
#define false        false
#define true        true

#endif /* __cplusplus */

/* Signal that all the definitions are present.  */
#define __bool_true_false_are_defined        1

#endif        /* stdbool.h */
//static const char HEXDIGITS[] = "0123456789abcdef";
typedef struct uint128_t { uint64_t elements[2]; } uint128_t;
typedef struct uint256_t { uint128_t elements[2]; } uint256_t;
#define UPPER_P(x) x->elements[0]
#define LOWER_P(x) x->elements[1]
#define UPPER(x) x.elements[0]
#define LOWER(x) x.elements[1]
inline bool equal128(uint128_t *number1, uint128_t *number2) {
	return (UPPER_P(number1) == UPPER_P(number2)) &&
		(LOWER_P(number1) == LOWER_P(number2));
}
inline bool equal256(uint256_t *number1, uint256_t *number2) {
	return (equal128(&UPPER_P(number1), &UPPER_P(number2)) &&
		equal128(&LOWER_P(number1), &LOWER_P(number2)));
}
inline bool gt128(uint128_t *number1, uint128_t *number2) {
	if (UPPER_P(number1) == UPPER_P(number2)) {
		return (LOWER_P(number1) > LOWER_P(number2));
	}
	return (UPPER_P(number1) > UPPER_P(number2));
}
inline bool gt256(uint256_t *number1, uint256_t *number2) {
	if (equal128(&UPPER_P(number1), &UPPER_P(number2))) {
		return gt128(&LOWER_P(number1), &LOWER_P(number2));
	}
	return gt128(&UPPER_P(number1), &UPPER_P(number2));
}
#pragma endregion
//---------------------------------------------------------------------------------------------------
inline uint64_t  wsto64(wstring wsWord)
{
	uint64_t u64Word = 0;
	for(int i=0;i<wsWord.length();i++)
		u64Word = (u64Word<<20)+wsWord[i];
	return u64Word;
}
//---------------------------------------------------------------------------------------------------
inline uint128_t  wsto128(wstring wsWord)
{
	uint128_t u128Word;
	uint64_t u64Word;

	u64Word = 0;
	for(int i=0;i<3;i++)
		u64Word = (u64Word<<20)+wsWord[i];
	u128Word.elements[0]=u64Word;

	u64Word = 0;
	for(int i=3;i<wsWord.length();i++)
		u64Word = (u64Word<<20)+wsWord[i];
	u128Word.elements[1]=u64Word;

	return u128Word;
}
//---------------------------------------------------------------------------------------------------
inline uint256_t  wsto256(wstring wsWord)
{
	uint128_t u128Word;
	uint256_t u256Word;
	uint64_t u64Word=0;
	int i;

	for(i=0;i<3;i++)
		u64Word = (u64Word<<20)+wsWord[i];
	u128Word.elements[0]=u64Word;

	u64Word=0;
	for(i=3;i<6;i++)
		u64Word = (u64Word<<20)+wsWord[i];
	u128Word.elements[1]=u64Word;

	//---------
	u256Word.elements[0]=u128Word;
	//---------

	u128Word.elements[1]=0;
	u64Word=0;
	for(i=6;i<wsWord.length();i++)
	{
		u64Word = (u64Word<<20)+wsWord[i];
		if(i==8)
		{
			u128Word.elements[0]=u64Word;
			u64Word=0;
		}
	}
	if(i>8)
		u128Word.elements[1]=u64Word;

	//------------
	u256Word.elements[1]=u128Word;
	//---------

	return u256Word;
}
//---------------------------------------------------------------------------------------------------
class TreeNode64{
private:
	TreeNode64 *leftchild;
	TreeNode64 *rightchild;
	TreeNode64 *parent;
	uint64_t key;
public:
	TreeNode64():leftchild(0),rightchild(0),parent(0),key(0){};
	TreeNode64(uint64_t a):leftchild(0),rightchild(0),parent(0),key(a){};

	friend class BST64;   
};
//---------------------------------------------------------------------------------------------------
class BST64{
private:
	TreeNode64 *root;
	TreeNode64* Leftmost(TreeNode64 *current){

		while (current->leftchild != NULL){
			current = current->leftchild;
		}
		return current;
	}
	TreeNode64* Successor(TreeNode64 *current){

		if (current->rightchild != NULL){
			return Leftmost(current->rightchild);
		}

		TreeNode64 *new_node = current->parent;

		while (new_node != NULL && current == new_node->rightchild) {
			current = new_node;
			new_node = new_node->parent;
		}

		return new_node;
	}

public:
	 BST64():root(0){};
	 inline TreeNode64* Search(uint64_t KEY)
	{
		TreeNode64 *current = root;               

		while (current != NULL && KEY != current->key) {  

			if (KEY < current->key){                      
				current = current->leftchild;   
			}
			else {
				current = current->rightchild;  
			}
		}
		return current;
	}
	 void InsertBST(uint64_t key, wstring element)
	{
		TreeNode64 *y = 0;        
		TreeNode64 *x = 0;        
		TreeNode64 *insert_node = new TreeNode64(key); 

		x = root;
		while (x != NULL) {                
			y = x;                                
			if (insert_node->key < x->key){
				x = x->leftchild;
			}
			else{
				x = x->rightchild;
			}
		}                                  

		insert_node->parent = y;           

		if (y == NULL){                    
			this->root = insert_node;
		}
		else if (insert_node->key < y->key){
			y->leftchild = insert_node;
		}
		else{
			y->rightchild = insert_node;
		}
	}
	 void DeleteBST(uint64_t KEY)
	 {     
		TreeNode64 *delete_node = Search(KEY);
		if (delete_node == NULL) {
			cout << "data not found.\n";
			return;
		}

		TreeNode64 *y = 0;   
		TreeNode64 *x = 0;  

		if (delete_node->leftchild == NULL || delete_node->rightchild == NULL){
			y = delete_node;
		}
		else{
			y = Successor(delete_node);                     
		}      
		if (y->leftchild != NULL){              
			x = y->leftchild;  
		}
		else{
			x = y->rightchild;
		}

		if (x != NULL){ 
			x->parent = y->parent; 
		}
		if (y->parent == NULL){   
			this->root = x;
		}
		else if (y == y->parent->leftchild){
			y->parent->leftchild = x; 
		}
		else{ 
			y->parent->rightchild = x;
		}                   
		if (y != delete_node) {  
			delete_node->key = y->key;
		}
		delete y; 
		y = 0;
	}
};
//---------------------------------------------------------------------------------------------------
class TreeNode128{
private:
	TreeNode128 *leftchild;
	TreeNode128 *rightchild;
	TreeNode128 *parent;
	uint128_t key;
public:
	TreeNode128(): leftchild(0), rightchild(0), parent(0){};
	  TreeNode128(uint128_t a):leftchild(0),rightchild(0),parent(0),key(a){};
	  friend class BST128;   
};
//---------------------------------------------------------------------------------------------------
class BST128{
private:
	TreeNode128 *root;
	TreeNode128* Leftmost(TreeNode128 *current){

		while (current->leftchild != NULL){
			current = current->leftchild;
		}
		return current;
	}
	TreeNode128* Successor(TreeNode128 *current){

		if (current->rightchild != NULL){
			return Leftmost(current->rightchild);
		}

		TreeNode128 *new_node = current->parent;

		while (new_node != NULL && current == new_node->rightchild) {
			current = new_node;
			new_node = new_node->parent;
		}

		return new_node;
	}
public:
	BST128():root(0){};
	inline TreeNode128* Search(uint128_t KEY)
	{
		TreeNode128 *current = root;               

		while (current != NULL && !equal128(&KEY , &current->key)) {  

			if (     !gt128(&KEY , &current->key)    ){                      
				current = current->leftchild;   
			}
			else {
				current = current->rightchild;  
			}
		}
		return current;
	}
	void InsertBST(uint128_t key, wstring element)
	{
		TreeNode128 *y = 0;        
		TreeNode128 *x = 0;        
		TreeNode128 *insert_node = new TreeNode128(key); 

		x = root;
		while (x != NULL) {                
			y = x;                                
			if (     !gt128(&insert_node->key , &x->key)){
				x = x->leftchild;
			}
			else{
				x = x->rightchild;
			}
		}                                  

		insert_node->parent = y;           

		if (y == NULL){                    
			this->root = insert_node;
		}
		else if (      !gt128(&insert_node->key , &y->key)            ){
			y->leftchild = insert_node;
		}
		else{
			y->rightchild = insert_node;
		}
	}
	void DeleteBST(uint128_t KEY){  

		TreeNode128 *delete_node = Search(KEY);
		if (delete_node == NULL) {
			cout << "data not found.\n";
			return;
		}

		TreeNode128 *y = 0;
		TreeNode128 *x = 0; 

		if (delete_node->leftchild == NULL || delete_node->rightchild == NULL){
			y = delete_node;
		}
		else{
			y = Successor(delete_node);                 
		} 
		if (y->leftchild != NULL){              
			x = y->leftchild;  
		}    
		else{
			x = y->rightchild;
		}

		if (x != NULL){ 
			x->parent = y->parent; 
		}
		if (y->parent == NULL){    
			this->root = x;
		}
		else if (y == y->parent->leftchild){
			y->parent->leftchild = x; 
		}
		else{ 
			y->parent->rightchild = x; 
		}                         
		if (y != delete_node) {      
			delete_node->key = y->key;
		}

		delete y; 
		y = 0;
	}
};
//---------------------------------------------------------------------------------------------------
class TreeNode256{
private:
	TreeNode256 *leftchild;
	TreeNode256 *rightchild;
	TreeNode256 *parent;
	uint256_t key;
public:
	TreeNode256(): leftchild(0), rightchild(0), parent(0){};
	  TreeNode256(uint256_t a):leftchild(0),rightchild(0),parent(0),key(a){};
	  friend class BST256;   
};
//---------------------------------------------------------------------------------------------------
class BST256{
private:
	TreeNode256 *root;
	TreeNode256* Leftmost(TreeNode256 *current){

		while (current->leftchild != NULL){
			current = current->leftchild;
		}
		return current;
	}
	TreeNode256* Successor(TreeNode256 *current){

		if (current->rightchild != NULL){
			return Leftmost(current->rightchild);
		}

		TreeNode256 *new_node = current->parent;

		while (new_node != NULL && current == new_node->rightchild) {
			current = new_node;
			new_node = new_node->parent;
		}

		return new_node;
	}
public:
	BST256():root(0){};
	inline TreeNode256* Search(uint256_t KEY)
	{
		TreeNode256 *current = root;               

		while (current != NULL && !equal256(&KEY , &current->key)) {  

			if (     !gt256(&KEY , &current->key)    ){                      
				current = current->leftchild;   
			}
			else {
				current = current->rightchild;  
			}
		}
		return current;
	}
	void InsertBST(uint256_t key, wstring element)
	{
		TreeNode256 *y = 0;        
		TreeNode256 *x = 0;        
		TreeNode256 *insert_node = new TreeNode256(key); 

		x = root;
		while (x != NULL) {                
			y = x;                                
			if (     !gt256(&insert_node->key , &x->key)){
				x = x->leftchild;
			}
			else{
				x = x->rightchild;
			}
		}                                  

		insert_node->parent = y;           

		if (y == NULL){                    
			this->root = insert_node;
		}
		else if (      !gt256(&insert_node->key , &y->key)            ){
			y->leftchild = insert_node;
		}
		else{
			y->rightchild = insert_node;
		}
	}
	void DeleteBST(uint256_t KEY){  

		TreeNode256 *delete_node = Search(KEY);
		if (delete_node == NULL) {
			cout << "data not found.\n";
			return;
		}

		TreeNode256 *y = 0;
		TreeNode256 *x = 0; 

		if (delete_node->leftchild == NULL || delete_node->rightchild == NULL){
			y = delete_node;
		}
		else{
			y = Successor(delete_node);                 
		} 
		if (y->leftchild != NULL){              
			x = y->leftchild;  
		}    
		else{
			x = y->rightchild;
		}

		if (x != NULL){ 
			x->parent = y->parent; 
		}
		if (y->parent == NULL){    
			this->root = x;
		}
		else if (y == y->parent->leftchild){
			y->parent->leftchild = x; 
		}
		else{ 
			y->parent->rightchild = x; 
		}                         
		if (y != delete_node) {      
			delete_node->key = y->key;
		}

		delete y; 
		y = 0;
	}
};
//---------------------------------------------------------------------------------------------------
BST64 m_T64[3][TREE_NUMBER];   //3: 1~3個字的髒話
BST128 m_T128[3][TREE_NUMBER]; //3: 4~6個字的髒話
BST256 m_T256[6][TREE_NUMBER]; //6: 6~12個字的髒話
//---------------------------------------------------------------------------------------------------
void AddCmd()
{
	m_vCmd.push_back("開始過濾");	m_vCmd.push_back("取代字詞");	m_vCmd.push_back("搜尋目標");	m_vCmd.push_back("移除目標");	m_vCmd.push_back("新增目標");	m_vCmd.push_back("離開程式");
}
//---------------------------------------------------------------------------------------------------
enum ECommand
{
	E_RUN,							E_REPLACE_WORD,					E_SEARCH,						E_REMOVE,						E_ADD,							E_EXIT,
};
//---------------------------------------------------------------------------------------------------
string wstring_to_utf8 (const wstring& str)
{
	wstring_convert<codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}
//---------------------------------------------------------------------------------------------------
wstring Read(string filename)
{
	wifstream wif(filename);
	wif.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
	wstringstream wss;
	wss << wif.rdbuf();
	return wss.str();
}

inline int GetKeyFromWS(wstring ws)
{
	int len=ws.length();
	int KeySum=0;
	for(int i=0;i<len;i++)
		KeySum+=ws[i]+i;
	KeySum = KeySum%TREE_NUMBER;
	return KeySum;
}
//---------------------------------------------------------------------------------------------------
inline bool IsBadWord(wstring ws)
{
	int len=ws.length();

	int KeySum=GetKeyFromWS(ws);

	if(len<4)
	{
		if(m_T64[len-1][KeySum].Search(wsto64(ws)) == NULL )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else if(len<7)
	{
		if(m_T128[len-4][KeySum].Search(wsto128(ws)) == NULL )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		if(m_T256[len-7][KeySum].Search(wsto256(ws)) == NULL )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}
//---------------------------------------------------------------------------------------------------
void  RemoveBadWord(wstring ws)
{
	int len=ws.length();
	int KeySum=GetKeyFromWS(ws);
	if(len<4)
	{
		uint64_t u64;
		u64 = wsto64(ws);
		m_T64[len-1][KeySum].DeleteBST(u64);
	}
	else if(len<7)
	{
		uint128_t u128;
		u128 = wsto128(ws);
		m_T128[len-4][KeySum].DeleteBST(u128);
	}
	else
	{
		uint256_t u256;
		u256 = wsto256(ws);
		m_T256[len-7][KeySum].DeleteBST(u256);
	}
}
//---------------------------------------------------------------------------------------------------
void  AddBadWord(wstring ws)
{

	int len=ws.length();
	
	int KeySum=GetKeyFromWS(ws);

	if(len<4)
	{
		uint64_t u64;
		u64 = wsto64(ws);
		m_T64[len-1][KeySum].InsertBST(u64,ws);
	}
	else if(len<7)
	{
		uint128_t u128;
		u128 = wsto128(ws);
		m_T128[len-4][KeySum].InsertBST(u128,ws);
	}
	else
	{
		uint256_t u256;
		u256 = wsto256(ws);
		m_T256[len-7][KeySum].InsertBST(u256,ws);
	}
}
//---------------------------------------------------------------------------------------------------
void ReadBadWordDic(string sDicFile)
{
	const locale empty_locale = locale::empty();
	typedef codecvt_utf8<wchar_t> converter_type;
	const converter_type* converter = new converter_type;
	const locale utf8_locale = locale(empty_locale, converter);
	wifstream stream((sDicFile));
	stream.imbue(utf8_locale);
	wstring line;

	ifstream inFile(sDicFile); 
	int countline = count(istreambuf_iterator<char>(inFile), 
		istreambuf_iterator<char>(), '\n');
	clock_t t1 = clock();		/*///////////////////////////////////////////*/
	static int i;
	i=0;
	while (getline(stream, line))
	{
		i++;

		if((countline-i)%5000==0)
			cout<<"Reading bad word dictionary, remaining:"<<countline-i<<endl;

		if(line.length()>0 && line.length()<13)
		{
			for(int i=0;i<line.length();i++)
				line[i]= towupper(line[i]);
			AddBadWord(line);

			for(int i=0;i<line.length();i++)
				line[i]= towlower(line[i]);
			AddBadWord(line);
		
			
			line[0]= towupper(line[0]);
			AddBadWord(line);
		}
	}
	clock_t t2 = clock();   	/*///////////////////////////////////////////*/  printf("%s:%f\n", "Reading Time: ",(t2-t1)/(double)(CLOCKS_PER_SEC));
}
//---------------------------------------------------------------------------------------------------
string cinRegMatch(string sMatch)
{
	while(true)
	{
		string buffer;
		getline(cin, buffer);
		regex rx(sMatch);
		smatch m;
		string str = buffer;
		if(regex_search(str, m, rx))
		{
			return buffer;
		}
	}
}
//---------------------------------------------------------------------------------------------------
wstring wcinRegMatch(wstring sMatch)
{
	locale::global(locale(""));
	while(true)
	{
		wstring buffer;
		getline(wcin, buffer);
		wregex  rx(sMatch);
		wsmatch m;
		wstring str = buffer;
		if(regex_search(str, m, rx))
		{
			return buffer;
		}
	}
}
//---------------------------------------------------------------------------------------------------
int cinRegMatchWholeNumber(string sMatch)
{
	while(true)
	{
		string buffer;
		getline(cin, buffer);
		regex rx(sMatch);
		smatch m;
		string str = buffer;
		if(regex_search(str, m, rx))
		{
			int nNum;
			istringstream is(buffer);
			is>>nNum;
			return nNum;
		}
	}
}
//---------------------------------------------------------------------------------------------------
string ExePath() 
{
	char buffer[MAX_PATH];
	GetModuleFileNameA( NULL, buffer, MAX_PATH );
	string::size_type pos = string( buffer ).find_last_of( "\\/" );
	return string( buffer ).substr( 0, pos);
}
//---------------------------------------------------------------------------------------------------
int main()
{
	AddCmd();

	//global para
	wstring wsOutput;
	bool bRun = false;
	int cmd;
	ofstream myfile;

	//default value
	string sRootPath = ExePath()+"\\";
	string sOutputFile =sRootPath+"Output.txt";
	string sInputFile =sRootPath+"Input.txt";
	string sDicFile =sRootPath+"BadWords.txt";
	wstring wsReplace = L"【X】";

	//case para
	wstring wsRemove;
	wstring wsAdd;
	wstring wsSearch;

	locale::global(locale(""));
	cout<<"取代字詞:"; wcout<<wsReplace<<endl;
	cout<<"輸出路徑:"<<sOutputFile<<endl;
	//cout<<"輸入路徑:"<<sInputFile<<endl;
	cout<<"字典路徑:"<<sDicFile<<endl;

	cout<<"確認 字典路徑 檔案後按下enter繼續\n";
	system("pause");

	ReadBadWordDic(sDicFile);

	while(true)
	{
		cout<<endl<<endl;
		cout<<"請輸入CMD："<<endl;
		for(int i=0;i<m_vCmd.size();i++)
			cout<<i<<"."<<m_vCmd[i]<<" ";
		cout<<endl;
		cmd = cinRegMatchWholeNumber("^([0-9]*)$");
		cout<<endl<<endl;

		switch(cmd)//cmd
		{
			//---------------------------------------------
		case (int)E_RUN:
			//cout<<"開始過濾"<<endl;
			bRun=true;
			goto run;
			//---------------------------------------------
		case (int)E_REPLACE_WORD:
			cout<<"請輸入 "<<m_vCmd[cmd]<<" ："<<endl;
			wsReplace = wcinRegMatch(L"^.+$");
			goto es;
			//---------------------------------------------
		case (int)E_SEARCH:
			cout<<"請輸入 "<<m_vCmd[cmd]<<" ："<<endl;
			wsSearch = wcinRegMatch(L"^.{1,12}$");

			if(IsBadWord(wsSearch))
			{
				cout<<"Found"<<endl;
				goto es;
			}
			cout<<"Not found"<<endl;
			goto es;
			//---------------------------------------------
		case (int)E_REMOVE:
			cout<<"請輸入 "<<m_vCmd[cmd]<<" ："<<endl;
			wsRemove = wcinRegMatch(L"^.{1,12}$");
			RemoveBadWord(wsRemove);
			RemoveBadWord(wsRemove);
			RemoveBadWord(wsRemove);
			goto es;
			//---------------------------------------------
		case (int)E_ADD:
			cout<<"請輸入 "<<m_vCmd[cmd]<<" ："<<endl;
			wsAdd = wcinRegMatch(L"^.{1,12}$");
			AddBadWord(wsAdd);
			goto es;
			//---------------------------------------------
		case (int)E_EXIT:
			return 0;
			//---------------------------------------------
		default:
			goto es;
			//---------------------------------------------
		}
		//---------------------------------------------
es://escape_route:
run://run:
		if(bRun)
		{
			bRun = false;
			wstring wsInput;// = Read(sInputFile);
			cout<<endl<<"請輸入過濾句子:"<<endl;
			getline(wcin, wsInput);


			/*計時開始*/  static LARGE_INTEGER nFreq; static LARGE_INTEGER nBeginTime; static LARGE_INTEGER nEndTime; static int count=0;  static double timeT=0;  QueryPerformanceFrequency(&nFreq); QueryPerformanceCounter(&nBeginTime); //-----------------------------------------------------------------------------------

			int* ptr_nFilterRecord        = new int[wsInput.size()]();
			int* ptr_nFilterRecordForMark = new int[wsInput.size()]();
			for(int i =0;i<wsInput.length();i++)
			{
				int temp = wsInput[i];
				//各類符號跟標記
				if( temp==9  ||  temp==10  || temp==13 ||  (temp>=32 && temp<=47)|| (temp>=58 && temp<=64) || (temp>=91 && temp<=96) || (temp>=123 && temp<=126) ||
					temp==8739  || temp==65306 || temp==65307 || temp==8212  || temp==8231  || temp==65288 || temp==65289 || 
					temp==65292 || temp==65311 || temp==12290 || temp==65281 || temp==12301 || temp==12300 || temp==8217
					)
				{
					ptr_nFilterRecord[i] = 1;
					ptr_nFilterRecordForMark[i] = 1;
				}
			}

			int nSortCount = 0;
			for(int nWordLen=12;nWordLen>0;nWordLen--)
			{
				int diff = wsInput.length()-nWordLen;//need assign to diff to check
				if(diff<0)
					continue;
				
				for(int i=0;i<wsInput.length()-nWordLen+1;i++)
				{
					if(ptr_nFilterRecord[i]>=nWordLen)
					{
						i+=nWordLen;
						if(i>wsInput.length()-1-nWordLen-1)
							continue;
					}
					
					wstring wsTemp;
					for(int j=0;j<nWordLen;j++)
						wsTemp += wsInput[i+j];
					
					if(IsBadWord(wsTemp))
					{
						for(int j=0;j<nWordLen;j++)
							ptr_nFilterRecord[i+j]=1;
						i += (nWordLen-1);

						if(nSortCount++%10==0)
						{
							int pos = -1; 
							for(int k=0;k<wsInput.length();k++)
							{
								if(ptr_nFilterRecord[k]>0 &&  pos==-1)
									pos = k;

								if(ptr_nFilterRecord[k]==0 && pos>-1)
								{
									ptr_nFilterRecord[pos]=k-pos;
									pos = -1;
								}
							}
						}
					}
				}
			}


			//recover mark
			for(int i =0;i<wsInput.length();i++)
				if(ptr_nFilterRecordForMark[i] == 1)
					ptr_nFilterRecord[i] = 0;

			for(int i =0;i<wsInput.length();i++)
			{
				if(
					i>0&&i<wsInput.length()-1  &&  ptr_nFilterRecordForMark[i] == 1  
					&&  
					( (ptr_nFilterRecord[i-1] >0 )  && (ptr_nFilterRecord[i+1] > 0) )    
					)
				{
					ptr_nFilterRecord[i] = 1;
				}
			}

			
			//Special run length encode
			vector<int> vRunLengthEncode;
			int pos = -1;
			int len = wsInput.size();
			for (int j = 0; j < len; ++j)
			{
				if(ptr_nFilterRecord[j]>0 &&  pos==-1)
				{
					pos = j;
				}
				
				if((ptr_nFilterRecord[j]==0) && pos>-1)
				{
					vRunLengthEncode.push_back(pos);
					vRunLengthEncode.push_back(j-pos);
					pos=-1;
				}

				if(j==len-1)
				{
					if(ptr_nFilterRecord[j]>0)
					{
						vRunLengthEncode.push_back(pos);
						vRunLengthEncode.push_back(j-pos+1);
					}
				}
			}
			
			
			//replace
			wsOutput.assign( wsInput );
			for(int i=vRunLengthEncode.size()-2;i>-1;i-=2)
				wsOutput = wsOutput.replace(vRunLengthEncode[i], vRunLengthEncode[i+1], wsReplace);

			delete ptr_nFilterRecord;
			delete ptr_nFilterRecordForMark;
			/*計時結束*/  QueryPerformanceCounter(&nEndTime);    timeT+=(double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart;count++;if(count%1==0){ printf("\nReplacing time:%f\n",timeT/count); timeT=0;count=0;}

			
			wcout<<endl<<L"過濾後:"<<endl<<wsOutput<<endl;;

			myfile.open (sOutputFile);
			myfile << wstring_to_utf8(wsOutput);
			myfile.close();
			//---------------------------------------------
		}
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------
