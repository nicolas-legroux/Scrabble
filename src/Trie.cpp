#include <algorithm>

#include "Trie.hpp"

Trie::Trie(std::ifstream &inputFile){
	std::vector<std::string> dict;
	std::string s;
	while(std::getline(inputFile, s)){
		dict.push_back(s);
	}
	std::sort(dict.begin(), dict.end());
	buildTrie(dict, 0, dict.size(), 0);
}

Trie::Trie(const std::vector<std::string> &dict){
	std::vector<std::string> sorted_dict(dict);
	std::sort(sorted_dict.begin(), sorted_dict.end());
	buildTrie(sorted_dict, 0, sorted_dict.size(), 0);
}

std::vector<Trie::TrieConstructorHelper> Trie::buildHelpers(const std::vector<std::string> &dict, 
		unsigned int start, unsigned int end, unsigned int idx){
	char currentLetter = 0;
	std::vector<TrieConstructorHelper> newEdges;
	for(unsigned int i = start; i < end; ++i){
		if(dict[i][idx] != currentLetter){
			currentLetter = dict[i][idx];
			if(!newEdges.empty()){
				newEdges.back().end = i;
			}
			newEdges.emplace_back(currentLetter, i, (idx == dict[i].size() - 1));
		}
	}

	if(!newEdges.empty()){
		newEdges.back().end = end;
	}

	return newEdges;
}

// The dicionary is assumed to be sorted
void Trie::buildTrie(const std::vector<std::string> &dictionary, 
		unsigned int start, unsigned int end, unsigned int idx){
	
	unsigned int currentIdx = edgeList.size();
	
	// Step 1 : compute the set of letters which will be used to label edges at
	// the given position
	auto newEdgeHelpers = buildHelpers(dictionary, start, end, idx);
	for(const auto &newEdge: newEdgeHelpers){
		TrieEdge trieEdge(newEdge.letter);
		if(newEdge.terminal){
			trieEdge.setTerminal(true);
		}
		edgeList.push_back(trieEdge);
	}	
	edgeList.back().setLastEdge();

	// Step 2 : for each letter, recursively build the Trie
	for(unsigned int i = 0; i < newEdgeHelpers.size(); ++i){
		unsigned int next_start = newEdgeHelpers[i].start;
		unsigned int next_end = newEdgeHelpers[i].end;

		// The edge is terminal and there is only one element in this part of the tree, 
		// so we are done
		if(newEdgeHelpers[i].terminal && ((next_start+1) == next_end)){
			continue;
		}

		// If the edge is terminal, it won't be possible to extend the first word of the 
		// range, so we can increment the first pointer
		if(newEdgeHelpers[i].terminal){
			++next_start;
		}

		// Set the index of the next edge and recurse
		edgeList[currentIdx + i].setNext(edgeList.size());
		buildTrie(dictionary, next_start, next_end, idx+1);
	}
}

void Trie::buildDictionary(unsigned int startNode, std::string *currentString, 
		std::vector<std::string> *dict, bool currentlyAtRoot){
	if(startNode != 0 || currentlyAtRoot){
		unsigned int i = startNode;
		do{
			currentString->push_back(edgeList[i].getLetter());
			if(edgeList[i].isTerminal()){
				dict->push_back(*currentString);
			}

			buildDictionary(edgeList[i].getNext(), currentString, dict, false);
			currentString->pop_back();

		} while(!(edgeList[i++].isLastEdge()));
	}
}

std::vector<std::string> Trie::getDictionary(){
	std::string current;
	std::vector<std::string> dictionary;
	if(edgeList.size() > 0){
		buildDictionary(0, &current, &dictionary, true);
	}
	return dictionary;
}

// Assume that start and end pointers are not equal
bool Trie::checkWord(unsigned int startNode, std::string::const_iterator start, 
		std::string::const_iterator end){
	unsigned int i = startNode;
	do{
		if(edgeList[i].getLetter() == *start){
			++start;
			if(start == end){
				return edgeList[i].isTerminal();
			}
			if(edgeList[i].getNext() != 0){
				return checkWord(edgeList[i].getNext(), start, end);
			}
			return false;
		}
	} while(!edgeList[i++].isLastEdge());

	return false;
}

std::vector<Trie::Edge> Trie::getEdges(unsigned int node){
	unsigned int i = node;
	std::vector<Edge> edges;
	do{
		edges.emplace_back(edgeList[i].getLetter(), edgeList[i].getNext(), edgeList[i].isTerminal());
	} while(!edgeList[i++].isLastEdge());
	return edges;
}

// We assume that the prefix is valid (otherwise we return -1)
unsigned int Trie::findPrefix(unsigned int startNode, 
		std::string::const_iterator start, std::string::const_iterator end){
	unsigned int i = startNode;
	do{
		if(edgeList[i].getLetter() == *start){
			++start;
			if(start == end){
				return edgeList[i].getNext();
			}
			if(edgeList[i].getNext() != 0){
				return findPrefix(edgeList[i].getNext(), start, end);
			}
			return -1;
		}
	} while(!edgeList[i++].isLastEdge());
	
	return -1;
}
