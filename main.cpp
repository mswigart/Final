#include <iostream>
#include "include/rapidjson/document.h" //used to implement rapidJson
#include <string>
#include <strstream>
#include <map>
#include <set>
#include <fstream>
#include "AVLTree.h"
#include <filesystem>

using namespace std;

set<string> stopwords = {"able","about","above","abroad","according","accordingly","across","actually","adj","after",
                         "afterwards","again","against","ago","ahead","ain't","all","allow","allows","almost","alone",
                         "along","alongside","already","also","although","always","am","amid","amidst","among","amongst",
                         "an","and","another","any","anybody","anyhow","anyone","anything","anyway","zero"};//Will add more

string normalize (string token5){//normalize string
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
    return token5.substr(0,j);
}

bool isStopWord(string word){
    stopwords.contains(word);
    return stopwords.contains(word);
};

string stem(string w){//porter stemming algo
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

struct entry {string word; set<int> id;};

AvlTree <entry> revIndex;//places into AVL tree

bool operator<(const entry& first, const entry& second) {
    return first.word<second.word; //compares words will be index
}

int main(int argc, char** argv) {

    int number = 0;
    string word;


    //Code below used for parsing with Json
    for (auto ent: filesystem::recursive_directory_iterator("data")) {//finds all files
        number++;

        ifstream input(ent.path().c_str());//finds file path

        rapidjson::Document doc;

        string wholeFile;
        wholeFile = readFile(ent.path().c_str());
// input the whole .json into wholeFile
        doc.Parse(wholeFile.c_str());

        string text = doc["text"].GetString();//gets string value of hastable of text

        text = normalize(text);

        istrstream in(text.c_str());//treats string as input stream


        //Still need to balance tree
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
