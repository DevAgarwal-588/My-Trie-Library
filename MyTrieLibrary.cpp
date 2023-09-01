// Author : SAHIL SHARMA

/*                                            
    FEATURES
    *GENERIC CLASS SUPPORTS EMPTY , SERIALISED STRING AND VECTORS OF WORDS
    *CAN INSERT A WORD INTO A TRIE 
    *CAN DELETE A WORD FROM A TRIE
    *CAN SEARCH A WORD IN A TRIE
    *CAN FIND THE NUMBER OF WORDS IN A TRIE
    *CAN SERIALIZE A TRIE INTO A STRING 
    *CAN DESERIALIZE A SERIALISED STRING INTO A TRIE
    *CAN FORM A VECTOR FROM A GIVEN TRIE
    *CAN COMPARE TWO TRIES 
    *CAN COUNT THE NUMBER OF WORDS WITH GIVEN PREFIX
    *CAN AUTO-SUGGEST BASED ON THE GIVEN PREFIX

*/

#include<bits/stdc++.h>
using namespace std;

//Structure of Node of a trie
struct trieNode{

    //Indicate if word it a word ending
    bool isEnd;

    //number of words have the prefix that is represented by this node
    int wordsWithPrefix;
    
    //Store all children of this node
    unordered_map<char,trieNode*> child;

    //Constructor
    trieNode(){
        isEnd=false;
        wordsWithPrefix=0;
    }
};

// Generic Class of Trie Data Strucature (Vector, Serialised_String , Empty)
class TrieDS{
    private:

        trieNode *root;
        int totalWords;

    //Helper function that traverse the trie and finds all word present in trie.
    void treeDepthTraversal(trieNode *currentNode,vector<string>& list,string word)
   {
   	    if(currentNode->isEnd)
   	    {
   	 	    list.push_back(word);
   	    }
   	    for(auto p : currentNode->child)
   	    {
   	 	    word.push_back(p.first);
   	 	    treeDepthTraversal(p.second, list, word);
   	 	    word.pop_back();
   	    }
   }

    //Helper for Auto-Suggest Function
    void helpAuto(string word,trieNode *currentNode){
        if(currentNode->isEnd)
   	    {
   	 	    cout<<word<<" ";
   	    }
   	    for(auto p : currentNode->child)
   	    {
   	 	    word.push_back(p.first);
   	 	    helpAuto(word,p.second);
   	 	    word.pop_back();
   	    }
    }

   //helper function for trieToString function
   void buildSerialTrie(trieNode *root, string &serialTrie)
   {
        if(root->isEnd){
             serialTrie.push_back(']');
        }
        unordered_map<char, trieNode*> &children = root->child;
        for(auto word : children)
        {
            serialTrie.push_back(word.first);
            buildSerialTrie(word.second, serialTrie);
        }
        serialTrie.push_back('>');
   }

    //Helper function to covert the serialised string to Trie
   int stringToTrie(trieNode *node, string &serialTrie, int &read)
   {
        int wordSeen = 0;
        if(serialTrie[read] == ']')
        {
           node->isEnd = 1;
           wordSeen++;
           read++;
        }
        else
        { 
            node->isEnd = 0;
        }

        unordered_map<char, trieNode*> &childMap = node->child;
        while(serialTrie[read] != '>')
        {
            char ch = serialTrie[read++];
            childMap[ch] = new trieNode();
            wordSeen += stringToTrie(childMap[ch], serialTrie, read);
        }
        read++;
        node->wordsWithPrefix = wordSeen;
        return wordSeen;
   }


    public:

    //used to construct an empty trie.
    TrieDS()
    {
        root = new trieNode();
        totalWords = 0;
    }

    //Used to construct a trie initialized with a list of words.
    TrieDS(vector<string> words)
    {   
        totalWords = 0;
        root = new trieNode();
        for(string word : words)
        {
       	    insertAtTrie(word);
        }
    }

    //Used to construct a trie using an existing trie given in serial format.
    TrieDS(string serialTrie)
    {
        root = new trieNode();
        int read = 0;
        totalWords = stringToTrie(root, serialTrie, read);
    }

    //gives the total number of words currently present in the trie.
    int numberOfWordsInTrie()
    {
        return totalWords;
    }

    //to insert a word in a trie
    void insertAtTrie(string word){

        if(searchInTrie(word)==true){
            return;
        }
        trieNode* curr=root;

        for(int i=0;word.length()>i;i++){
            curr->wordsWithPrefix++;
            unordered_map<char,trieNode*> &childMap = curr->child;
            auto it=childMap.find(word[i]);

            if(it!=childMap.end()){
                curr=childMap[word[i]];
            }else{
                curr=new trieNode();
                childMap[word[i]]=curr;
            }
        }
        
        curr->wordsWithPrefix++;
        curr->isEnd=true;
        totalWords++;
    }
    

    //to search a word in a trie
    bool searchInTrie(string word){
        trieNode* curr=root;

        for(int i=0;word.length()>i;i++){
            unordered_map<char,trieNode*> &childMap = curr->child;
            
            auto it=childMap.find(word[i]);

            if(it!=childMap.end()){
                curr=childMap[word[i]];
            }else{
                return false;
            }
        }
        if(curr->isEnd==true){
            return true;
        }else{
            return false;
        }

    }

    //Auto-Suggest Function in Trie 
    void autoSuggest(string word){
        trieNode* curr=root;
    
        for(int i=0;word.length()>i;i++){
            unordered_map<char,trieNode*> &childMap = curr->child;
            
            auto it=childMap.find(word[i]);

            if(it!=childMap.end()){
                curr=childMap[word[i]];
            }else{
                cout<<"No such word with the given Prefix exist"<<endl;
                return ;
            }
        }

        helpAuto(word,curr);

    }

