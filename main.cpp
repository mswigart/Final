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

struct entry {string word; set<string> id;};//define entry to only compare word, set is to handle multiple instances


AvlTree <entry> revIndex;//places into AVL tree

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

int main(int argc, char** argv) {

    string number;
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

        doc.Parse(wholeFile.c_str());//json parsing

        if(! doc.HasMember("text")){//checks is article jas "text"
                continue;
        }

        string text = doc["text"].GetString();//gets string value of hashtable of text


        if(doc.HasMember("uuid")){
                number = doc["uuid"].GetString();
            }
        else{
                number = "Empty String";
            }

        for(auto& person:doc["entities"]["persons"].GetArray()){
            string word = person["name"].GetString();

            entry x;

            x.word = word;

            entry *current = revIndexPerson.get(x);//x here is stemmed word, checks index for word

            if (current == nullptr) {//if found will return pointer, if not return null

                x.id.insert(filename);
                revIndexPerson.insert(x);
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

                x.id.insert(filename);
                revIndex.insert(x);
                }

            else {
                current->id.insert(number);//Need to add to documents that already contain word, already seen word
            }
        }
    }


    entry query;
    query.word = stem(argv[1]);

    entry *result = revIndex.get(query);

    if (result == nullptr) {
        cout << "argv1 not found" <<endl;

    }
    else {
        for (auto id: result->id) {//auto will tell compiler to figure out type
            cout << id << endl;
        }
    }
}

//usr/bin/time -l
//program instruments
//perl -pe 'chomp;print qq{“$_”,\n}’
//ifstream in("stopwords.txt”)
//ifstream in("stopwords.txt”)
//while (in >> word) { stopwords.insert(word); }