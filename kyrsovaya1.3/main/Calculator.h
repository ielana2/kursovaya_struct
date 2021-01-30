#pragma once
#include "List.h"
#include <string>

using namespace std;

class Calculator {
private:
	//int MAX_SIZE = 256; //maximum size of the array for an input phrase
	string phrase; //string for an input phrase
	int phrase_size;
	List* prefix_phrase; //pointer to a list for prefix form

	float GetOperand(); //to get the right operand of the action
	float MakeNumber(); //to make an int from separate chars
	void DecodeConstants(float&, float&);
	void CreateClassForRecursion(List*, string, List*, int, int, bool); //to create a inner class for recursion which will process complicated operands
	void CheckBrackets(int, int); //to check if the brackets are placed correct int the phrase
	char GetSymbolType(string, int, int&, int, char); // to check symbols and mistakes after them
	void DecodePrefixPhrase(float); //to print prefix form to console correctly
	void ActionsSearch(List*, int, int); //to search actions in the input phrase
	void ChooseOperation(); //to know what operation has to be done


	void Add(int);
	void Substract(int);
	void Minus(int); //function for unary minus
	void Multiply(int);
	void Divide(int);
	void Elevate(int);
	void Cos(int);
	void Sin(int);
	void Tg(int);
	void Ctg(int);
	void Ln(int);
	void Log(int);
	void Sqrt(int);
	void Module(int);
public:
	Calculator(); //constructor
	~Calculator(); //destructor

	void SetPhrase(string); //to set phrase in new recursion class after creating it
	void PrefixOuput(); //to print the prefix form and the result
	void ReadPhrase(); //to get an input phrase from the file
	int GetPhraseSize(); //to know size of the input phrase
	void TransformPhrase(int, int); //transform the phrase to prefix form
	List* GetPrefixPhrase(); //to get prefix_phrase from recurtion class
	void CheckPhrase(int, int); //to check if the frase was correctly inputed
};