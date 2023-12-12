/**
 * @file: index-tree.h
 * @author: Hannah Culver
 * @date: September 28, 2022
 * @brief: header file for the index-tree class
 */

#ifndef INDEXTREE
#define INDEXTREE

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

struct Indices{
    Indices(){line_number = 0, occurrences = 0;} //zero-arg c-tor

    Indices(int new_line_number, int new_occurrences){ //full service c-tor
        line_number = new_line_number,
        occurrences = new_occurrences;
    }

    int line_number;
    int occurrences;
};

struct BSTNode{
    BSTNode(){total = 0, word = "", left = nullptr, right = nullptr;} //zero-arg c-tor

    BSTNode(int new_total, string new_word, BSTNode *leftptr, BSTNode *rightptr){ //full service c-tor
        total = new_total,
        word = new_word,
        left = leftptr,
        right = rightptr;
    
    }

    int total; //total number of occurrences for a given word
    string word;

    vector<Indices> index_vector; //vector of indices (separate structure)

    BSTNode *left;
    BSTNode *right;
};

class IndexTree{
    public:
    IndexTree(); //default constructor

    //string functions
    string to_lower(string input_string); //convert string to lowercase
    string strip_poss(string input_string); //strip possessive ('s)
    string strip_punct(string input_string); //strip punctuation from beginning and end - NOT middle (hyphenated words, for example)

    string all(string input_string); //combination of all string functions
    bool all_punct(string input_string); //returns true if a string is all punctuation ("...")

    //input filename
    void process_file(string input_filename); //process input file
    void insert(string input_filename){process_file(input_filename);} //recursive

    //main functions
    void display_index(){tree_display_index(root);} //display index in order
    void word_search(string target){tree_word_search(root, target);} //search for a word (ask the user for a word and display the node information where the word was found)
    void most_common(){tree_most_common(root);} //most common word (return the first most common word and how many times it occurred)

    void unique_words(){find_unique(root);} //number of unique words in a file (excluding ignore words)
    void word_count(){find_wc(root);} //total number of words in a file (again, excluding ignore words)

    private:
    BSTNode *root; //root

    //private functions
    void tree_insert(BSTNode *&root, string input_word, int current_line_number);
    void tree_display_index(BSTNode *root);
    void tree_word_search(BSTNode *root, string target);
    void tree_most_common(BSTNode *root);

    //helper functions for extra stuff
    size_t tree_size(const BSTNode *root);
    size_t tree_traversal(const BSTNode *root);
    
    //most_common
    void collect_totals(BSTNode *root);
    void max_total();
    void find_most_common(BSTNode *root);

    //extra
    void find_unique(BSTNode *root); //# of unique
    void find_wc(BSTNode *root); //find word count

    //miscellaneous private variables
    int num_times; //number of times of most common word (from max_total)
    string most_common_string; //most common word (from find_most_common)
    string found_string; //for word search function
    string display_index_string; //for display index function
    vector<int> total_collection; //collection of totals (from collect_totals)
    vector<string> ignore; //vector of strings to ignore (from ignore.txt)
};

#endif

IndexTree::IndexTree(){ //default constructor
    root = nullptr;
    ifstream inStream;
    string input_filename = "ignore.txt";
	inStream.open(input_filename); //open file
	if(inStream.fail()){
		cout << "Error: file not found" << endl;
		exit(0);
	}
    string ignore_word;
    while(!inStream.eof()){
        while(inStream >> ignore_word){
            ignore.push_back(ignore_word); //fill up ignore vector
        }
    }
    inStream.close(); //close file
}

void IndexTree::process_file(string input_filename){
    ifstream inStream;
	inStream.open(input_filename); //open file
	if (inStream.fail()){
		cout << "Error: file not found" << endl;
		exit(0);
	}

    bool found = false;
    stringstream ss;
    string file_line;
    string file_word;
    int line_number_count = 0;

    while(getline(inStream, file_line)){
        line_number_count++;
        stringstream ss(file_line);
        while(ss >> file_word){
            for(auto it = ignore.begin(); it != ignore.end(); it++){
                if(*it == all(file_word)){
                    found = true;
                }
            }
            if(found != true){ //insert only if word is NOT found in the ignore vector
                tree_insert(root, all(file_word), line_number_count);
            }
            found = false;
        }
    }

    inStream.close(); //close file
}

