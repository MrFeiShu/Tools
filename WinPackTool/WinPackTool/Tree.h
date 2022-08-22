//Tree.h �ļ�
#pragma once
#include <list>
#include <algorithm>
using namespace std;

struct TreeNode;   //����һ���ṹ��ԭ��
class Tree;        //����һ����ԭ��
class Iterator;    //����һ����ԭ��
typedef list<TreeNode*> List; //������һ���ڵ�����

TreeNode* clone(TreeNode*, List&, TreeNode*);//Clone���ƺ���

struct TreeNode {
	int _data;                         //����
	TreeNode* _parent;                 //���ڵ�
	List _children;                    //�ӽڵ�
	TreeNode(int, TreeNode* );          //���캯��
	void SetParent(TreeNode& );         //���ø��ڵ�
	void InsertChildren(TreeNode& );    //�����ӽڵ�
};

class Tree{
public:

	//�����ǹ����������������
	Tree();                                //Ĭ�Ϲ��캯��
	Tree(const Tree&);                     //���ƹ��캯��		tytodo ���ƹ��캯������ͨ���캯������������ʲô��
	Tree(const int);                       //���������캯��
	Tree(const int,const list<Tree*>&);    //���������캯��
	~Tree();                               //��������
	Tree& operator=(const Tree&);           //=�������������
	bool operator==(const Tree&);           //==�������������
	bool operator!=(const Tree&);           //!=�������������

	//�����ǳ�Ա����
	void Clear();                         //���
	bool IsEmpty()const;                  //�ж��Ƿ�Ϊ��
	int Size()const;                      //����ڵ���Ŀ
	int Leaves();                         //����Ҷ����
	int Root()const;                      //���ظ�Ԫ��
	int Height();                         //�������ĸ߶�


	//�����Ǿ�̬��Ա����
	static bool IsRoot(Iterator);         //�ж��Ƿ��Ǹ�
	static bool isLeaf(Iterator);         //�ж��Ƿ���Ҷ��
	static Iterator Parent(Iterator);     //�����丸�ڵ�
	static int NumChildren(Iterator);     //�������ӽڵ���Ŀ

	//����������
	Iterator begin();                //Tree Begin
	Iterator end();                  //Tree End
	friend class Iterator;            //Iterator SubClass	// ��Ԫ������
private:
	list<TreeNode*> _nodes;            //�ڵ�����
	list<TreeNode*>::iterator LIt;     //һ���ڵ������
	int height(TreeNode*);
	int level(TreeNode*,Iterator);
};

//This is TreeSub Class Iterator
class Iterator{
private:
	Tree* _tree;                        //Tree data
	list<TreeNode*>::iterator _lit;     //List Iterator
public:
	Iterator();                                 //Ĭ�Ϲ��캯��
	Iterator(const Iterator&);                  //���ƹ��캯��
	Iterator(Tree*,TreeNode*);                  //���캯��
	Iterator(Tree*,list<TreeNode*>::iterator);  //���캯��
	//���������
	void operator=(const Iterator&);            //��ֵ���������
	bool operator==(const Iterator&);           //��ϵ���������
	bool operator!=(const Iterator&);           //��ϵ���������
	Iterator& operator++();                     //ǰ׺++�����
	Iterator operator++(int);                   //��׺++�����
	int operator*()const;                       //��ýڵ���Ϣ
	bool operator!();                           //��ֵ���������

	typedef list<TreeNode*>::iterator List;
	friend class Tree;
};