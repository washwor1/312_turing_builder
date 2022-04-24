//program that generates an adjacency list based on input (categories,alphabet,examples)
//made to assist me with a cs312 project in order to parse emoticons but could probably be useful for other things.

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iterator>
#include <fstream>

using namespace std;

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

//for testing 
template<typename K, typename V>
void print_map(multimap<K, V> const &m)
{
    for (auto const &pair: m) {
        std::cout << "{" << pair.first << " -> " << pair.second << "}\n";
    }
}


int main() {
    string cat,input;
    vector<multimap<char,char>> adj;
    vector<map<char,int>> states;
    bool dup = false;
    double othercount = 0;
    int count = 0;
    char tab = 9;
    ofstream myfile;
    myfile.open ("output.txt");

    adj.resize(2);
    adj[0].insert(make_pair('s','y'));


    while(cin>>input) {
        cat = "y"+input+"y";
        for(int i = 1;i<cat.size();i++) {
            if (adj.size()<cat.size()) adj.resize(cat.size());
            std::pair <std::multimap<char,char>::iterator, std::multimap<char,char>::iterator> ret;
            ret = adj[i].equal_range(cat[i-1]);
            for (std::multimap<char,char>::iterator it=ret.first; it!=ret.second; ++it){
                if (it->second==cat[i]) dup=true;
            }
            if (dup==false) adj[i].insert(make_pair(cat[i-1],cat[i]));
            else dup=false;
        }
    }  
    myfile.setf(ios::fixed);
    myfile.setf(ios::showpoint);
    myfile.precision(6);

    myfile << "// Save File for STEM\n// Version 1.00\n\n// State Format: name x y start accept\nSTATES:\n";
    for (double i = 0; i<adj.size(); i++) {
        for (auto const pair: adj[i]) {
            myfile<<tab<<count<< ' ' << i*100+100 << ' ' << othercount*100+100 << ' ';
            if (pair.second == 'y' && pair.first == 's') myfile << "true" << ' ';
            else myfile << "false" << ' ';
            if (pair.second == 'y' && pair.first != 's') myfile << "true" << ' ';
            else myfile << "false" << ' ';
            myfile << "0.980392 0.980392 0.823529 1.000000\n";
            if (states.size() != adj.size()) states.resize(adj.size());
            states[i].insert(make_pair(pair.second,count));
            count++;
            othercount++;
        }
        othercount = 0;
    }
    //0 1 b e STAY

    myfile << "\n// Transition format: fromStateId toStateId readCHar writeChar moveDirection\n// The Character '~' is the catchall character\nTRANSITION:\n";
    for (double i = 1; i<adj.size(); i++) {
        for (auto const pair: adj[i]) {
            myfile << tab << states[i-1].find(pair.first)->second << ' ' << states[i].find(pair.second)->second << ' ' << pair.second << ' ' << pair.second << ' ' << "RIGHT\n"; 
        }
    }

    myfile << "\n// Tape format: tapeChar(0) tapeChar(1) ... tapeChar(n)\nTAPE:\n"<<tab<<"0\n\n Start Triangle Position:0\n";
}