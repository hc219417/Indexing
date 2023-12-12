/**
 * @file: indexing.cc
 * @author: Hannah Culver
 * @date: September 28, 2022
 * @brief: main program to test index-tree class
 * 
 * @objective: work with binary search trees
 * @pat: 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
 */

#include "index-tree.h"

/**
 * displayMenu
 * @param none
 * @return menu with four choices
 */
void displayMenu();

int main(){

    IndexTree mytree; //create index tree object
    string filename; //file to be indexed (inputted by the user)

    cout << "Welcome to your very own text file index! Please enter the name of the file you would like to index (case-sensitive): ";
    cin >> filename; //prompt the user and read in the file name

    mytree.insert(filename); //fill up the tree

    int choice; //choice variable for displayMenu()
    string found; //string for word search function

    do {
        displayMenu();
        cin >> choice;
        if(choice == 1){
            cout << "displaying index... one moment please" << endl;
            mytree.display_index();
        }
        else if(choice == 2){
            cout << "please enter the word you would like to find: ";
            cin >> found;
            mytree.word_search(found);
        }
        else if(choice == 3){
            mytree.most_common();
        }
        else if(choice == 4){
            mytree.unique_words();
        }
        else if(choice == 5){
            mytree.word_count();
        }
    } while (choice != 6);
    
    cout << "Come back soon!" << endl;

    return 0;
}

void displayMenu(){
    cout << "1. display index" << endl;
    cout << "2. search for a word" << endl;
    cout << "3. most common word" << endl;
    cout << "4. number of unique words" << endl;
    cout << "5. find word count" << endl;
    cout << "6. quit" << endl;
    cout << "please enter your choice: ";
}