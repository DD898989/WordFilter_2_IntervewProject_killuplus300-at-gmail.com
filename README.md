# WordFilter_2_IntervewProject_killuplus300-at-gmail.com


Wstring To Number
--
你 = 20320 = 0X4F60  
好 = 22909 = 0X597D  
嗎 = 21966 = 0X55CE  
  
#### wstring length less than 4 could be stored by a 64 bits type  
  
ex:  
wstring  ws = L"你好嗎";  
uint64_t n = 0;  
for(int i = 0;i<3;i++)  
　　n += (n<<20)+ws[i]; //n = 0X4F600597D055CE  
  
#### Therefore:  
wstring length less than 7  could be stored by a 128 bits type  
wstring length less than 13 could be stored by a 256 bits type  
  
Binary Tree
--
#### So we can store wstring into a binary:  
![](https://github.com/DD898989/Pictures/blob/master/tree.PNG)   
  
#### For less space operation, we might make three classes for these wstring:  
`BST64   m_T64;`   this tree for wstring length of 1-3  
`BST128 m_T128;`   this tree for wstring length of 4-6  
`BST256 m_T256;`   this tree for wstring length of 7-12  

#### Generally, more trees mean less time in search, remove, and add:
`BST64    m_T64[0];`    //this tree for wstring length of 1<br/>
`BST64    m_T64[1];`    //this tree for wstring length of 2<br/>
`BST64    m_T64[2];`    //this tree for wstring length of 3<br/>
`BST128   m_T128[0];`   //this tree for wstring length of 4<br/>
`BST128   m_T128[1];`   //this tree for wstring length of 5<br/>
`BST128   m_T128[2];`   //this tree for wstring length of 6<br/>
`BST256   m_T256[0];`   //this tree for wstring length of 7<br/>
`BST256   m_T256[1];`   //this tree for wstring length of 8<br/>
`BST256   m_T256[2];`   //this tree for wstring length of 9<br/>
`BST256   m_T256[3];`   //this tree for wstring length of 10<br/>
`BST256   m_T256[4];`   //this tree for wstring length of 11<br/>
`BST256   m_T256[5];`   //this tree for wstring length of 12<br/>

#### And more trees
wstring  ws = L"你好嗎";<br/>
int KeySum = 0;<br/>
for(int i = 0;i<ws.length();i++)<br/>
　　KeySum += ws[i];<br/>
KeySum = KeySum%500;<br/> // KeySum is a `key` of column for an 2D BST array
`BST64    m_T64[0][0-499];`   //this 500 trees shared all the wstring length of 1<br/>
`BST64    m_T64[1][0-499];`   //this 500 trees shared all the wstring length of 2<br/>
`BST64    m_T64[2][0-499];`   //this 500 trees shared all the wstring length of 3<br/>
`BST128   m_T128[0][0-499];`   //this 500 trees shared all the wstring length of 4<br/>
`BST128   m_T128[1][0-499];`   //this 500 trees shared all the wstring length of 5<br/>
`BST128   m_T128[2][0-499];`   //this 500 trees shared all the wstring length of 6<br/>
`BST128   m_T256[0][0-499];`   //this 500 trees shared all the wstring length of 7<br/>
`BST256   m_T256[1][0-499];`   //this 500 trees shared all the wstring length of 8<br/>
`BST256   m_T256[2][0-499];`   //this 500 trees shared all the wstring length of 9<br/>
`BST256   m_T256[3][0-499];`   //this 500 trees shared all the wstring length of 10<br/>
`BST256   m_T256[4][0-499];`   //this 500 trees shared all the wstring length of 11<br/>
`BST256   m_T256[5][0-499];`   //this 500 trees shared all the wstring length of 12<br/>

Search logic
--
###7 Chars Researcher

![](https://github.com/DD898989/Pictures/blob/master/7.png)<br/>

###3 Chars Researcher

![](https://github.com/DD898989/Pictures/blob/master/3.png)<br/>

