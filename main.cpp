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

using namespace std;

set<string> stopwords = {"able","about","above","abroad","according","accordingly","across","actually","adj","after",
                         "afterwards","again","against","ago","ahead","ain't","all","allow","allows","almost","alone",
                         "along","alongside","already","also","although","always","am","amid","amidst","among","amongst",
                         "an","and","another","any","anybody","anyhow","anyone","anything","anyway","zero"};//Will add more

void normalize (string &token5){//normalize string
    int j = 0;

    for (int i = 0; i < token5.size() ; ++i) {
        if(token5[i]>='a'&& token5[i]<='z'){
            token5[j++]=token5[i];
        }
        else if(token5[i]>='A'&& token5[i]<='Z'){
            token5[j++]=token5[i]+('a'-'A');
        }
        else if(token5[i]==' '){
            token5[j++]=token5[i];
        }
    }
    token5.resize(j);
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
    while (getline(in, temp)){
        contents += temp;
    }
    return contents;
}

struct entry {string word; set<string> id;};

AvlTree <entry> revIndex;//places into AVL tree

bool operator<(const entry& first, const entry& second) {
    return first.word<second.word; //compares words will be indexed
}

int main(int argc, char** argv) {

    string number;
    string word;


    //Code below used for parsing with Json
    for (auto ent: filesystem::recursive_directory_iterator("../data/")) {//finds all files


        if (ent.is_regular_file()) {
            if (ent.path().extension().string() == ".txt") {
                string filename = ent.path().c_str();
                std::cout << filename << "TEST" << endl;
            }
            ifstream input(ent.path().c_str());//finds file path

            rapidjson::Document doc;

            string wholeFile;
            wholeFile = readFile(ent.path().c_str());

            doc.Parse(wholeFile.c_str());

            if(! doc.HasMember("text")){
                continue;
            }

            string text = doc["text"].GetString();//gets string value of hashtable of text


            if(doc.HasMember("uuid")){
                number = doc["uuid"].GetString();
            }
            else{
                number = "Empty String";
            }

            normalize(text);

            istrstream in(text.c_str());//treats string as input stream


            while (in >> word) {
                if (isStopWord(word)) {
                    continue;
                }
                entry x;
                x.word = stem(word);

                entry *current = revIndex.get(x);
                if (current == nullptr) {
                    x.id.insert(number);
                    revIndex.insert(x);
                } else {
                    current->id.insert(number);
                }
            }
        }
    }

    entry query;
    query.word = stem(argv[1]);
    revIndex.get(query);

        entry *result = revIndex.get(query);

    if (result == nullptr) {
        cout << "argv1 not found" <<endl;

    } else {
        for (auto id: result->id) {
            cout << id << endl;
        }
    }
}