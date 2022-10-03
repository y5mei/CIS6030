//
// Created by Yao-Wen Mei on 2022/10/3.
//

#include "Record.h"

int Record::endOfField2(std::string str) {
    int cnt = 3;
    for (int i = 0; i < str.size(); i++) {
        if (isspace(str[i])) {
            cnt -= 1;
            if (cnt == 2 && i != 9) {
                throw invalid_argument("The length of first field must be 9 bytes!");
            }
            if (cnt == 0) {
                return i - 1;
            }
        }
    }
    if (cnt == 1) return str.size() - 1;
    throw invalid_argument("Input str is invalid, you need at least 3 spaces in the str.");
}

Record::Record(const string& inputText) {
    if (inputText.size() < 11) {
        throw invalid_argument("Input Text must be longer than 10 bytes, with 9 Bytes as Field1, separate by space");
    }
    field1 = inputText.substr(0, 9);
    idxField2End = endOfField2(inputText);
    field2 = inputText.substr(10, idxField2End + 1 - 10);
    int length = inputText.size();
    field3 = inputText.substr(min(idxField2End + 2, length));
    content = inputText;
}

Record::Record() = default;