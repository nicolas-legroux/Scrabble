#ifndef TRIE_HPP_
#define TRIE_HPP_

#include <vector>
#include <fstream>

class TrieEdge{
private:
	unsigned int data = 0;
public:
	TrieEdge(unsigned int _data) : data(_data) { }

	/* Use 5 bits to store the letter label */

	TrieEdge(char letter){
		data |= letter - 'A';
	}

	char getLetter() const {
		return 'A' + (data & 0x1F);
	}
	
	/* Use 20 bits to store the index of the next node */

	void setOutwardNode(int i) {
		data |= (i << 5);
	}
	
	int getOutwardNode() const {
		return (data & (0xFFFFF << 5)) >> 5;
	}

	/* Use one bit to store whether the edge leads to a terminal node */ 

	void setTerminal(bool b){
		data |= (b << 25);
	}

	bool isTerminal() const {
		return data & (1 << 25);
	}

	/* Use one bit to store whether the edge is the last edge of the current Node */

	void setLastEdge(){
		data |= (1 << 26);
	}

	bool isLastEdge() const {
		return data & (1 << 26);
	}

	unsigned int getData() const {
		return data;
	}
};


class Trie{
private:
	std::vector<TrieEdge> edgeList;
	
	struct trie_constructor_helper{
		char c;
		unsigned int start, end;
		bool terminal;
		trie_constructor_helper(char _c, unsigned int _start, bool _terminal) :
			c(_c), start(_start), terminal(_terminal) { }
	};
	std::vector<trie_constructor_helper> buildHelpers(const std::vector<std::string> &dict, 
			unsigned int start, unsigned int end, unsigned int idx);
	void buildTrie(const std::vector<std::string> &dictionary, unsigned int start, 
			unsigned int end, unsigned int idx);
	void buildDictionary(int index, std::string *currentString, std::vector<std::string> *dict, 
			bool currentlyAtRoot);
	bool checkWord(unsigned int idx, std::string::const_iterator start, 
			std::string::const_iterator end, bool wasTerminal);
public:
	Trie() { }

	Trie(const std::vector<unsigned int> &trie){
		for(auto i : trie){
			edgeList.emplace_back(i);
		}
	}
	Trie(std::ifstream &inputFile);
	Trie(const std::vector<std::string> &dict); 	
	
	std::vector<TrieEdge> getEdgeList() const { return edgeList; }
	std::vector<std::string> getDictionary();
	unsigned int getSize() { return edgeList.size(); }

	bool checkWord(const std::string &word) { 
		return checkWord(0, word.cbegin(), word.cend(), true);
	}
};

#endif /* TRIE_HPP_ */
