#include <iostream>
#include "include/rapidjson/document.h" //used to implement rapidJson
#include <string>
#include <strstream>
#include <map>
#include <set>
#include <fstream>
#include "AVLTree.h"
#include <filesystem>
#include "PorterStemmer.h"
#include "HashMap.h"
#include <functional>

using namespace std;

set<string> stopwords = {"a","able","about","above","abroad","according","accordingly","across","actually","adj","after",
                         "afterwards","again","against","ago","ahead","ain't","all","allow","allows","almost","alone",
                         "along","alongside","already","also","although","always","am","amid","amidst","among","amongst",
                         "an","and","another","any","anybody","anyhow","anyone","anything","anyway","am", "be", "became",
                         "become", "but", "by", "can", "could", "did", "do", "does", "each", "either", "else", "for", "from",
                         "had", "has", "have", "hence", "how", "I", "if", "in", "is", "it", "its", "just", "may", "maybe",
                         "me", "might", "mine", "must", "my", "mine", "neither", "nor", "not", "of", "oh", "ok", "when",
                         "where", "whereas", "wherever", "whenever", "whether", "which", "while", "who", "whom", "whomever",
                         "whose", "why", "will", "with", "within", "without", "would", "yes", "yet", "you", "your"};



void normalize (string &token5){//normalize string
    int j = 0;

    for (int i = 0; i < token5.size() ; ++i) {
        if(token5[i]>='a'&& token5[i]<='z'){//utilizes ASCII values
            token5[j++]=token5[i];
        }
        else if(token5[i]>='A'&& token5[i]<='Z'){
            token5[j++]=token5[i]+('a'-'A');
        }
        else if(token5[i]==' '){
            token5[j++]=token5[i];
        }
    }
    token5.resize(j);//actually changes object being passed when using &
}

bool isStopWord(string word){
    stopwords.contains(word);
    return stopwords.contains(word);
};

string stem(string w){//porter stemming algo
    Porter2Stemmer::stem(w);
    return w;
}

string readFile(const char* doc){//opens file and reads one line at a time to return to json
    ifstream in(doc);
    string contents;
    string temp;
    while (getline(in, temp)){//opens doc for reading, inputs into temp, appends to contents
        contents += temp;
    }
    return contents;
}

struct entry {string word; set<int> id;};//define entry to only compare word, set is to handle multiple instances


AvlTree <entry> revIndex;//places into AVL tree

vector<string> fileIndex;

template <> struct std::hash<entry>{
    int operator()(const entry& ent){
        return std::hash<string>()(ent.word);
    }
};

HashTable <entry> revIndexPerson;


HashTable <entry> revIndexOrgs;



bool operator<(const entry& first, const entry& second) {
    return first.word<second.word; //compares words will be indexed
}

set<int> query(string word) {//query function
    entry query;
    query.word = stem(word);

    entry *result = revIndex.get(query);

    if (result == nullptr) {
        return set<int>();

    } else {
        return result->id;//auto will tell compiler to figure out type
    }
}

set<int> queryPerson(string word) {//query function for Persons
    entry query;
    query.word = word;

    entry *result = revIndex.get(query);

    if (result == nullptr) {
        return set<int>();

    } else {
        return result->id;//auto will tell compiler to figure out type
    }
}

set<int> queryOrgs(string word) {//query function for Orgs
    entry query;
    query.word = word;

    entry *result = revIndex.get(query);

    if (result == nullptr) {
        return set<int>();

    } else {
        return result->id;//auto will tell compiler to figure out type
    }
}


void display(vector<int>result){
    for(int i = 0;i < result.size(); i++){
        cout << i << " " << endl;

        string fileName = fileIndex[result[i]];

        string content = readFile(fileName.c_str());

        rapidjson::Document doc;

        doc.Parse(content.c_str());

        cout << doc["title"].GetString() << " ";

        cout << doc["author"].GetString() << " ";

        cout << doc["published"].GetString() << endl;
    }
}

vector<int> ranking(set<int> results){//calls after id is returned// up to 15 lowest documents//look up json file with index//display data
    return vector<int>(results.begin(), results.end());//returns aplha
}