    //Function to delete a word from the trie
    void deleteInTrie(string word){
        if(!searchInTrie(word)){
            return;
        }

        trieNode* curr=root;
        for(int i=0;word.length()>i;i++){
            curr->wordsWithPrefix--;
            unordered_map<char,trieNode*> &childMap = curr->child;
            trieNode* prev=curr;
            curr=childMap[word[i]];
            if(prev->wordsWithPrefix==0){
                delete prev;
            }
        }
            curr->isEnd==false;
            if(curr->wordsWithPrefix==0){
                delete curr;
            }

    }

    //Number of words in the trie the have a given prefix.
    int wordsWithThePrefix(string prefix)
    {
        if(root==NULL)
            return 0;
        trieNode *currentNode = root;
        for(int i = 0; i < prefix.length(); i++)
        {
            unordered_map<char,trieNode*>& childMap = (currentNode->child);
      	    auto it = childMap.find(prefix[i]);
      	    if(it == childMap.end())
      	    return 0;
      	    else
      		    currentNode = childMap[prefix[i]];
        }
            return currentNode->wordsWithPrefix;
    }

   
    //Used to convert the trie to a list of words(serialization).
    vector<string> trieToList()
    {
   	    vector<string> list;
   	    treeDepthTraversal(root, list, "");
   	    return list;
    }

   //Make a string using the given trie (Serialize it)
   string trieToString()
   {
       string serializedTrie;
       buildSerialTrie(root, serializedTrie);
       return serializedTrie;
   }

    
};

      //Comparison Operators for Trie
    bool compareTrie(TrieDS *t1,TrieDS *t2)
    {
        if(t2->numberOfWordsInTrie()!=t1->numberOfWordsInTrie()){
            return false;
        }

        vector<string> v1= t1->trieToList();
        vector<string> v2= t2->trieToList();

        sort(v1.begin(),v1.end());
        sort(v2.begin(),v2.end());

        if(v1==v2){
            return true;
        }

        return false;
    }



int main(){

    freopen ("new_input.txt", "r", stdin);                                      // Taken input from this file
    freopen ("new_output.txt", "w", stdout);                                    // Given output to this file

    cout<<"Enter some words to make a trie"<<endl;

    vector<string> input;
 	string word;

 	cin>>word;
 	while(word != "#END#")                                                      //NOTE-PASS #END# TO GET OUT OF THE LOOP
    {
       input.push_back(word);
       cin>>word;
    }

 	TrieDS *myTrie = new TrieDS(input);                                                     // Make a trie object using the words provided by user
 	cout<<"Your serialised String is:-"<<"   "<<myTrie->trieToString()<<endl;               // Converted a trie into a string  (Serialisation)
    cout<<"Total words in a Trie is:-"<<"   "<<myTrie->numberOfWordsInTrie()<<endl;          //Counted the number of words in a trie

    cout<<"Enter some words to insert in this trie"<<endl;
    cin>>word;
 	while(word != "#END#")
    {
       myTrie->insertAtTrie(word);                                                             // Inserted a word to the trie
       cin>>word;
    }
    
    string serialstr=myTrie->trieToString();                                                   // Coverted the trie into a string (Serialisation)
    cout<<"Your new serialised String is:-"<<"   "<<serialstr<<endl;
    
    cout<<"Enter a word to search in a trie:-   ";
    cin>>word;
    if(myTrie->searchInTrie(word)){                                                             //Finding a word in a Trie
        cout<<word<<" is present in the trie"<<endl;
    }else{
        cout<<word<<" is not present in a trie"<<endl;
    }

    cout<<"Total words in a Trie is:-"<<"   "<<myTrie->numberOfWordsInTrie()<<endl;             //Counted the number of words in a trie
        
    TrieDS *myTrie2 = new TrieDS(serialstr);                                                //Made a new Trie using the Serialised String of the old Trie
    vector<string> v2= myTrie2->trieToList();                                               //Make a vector with the words of new Trie
    cout<<"New Trie:-   ";
    for(auto a:v2){
        cout<<a<<" ";
    }
    cout<<endl;

    if(compareTrie(myTrie,myTrie2)){                                                                  //Compare the two Tries
        cout<<"Both tries are equal"<<endl;
    }else{
        cout<<"Both the tries are not equal"<<endl;
    }
    
    cout<<"Enter a word to delete from Trie :::"<<endl;
    cin>>word;
    myTrie2->deleteInTrie(word);                                                                //Delete a number from the New Trie
    v2= myTrie2->trieToList();                                                                  //Make a vector with the words of new Trie
    cout<<"Modified Trie after deleting '"<<word<<"' :-   ";
    for(auto a:v2){
        cout<<a<<" ";
    }
    cout<<endl;


    cout<<"Enter Prefix to find the count of words with given prefix ::: "<<endl;
    cin>>word;
    cout<<"Total number of words with prefix '"<<word<<"' are:-   "<<myTrie2->wordsWithThePrefix(word)<<endl;           //Find the total number of words with prefix

    if(compareTrie(myTrie,myTrie2)){                                                                                     //Compare the two Tries
        cout<<"Both tries are equal"<<endl;
    }else{
        cout<<"Both the tries are not equal"<<endl;
    }

    cout<<"Enter a prefix to find the words in a Trie with the given prefix"<<endl;
    cin>>word;
    cout<<"The words are:-   ";
    myTrie2->autoSuggest(word);                                                                         //Suggest the words with a given Prefix

 	return 0;
}
