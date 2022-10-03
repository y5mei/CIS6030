//
// Created by Yao-Wen Mei on 2022/10/3.
//

#ifndef QUESTION2_RECORD_H
#define QUESTION2_RECORD_H

#include <vector>
#include <string>

using namespace std;

class Record{
public:
    std::string field1;
    std::string field2;
    std::string field3;
    std::string content;

    explicit Record(const std::string& str);
    Record();
    friend ostream &operator<<(ostream &os, Record const &record) {
        string s;
        s.append("Field 1: ");
        s.append(record.field1+'\n');
        s.append("Field 2: ");
        s.append(record.field2+'\n');
        s.append("Field 3: ");
        s.append(record.field3+'\n');
        return os << s;
    }
private:
    short idxField2End;
    // returns the end idx of the second field (the space),
    // the start idx of the third field will be idx + 1
    // Example: a b c def, this method will return 4
    static int endOfField2(std::string str);
};

#endif //QUESTION2_RECORD_H