set<int> runQuery(string search){//start out implementing single word queries

    string word;

    istrstream in(search.c_str());

    in >> word;

    set<int> results;

    set<int> resultTwo;

    if(word!="AND"&&word!="OR"){
       results = query(word);

    }

    else if (word == "AND"){

        in >> word;//first word
        set<int> resultOne = query(word);
        in >> word;
        set<int> resultTwo = query(word);

            for(int i:resultOne){
                if(resultTwo.contains(i)){
                    results.insert(i);
                }
            }
    }

    else if (word == "OR"){

        in >> word;//first word
        results = query(word);
        in >> word;
        set<int> resultTwo = query(word);

        for(int i:resultTwo){
                results.insert(i);
            }
        }

    while(in >> word){

        if(word == "NOT"){
            in >> word;
            set<int> resultNot = query(word);

            for(int i:resultNot){
                results.erase(i);
            }
        }

        if(word == "PERSON"){
            in >> word;
            set<int> person = queryPerson(word);
            set<int> resultPerson;

            for(int i:person){
                if(results.contains(i)){
                    resultPerson.insert(i);
                }
            }
            results = resultPerson;
        }

        if(word == "ORG"){
            in >> word;
            set<int> person = queryOrgs(word);
            set<int> resultOrgs;

            for(int i:person){
                if(results.contains(i)){
                    resultOrgs.insert(i);
                }
            }
            results = resultOrgs;
        }
    }
    return results;
}

//

int main(int argc, char** argv) {

    int number =0;
    string word;
    string filename;

    //Code below used for parsing with Json
    for (auto ent: filesystem::recursive_directory_iterator("../data/")) {//finds all files


        if (! ent.is_regular_file()||! (ent.path().extension().string() == ".json")) {
            continue;//check file is valid
        }

        const char* filename = ent.path().c_str();//c_str turns into string

        rapidjson::Document doc;

        string wholeFile;

        wholeFile = readFile(filename);

        fileIndex.push_back(filename);//adds file number to vector

        doc.Parse(wholeFile.c_str());//json parsing

        if(! doc.HasMember("text")){//checks is article jas "text"
                continue;
        }

        string text = doc["text"].GetString();//gets string value of hashtable of text


        for(auto& person:doc["entities"]["persons"].GetArray()){//
            string word = person["name"].GetString();

            entry x;

            x.word = word;

            entry *current = revIndexPerson.get(x);//x here is stemmed word, checks index for word

            if (current == nullptr) {//if found will return pointer, if not return null

                x.id.insert(number);
                revIndexPerson.insert(x);
            }

            else {
                current->id.insert(number);//Need to add to documents that already contain word, already seen word
            }
        }

        for(auto& person:doc["entities"]["organizations"].GetArray()){//make own function
            string word = person["name"].GetString();

            entry x;

            x.word = word;

            entry *current = revIndexOrgs.get(x);//x here is stemmed word, checks index for word

            if (current == nullptr) {//if found will return pointer, if not return null

                x.id.insert(number);
                revIndexOrgs.insert(x);
            }

            else {
                current->id.insert(number);//Need to add to documents that already contain word, already seen word
            }
        }



        normalize(text);

        istrstream in(text.c_str());//treats string as input stream, using to split into words


        while (in >> word) {//checks if words remaining

            if (isStopWord(word)) {
                continue;
            }

            entry x;

            x.word = stem(word);

            entry *current = revIndex.get(x);//x here is stemmed word, checks index for word

            if (current == nullptr) {//if found will return pointer, if not return null

                x.id.insert(number);
                revIndex.insert(x);
                }

            else {
                current->id.insert(number);//Need to add to documents that already contain word, already seen word
            }
        }
        number++;
    }

    string buffer;

    while(getline(cin, buffer)){

        set<int> result = runQuery(buffer);
        vector<int> rank = ranking(result);
        display(rank);//give the option to display full text
    }

    }


//usr/bin/time -l
//program instruments
//ranking and author

//ranking TDIEF, how many times word occurs, look at PA01