void IndexTree::tree_insert(BSTNode *&root, string input_word, int current_line_number){
    if(root == nullptr){ //empty tree (first entry is being added)
        root = new BSTNode(1, input_word, nullptr, nullptr);
        root -> index_vector.push_back(Indices(current_line_number, 1));
    }
    else if(input_word < root -> word){
        tree_insert(root -> left, input_word, current_line_number);
    }
    else if(input_word == root -> word){
        root -> total++; //increment total
        auto it = root -> index_vector.back(); //look at last entry
        int current_occurrences = it.occurrences; //set aside current number of occurrences
        if(it.line_number == current_line_number){
            root -> index_vector.pop_back(); //remove last entry
            root -> index_vector.push_back(Indices(current_line_number, current_occurrences + 1));
        } //add entry back, this time incrementing the total number of occurrences within that line
        else{
            root -> index_vector.push_back(Indices(current_line_number, 1));
        }
    }
    else if(input_word > root -> word){
        tree_insert(root -> right, input_word, current_line_number);
    }
    //else? hmm... not sure what else
}

void IndexTree::tree_display_index(BSTNode *root){
    if(root != nullptr){ //in-order traversal
        tree_display_index(root -> left);
        display_index_string = root -> word + ": [";
        for(auto it = root -> index_vector.begin(); it != root -> index_vector.end(); it++){
            display_index_string += "<" + to_string(it -> line_number) + ", " + to_string(it -> occurrences) + ">" + ", ";
        }
        display_index_string.erase(display_index_string.length() - 2);
        cout << display_index_string << "]" << endl;
        tree_display_index(root -> right);
    }
}

void IndexTree::tree_word_search(BSTNode *root, string target){
    if(root == nullptr){ //empty tree
        cout << "Error: word not found" << endl;
    }
    else if(to_lower(target) < root -> word){
        tree_word_search(root -> left, target);
    }
    else if(to_lower(target) == root -> word){ //word was found
        found_string = "Found " + to_lower(target) + " (" + to_string(root -> total) + " instances): [";
        for(auto it = root -> index_vector.begin(); it != root -> index_vector.end(); it++){
            found_string += "<" + to_string(it -> line_number) + ", " + to_string(it -> occurrences) + ">" + ", ";
        }
        found_string.erase(found_string.length() - 2);
        cout << found_string << "]" << endl;
    }
    else if(to_lower(target) > root -> word){
        tree_word_search(root -> right, target);
    }
}

void IndexTree::tree_most_common(BSTNode *root){
    collect_totals(root); //collect all the totals
    max_total(); //find the maximum total
    find_most_common(root); //find the first instance of the word that matches the maximum total
    cout << "The most common word is " << most_common_string << ", it occurred " << num_times << " times." << endl;
}

void IndexTree::collect_totals(BSTNode *root){
    if(root != nullptr){ //in-order traversal
        collect_totals(root -> left);
        total_collection.push_back(root -> total);
        collect_totals(root -> right);
    }
}

void IndexTree::max_total(){
    num_times = *total_collection.begin(); //beginning of total collection vector
    if(root == nullptr){
        cout << "Error: empty tree" << endl;
    }
    else{
        for(auto it = total_collection.begin(); it != total_collection.end(); it++){
            if(*it > num_times){
                num_times = *it; //find the maximum element of the total collection vector
            }
        }
    }
}

void IndexTree::find_most_common(BSTNode *root){
    if(root != nullptr){ //in-order traversal
        find_most_common(root -> left);
        if(root -> total == num_times){
            most_common_string = root -> word;
        }
        find_most_common(root -> right);
    }
}

