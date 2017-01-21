#ifndef TRIE_HPP_
#define TRIE_HPP_

#include <vector>
#include <fstream>

class Trie{
	private:

		// A class which efficiently stores the edges of a Trie by using 32 bit integers
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
				void setNext(unsigned int i) {
					data |= (i << 5);
				}

				unsigned int getNext() const {
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
		};

		// A trie is stored as a vector of TrieEdge
		std::vector<TrieEdge> edgeList;

		// A helper struct used during the construction of the Trie
		struct TrieConstructorHelper{
			char letter;
			unsigned int start, end;
			bool terminal;
			TrieConstructorHelper(char _letter, unsigned int _start, bool _terminal) :
				letter(_letter), start(_start), terminal(_terminal) { }
		};

		// Helper functions used during the construction of the Trie
		std::vector<TrieConstructorHelper> buildHelpers(const std::vector<std::string> &dict, 
				unsigned int start, unsigned int end, unsigned int idx);
		void buildTrie(const std::vector<std::string> &dictionary, unsigned int start, 
				unsigned int end, unsigned int idx);
		
		// Helper function used when computing the dictionary recongized by the Trie
		void buildDictionary(unsigned int index, std::string *currentString, std::vector<std::string> *dict, 
				bool currentlyAtRoot);

		// Helper function to check whether a word is in the dictionary
		bool checkWord(unsigned int startNode, std::string::const_iterator start, 
				std::string::const_iterator end);
		
		// Helper function to find the internal node when looking for a prefix
		unsigned int findPrefix(unsigned int node, std::string::const_iterator start, 
				std::string::const_iterator end);
	public:
		// A class which can be used to easily represent an edge
		class Edge{
			private:
				char letter;
				unsigned int nextNode;
				bool terminal;
			public:
				Edge(char l, unsigned int next, bool t) : letter(l), nextNode(next), terminal(t) { }
				char getLetter() const { return letter; }
				unsigned int getNextNode() const { return nextNode; }
				bool isTerminal() const { return terminal; };
		};

		Trie() { }

		// Initialize a Trie from an array of integers
		Trie(const std::vector<unsigned int> &trie){
			for(auto i : trie){
				edgeList.emplace_back(i);
			}
		}

		// Initialize a Trie from a file containing the dictionary
		Trie(std::ifstream &inputFile);

		// Initialize a Trie from a vector of string representing the dictionary
		Trie(const std::vector<std::string> &dict); 	

		// Get the outward edges of a node
		std::vector<Edge> getEdges(unsigned int node);

		// Find the internal node obtained after traversal by a prefix
		unsigned int findPrefix(const std::string &s){
			if(s.empty()) return 0;
			return findPrefix(0, s.cbegin(), s.cend());
		}

		// Compute the dictionary represented by the Trie
		std::vector<std::string> getDictionary();

		// Return the number of edges stored by the Trie
		unsigned int getSize() { return edgeList.size(); }

		// Check whether a word is in the dictionary
		bool checkWord(const std::string &word) { 
			if(word.empty()) return false;
			return checkWord(0, word.cbegin(), word.cend());
		}

		// Check for a suffix after starting at an internal node
		bool checkSuffix(unsigned int startNode, const std::string &suffix){
			if(suffix.empty()) return false;
			return checkWord(startNode, suffix.cbegin(), suffix.cend());
		}
};

#endif /* TRIE_HPP_ */
