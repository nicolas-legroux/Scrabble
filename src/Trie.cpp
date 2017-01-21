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

std::vector<Trie::trie_constructor_helper> Trie::buildHelpers(const std::vector<std::string> &dict, unsigned int start, 
		unsigned int end, unsigned int idx){
	char current_c = '0';
	std::vector<trie_constructor_helper> v;
	for(unsigned int i = start; i < end; ++i){
		if(dict[i][idx] != current_c){
			current_c = dict[i][idx];
			if(!v.empty()){
				v.back().end = i;
			}
			v.emplace_back(current_c, i, (idx == dict[i].size() - 1));
		}
	}

	if(!v.empty()){
		v.back().end = end;
	}

	return v;
}

void Trie::buildTrie(const std::vector<std::string> &dictionary, unsigned int start, unsigned int end,
		unsigned int idx){
	
	auto helpers = buildHelpers(dictionary, start, end, idx);
	unsigned int currentIdx = edgeList.size();

	for(const auto &element: helpers){
		TrieEdge trieEdge(element.c);
		if(element.terminal){
			trieEdge.setTerminal(true);
		}
		edgeList.push_back(trieEdge);
	}
	
	edgeList.back().setLastEdge();

	for(unsigned int i = 0; i < helpers.size(); ++i){
		unsigned int next_start = helpers[i].start;
		unsigned int next_end = helpers[i].end;
		if(helpers[i].terminal && ((next_start+1) == next_end)){
			// Done
			continue;
		}
		if(helpers[i].terminal){
			++next_start;
		}
		edgeList[currentIdx + i].setOutwardNode(edgeList.size());
		buildTrie(dictionary, next_start, next_end, idx+1);
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

void Trie::buildDictionary(int index, std::string *current, std::vector<std::string> *dict, 
		bool currentlyAtRoot){
	if(index != 0 || currentlyAtRoot){
		int i = index;
		do{
			current->push_back(edgeList[i].getLetter());
			if(edgeList[i].isTerminal()){
				dict->push_back(*current);
			}

			buildDictionary(edgeList[i].getOutwardNode(), current, dict, false);
			current->pop_back();

		} while(!(edgeList[i++].isLastEdge()));
	}
}

bool Trie::checkWord(unsigned int idx, std::string::const_iterator start, 
		std::string::const_iterator end, bool wasTerminal){
	if (start == end){
		return wasTerminal;
	}

	unsigned int i = idx;
	do{
		if(edgeList[i].getLetter() == *start){
			return checkWord(edgeList[i].getOutwardNode(), ++start, end, edgeList[i].isTerminal());
		}
	} while(!edgeList[i++].isLastEdge());

	return false;
}

std::vector<Trie::Edge> Trie::getOutwardEdges(unsigned int node){
	unsigned int i = node;
	std::vector<Edge> edges;
	do{
		edges.emplace_back(edgeList[i].getLetter(), edgeList[i].getOutwardNode(), edgeList[i].isTerminal());
	} while(!edgeList[i++].isLastEdge());
	return edges;
}

unsigned int Trie::findPrefixNode(unsigned int node, 
		std::string::const_iterator start, std::string::const_iterator end){
	unsigned int i = node;
	do{
		if(edgeList[i].getLetter() == *start){
			if((start+1) == end){
				return edgeList[i].getOutwardNode();
			}
			else{
				return findPrefixNode(edgeList[i].getOutwardNode(), start+1, end);
			}
		}
	} while(!edgeList[i++].isLastEdge());
	
	return 0;
}