size_t IndexTree::tree_size(const BSTNode *root){
    if(root == nullptr){
        return 0;
    } //copied function from notes
    return 1 + tree_size(root -> left) + tree_size(root -> right);
}

void IndexTree::find_unique(BSTNode *root){
    cout << "There are a total of " << tree_size(root) << " unique words in your file." << endl;
}

size_t IndexTree::tree_traversal(const BSTNode *root){
    if(root == nullptr){
        return 0;
    } //similar to size function
    return root -> total + tree_traversal(root -> left) + tree_traversal(root -> right);
}

void IndexTree::find_wc(BSTNode *root){
    cout << "There are a total of " << tree_traversal(root) << " words in your file (excluding ignore words)." << endl;
}

string IndexTree::to_lower(string input_string){ //tolower
    for(unsigned int i = 0; i < input_string.length(); i++){
        input_string[i] = tolower(input_string[i]);
    }
    return input_string;
}

string IndexTree::strip_poss(string input_string){ //strip possessive
    if(input_string.length() >= 2){ //string is at least two characters long
        string last_two_ch = input_string.substr(input_string.length() - 2);
        if(last_two_ch == "'s"){ //word is possessive (last two characters equal 's)
            input_string.erase(input_string.length() - 2, 2);
        } //remove the last two characters and return the string
    }
    return input_string;
}

string IndexTree::strip_punct(string input_string){ //strip punctuation
    int start, end;
    int size = input_string.size(); //initialize some variables
    if(!ispunct(input_string[0])){
        start = 0; //string already starts with a character
    }              //nothing needs removed from the front
    else{
        for(int i = 0; i < size; i++){ //traverse the string forwards
            if(!ispunct(input_string[i])){
                start = i; //find the first instance of a character
                break;     //from the front of the string
            }
        }
    }
    if(!ispunct(input_string[size - 1])){
        end = size; //string already ends with a character
    }               //nothing needs removed from the back
    else{
        for(int i = size - 1; i > 0; i--){ //traverse the string backwards
            if(!ispunct(input_string[i])){
                end = i; //find the first instance of a character
                break;   //from the back of the string
            }
        }
    }
    return input_string.substr(start, (end - start) + 1); //return everything in between (hyphens included)
}

bool IndexTree::all_punct(string input_string){ //all punctuation
    size_t count = 0;
    for(unsigned int i = 0; i < input_string.length(); i++){
        if(ispunct(input_string[i])){
            count++;
        }
    }
    if(count == input_string.length()){
        return true;
    }
    return false;
}

string IndexTree::all(string input_string){ //all string functions (plus all_punct)
    //to_lower
    for(unsigned int i = 0; i < input_string.length(); i++){
        input_string[i] = tolower(input_string[i]);
    }
    //strip_poss
    if(input_string.length() >= 2){ //string is at least two characters long
        string last_two_ch = input_string.substr(input_string.length() - 2);
        if(last_two_ch == "'s"){ //word is possessive (last two characters equal 's)
            input_string.erase(input_string.length() - 2, 2);
        } //remove the last two characters and return the string
    }
    //strip_punct
    int start, end;
    int size = input_string.size(); //initialize some variables
    if(!ispunct(input_string[0])){
        start = 0; //string already starts with a character
    }              //nothing needs removed from the front
    else{
        for(int i = 0; i < size; i++){ //traverse the string forwards
            if(!ispunct(input_string[i])){
                start = i; //find the first instance of a character
                break;     //from the front of the string
            }
        }
    }
    if(!ispunct(input_string[size - 1])){
        end = size; //string already ends with a character
    }               //nothing needs removed from the back
    else{
        for(int i = size - 1; i > 0; i--){ //traverse the string backwards
            if(!ispunct(input_string[i])){
                end = i; //find the first instance of a character
                break;   //from the back of the string
            }
        }
    }
    if(all_punct(input_string)){ //return just the input string if everything is punctuation
        return input_string;
    }
    return input_string.substr(start, (end - start) + 1); //return everything in between (hyphens included)
}

//eof