# WordFilter_2_IntervewProject_killuplus300-at-gmail.com

wstring to number
--
你 = 20320 = 0X4F60
好 = 22909 = 0X597D
嗎 = 21966 = 0X55CE

wstring length less than 4 could be stored by a 64 bits type

ex:
wstring  ws = L"你好嗎";
uint64_t n = 0;
for(int i = 0;i<3;i++)
    n += (n<<20)+ws[i]; //n = 0X4F600597D055CE

Therefore:
wstring length less than 7  could be stored by a 128 bits type
wstring length less than 13 could be stored by a 256 bits type

binary tree
--
So we can store wstring into a binary:
![](https://github.com/DD898989/Pictures/blob/master/tree.PNG) 

For less space operation, we might make three classes for these wstring:
`BST64   m_T64;`   this tree for wstring length of 1~3
`BST128 m_T128;`   this tree for wstring length of 4~6
`BST256 m_T256;`   this tree for wstring length of 7~12
