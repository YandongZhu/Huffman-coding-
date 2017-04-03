#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <queue>
#include <bitset>

using namespace std;

struct tree
{
	int weight;
	int val;
	tree* left;
	tree* right;
	tree(int weight, int val): weight(weight), val(val) {
		left = NULL;
		right = NULL;
	}
	~tree(){
		delete left;
		delete right;
	}
	void setWeight(){
		if(left == NULL && right == NULL){
			weight = 0;
		}
		else if (left == NULL){
			weight = right->weight;
		}
		else if (right == NULL){
			weight = left->weight;
		}
		else{
			weight = right->weight + left ->weight;
		}
		return;
	}
};

int leap = 128;
const int length = 128;
int frequency[128];
int heapify[128];
int temp[128];
int order[128];
string code[128];
std::queue<tree*> q;

void initial(){
	for (int i = 0; i < length; ++i)
	{
		string binary = std::bitset<7>(i).to_string(); //to binary
    	code[i] = binary; // store T0
	}
}

void decode(){
	char input;
	string expect = "";
	int i = 0;
	while (i < leap)
	{
		input = cin.get();
		if (cin.eof()) {
			throw("did't read enough integer.");
			break;
			}
		expect += input;
		for (int j = 0; j < length; ++j)
		{
			if(expect == code[j]){
				char c = j;
				cout << c;
				//cout << expect << endl;
				//cout << j << " " << code[125] << endl;
				++i;
				frequency[j]++;
				expect = "";
				break;
			}
		}
	}
}

void assign(){
	for (int i = 0; i < length; ++i)
	{
		for (int j = 0; j < length; ++j)
		{
			// if we get the heapify and temp have the same value
			if (heapify[i] == temp[j]){
				order[i] = j;
				// set the temp to leap + 1 that wont repeat
				temp[j] = leap + 1;
				break;
			}
		}
	}
}

void updateCode(tree* root, string cur){
	if (root == NULL){
		return;
	}
	else if (root->left == NULL && root->right == NULL){
		int var = root->val;
		code[var] = cur;
	}
	else{
		updateCode(root->left, cur + "0");
		updateCode(root->right, cur + "1");
	}
}

void generateTree(){
	int i = 0;
	tree* first;
	tree* second;
	tree* root;
	int arrVal;
	int arrWeight;
	int treeWeight;
	while (i < length){
		// if the queue is empty
		if (q.size() == 0){
			// create the first least frequency node
			arrVal = order[i];
			arrWeight = frequency[arrVal];
			first = new tree(arrWeight, arrVal);
			// increase i by 1
			i++;
			// create the second least frequency node
			arrVal = order[i];
			arrWeight = frequency[arrVal];
			second = new tree(arrWeight, arrVal);
			// increase i by 1
			i++;
		}
		// if there is only one element in the queue
		else if(q.size() == 1){
			// get the weight of the first element of root
			treeWeight = q.front()->weight;
			// get the weight of the least frequency node
			arrVal = order[i];
			arrWeight = frequency[arrVal];
			// increase i by 1
			i++;
			if (treeWeight <= arrWeight)
			{
				// pop the first element of the queue and assign to first tree
				first = q.front();
				q.pop();
				// set the second element to be another leave
				second = new tree(arrWeight, arrVal);
			}
			else {
				first = new tree(arrWeight, arrVal);
				// get the weight of the least frequency node
				arrVal = order[i];
				arrWeight = frequency[arrVal];
				// increase i by 1
				i++;
				if (treeWeight <= arrWeight)
				{
					// pop the first element of the queue and assign to first tree
					second = q.front();
					q.pop();
				}
				else{
					// set the second element be the node
					second = new tree(arrWeight, arrVal);
				}
			}
		}
		// if the size of queue if greater than two
		else{
			// get the weight of the first element of root
			treeWeight = q.front()->weight;
			// get the weight of the least frequency node
			arrVal = order[i];
			arrWeight = frequency[arrVal];
			if (treeWeight <= arrWeight||i == 127){
				first = q.front();
				q.pop();
				treeWeight = q.front()->weight;
				if (treeWeight <= arrWeight)
				{
					second = q.front();
					q.pop();
				}
				else {
					// increase i by 1
					i++;
					second = new tree(arrWeight, arrVal);
				}
			}
			else{
				first = new tree(arrWeight, arrVal);
				// increase i by 1
				i++;	
				arrVal = order[i];
				arrWeight = frequency[arrVal];
				if (treeWeight <= arrWeight)
				{
					second = q.front();
					q.pop();
				}
				else{
					// increase i by 1
					i++;
					second = new tree(arrWeight, arrVal);
				}
			}
		}
		// create the root
		root = new tree(0, -1);
		// push two children into root;
		root->left = first;
		root->right = second;
		root->setWeight();
		// push root into the queue
		q.push(root);
	}
	// compress the queue
	while (q.size() > 1){
		first = q.front();
		q.pop();
		second = q.front();
		q.pop();
		// create the root
		root = new tree(0, -1);
		// push two children into root;
		root->left = first;
		root->right = second;
		root->setWeight();
		// push root into the queue
		q.push(root);
	}
	root = q.front();
	updateCode(root, "");
	q.pop();
	delete root;
}

int main(int argc, char const *argv[])
{
	initial();
	// begin to read the character
	try{
		while (!cin.eof()){
			// if did't read enough char then throw.
			decode();
			// copy the frequency arr to arr temp
			copy(frequency, frequency + length, temp);
			// copy the frequency arr to arr heapify
			copy(frequency, frequency + length, heapify);
			// sort the arr of frequency from least to most
			sort(heapify, heapify + length);
			//
			assign();
			// generate the tree
			generateTree();
			leap = leap * 2;
			copy(frequency, frequency + length, temp);
		}
	}
	catch(...){

	}
	return 0;
}
