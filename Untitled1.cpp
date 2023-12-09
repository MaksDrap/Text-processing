#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;

bool isVowel(char c) {
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y');
}

bool isSpecial(char c) {
    return !isalpha(c) && !isdigit(c) && !isspace(c);
}

vector<pair<char, int>>::iterator findPair(vector<pair<char, int>>& vec, char key) {
    return find_if(vec.begin(), vec.end(), [&key](pair<char, int>& element) -> bool {
        return element.first == key;
    });
}

void countLetters(const string& text, vector<pair<char, int>>& counts) {
    for (size_t i = 0; i < text.length(); ++i) {
        char c = text[i];
        if (isalpha(c)) {
            char lowercaseC = tolower(c);
            auto it = findPair(counts, lowercaseC);

            if (it != counts.end()) {
                it->second++;
            } else {
                counts.push_back(make_pair(lowercaseC, 1));
            }
        }
    }
}

void countCharacters(const string& text, vector<pair<char, int>>& counts) {
    for (size_t i = 0; i < text.length(); ++i) {
        char c = text[i];
        auto it = findPair(counts, c);

        if (it != counts.end()) {
            it->second++;
        } else {
            counts.push_back(make_pair(c, 1));
        }
    }
}

struct ComparePairs {
    bool operator()(const pair<char, int>& a, const pair<char, int>& b) const {
        return a.second > b.second;
    }
};

void printSortedSpecialChars(const vector<pair<char, int>>& counts) {
    for (size_t i = 0; i < counts.size(); ++i) {
        char c = counts[i].first;
        if (isSpecial(c)) {
            cout << "'" << c << "': " << counts[i].second << '\n';
        }
    }
}

void printStartingCharacters(const string& text) {
    map<char, int> startingCharCounts;

    stringstream ss(text);
    string word;

    while (ss >> word) {
        if (!word.empty() && !isSpecial(word[0])) { // Check if the first character is not special
            char startingChar = tolower(word[0]);
            startingCharCounts[startingChar]++;
        }
    }

    vector<pair<char, int>> sortedChars;
    for (auto& pair : startingCharCounts) {
        sortedChars.push_back(pair);
    }

    sort(sortedChars.begin(), sortedChars.end(),
         [](const pair<char, int>& a, const pair<char, int>& b) {
             return a.second > b.second;
         });

    cout << "Starting Characters of Words and their count:\n";
    for (const auto& pair : sortedChars) {
        cout << "'" << pair.first << "': " << pair.second << '\n';
    }
}

void printStartingWord(const string& text) {
    vector<pair<string, int>> counts;

    stringstream ss(text);
    string word;

    while (ss >> word) {
        if (!word.empty() && !isSpecial(word[0])) { // Check if the first character is not special
            char startingChar = tolower(word[0]);
            auto it = find_if(counts.begin(), counts.end(),
                              [&startingChar](const pair<string, int>& element) {
                                  return element.first[0] == startingChar;
                              });

            if (it != counts.end()) {
                it->second++;
            } else {
                counts.push_back(make_pair(word, 1));
            }
        }
    }

    sort(counts.begin(), counts.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second;
         });

    cout << "Starting Characters of Words and their count:\n";
    for (const auto& pair : counts) {
        cout << "'" << pair.first << "': " << pair.second << '\n';
    }
}

void encryptAndDecrypt(const string& text) {
  // Encryption
  srand(static_cast<unsigned int>(time(0)));

  map<string, int> encodingTable;
  map<int, string> reverseEncodingTable; // Declare the reverseEncodingTable variable
  stringstream encryptedText;
  stringstream ss(text);
  string word;



  // Write the encrypted text to a file
  ofstream outFile("encrypted_text.txt");
  while (ss >> word) {
    int encodedValue = rand(); // Use a random number as an encoded value
    encodingTable[word] = encodedValue;
    reverseEncodingTable[encodedValue] = word; // Store the decoded word in the reverse encoding table
    encryptedText << encodedValue << " ";
    outFile << encodedValue << " ";
  }
  outFile.close();

  cout << "\n\n";

  // Decryption
  ss.clear();
  ss.str(encryptedText.str());

  cout << "Decrypted text:\n";

  // Read the encrypted text from the file
  ifstream inFile("encrypted_text.txt");
  while (inFile >> word) {
    try {
      int encodedValue = stoi(word);
      // Look up the encoded value in the reverse encoding table
      auto it = reverseEncodingTable.find(encodedValue);
      if (it != reverseEncodingTable.end()) {
        cout << it->second << " ";
      } else {
        // Handle case where encoded value is not found
        cout << "[Not Found] ";
      }
    } catch (invalid_argument&) {
      // Ignore non-integer values
    }
  }
  inFile.close();

  cout << "\n\n";

  // Decoding Table
  cout << "Decoding Table:\n";
  for (const auto& pair : reverseEncodingTable) {
    cout << "Encoded: " << pair.first << " -> Decoded: " << pair.second << '\n';
  }
}

void compressAndDecompressWords(const string& text) {
    map<string, string> encodingTable;
    map<string, string> decodingTable;
    stringstream compressedText;
    stringstream ss(text);
    string word;

    map<string, int> wordOccurrences;
    while (ss >> word) {
        if (word.length() >= 5) {
            wordOccurrences[word]++;
        }
    }

    for (const auto& pair : wordOccurrences) {
        if (pair.second > 2) {
            string encodedValue = "#" + to_string(rand()) + "#";
            encodingTable[pair.first] = encodedValue;
            decodingTable[encodedValue] = pair.first;
        }
    }

    ss.clear();
    ss.str(text);
    while (ss >> word) {
        if (word.length() >= 5 && encodingTable.count(word) > 0) {
            compressedText << encodingTable[word] << " ";
        } else {
            compressedText << word << " ";
        }
    }

    ofstream outFile("compressed_words_text.txt");
    outFile << compressedText.str();
    outFile.close();

    cout << "Compressed text with words:\n";
    cout << compressedText.str() << "\n\n";

    ifstream inFile("compressed_words_text.txt");
    stringstream decompressedText;
    while (inFile >> word) {
        try {
            string decodedValue = decodingTable[word];
            decompressedText << decodedValue << " ";
        } catch (const out_of_range&) {
            decompressedText << word << " ";
        }
    }
    inFile.close();

    cout << "Decompressed text with words if words has more than 5 letters and more 3 time in text:\n";
    cout << decompressedText.str() << "\n\n";

    cout << "Decoding Table:\n";
    for (const auto& pair : decodingTable) {
        cout << "Encoded: " << pair.second << " -> Decoded: " << pair.first << '\n';
    }
}


int convert_roman_numeral(const vector<char>& digits) {
  // Create a copy of the romans map
    const map<char, int> romans = {
    {'I', 1},
    {'V', 5},
    {'X', 10},
    {'L', 50},
    {'C', 100},
    {'D', 500},
    {'M', 1000}
  };
  map<char, int> romansCopy = romans;

  // Check if the input is a valid roman numeral
  for (char digit : digits) {
    if (romansCopy.find(digit) == romansCopy.end()) {
      return -1;
    }
  }

  // Convert roman numeral to arabic numeral
  int result = 0;
  for (int i = digits.size() - 1; i >= 0; i--) {
    if (i > 0 && romansCopy[digits[i]] > romansCopy[digits[i - 1]]) {
      result -= romansCopy[digits[i - 1]];
    }
    result += romansCopy[digits[i]];
  }

  // Print the arabic numeral
  cout << "Арабські цифри: " << result << endl;
    cout << "Римські цифри: ";
  for (char digit : digits) {
    cout << digit;
  }
  cout << endl;

  return result;
}

int main() {
    string text;
    cout << "Enter the text: ";
    getline(cin, text);

    vector<pair<char, int>> letterCounts;
    vector<pair<char, int>> charCounts;

    countLetters(text, letterCounts);
    countCharacters(text, charCounts);

    cout << "Vowel letters and their count:\n";
    for (size_t i = 0; i < letterCounts.size(); ++i) {
        if (isVowel(letterCounts[i].first)) {
            cout << letterCounts[i].first << ": " << letterCounts[i].second << " (Vowel)\n";
        }
    }

    cout << "Consonant letters and their count:\n";
    for (size_t i = 0; i < letterCounts.size(); ++i) {
        if (!isVowel(letterCounts[i].first)) {
            cout << letterCounts[i].first << ": " << letterCounts[i].second << " (Consonant)\n";
        }
    }

    sort(charCounts.begin(), charCounts.end(), ComparePairs());

    cout << "Special Characters and their count (sorted by count in descending order):\n";
    printSortedSpecialChars(charCounts);

    printStartingCharacters(text);
    printStartingWord(text);
    encryptAndDecrypt(text);
    compressAndDecompressWords(text);

vector<char> digits = {'M', 'D', 'C', 'L', 'X', 'V', 'I'};

  int number = convert_roman_numeral(digits);

  if (number == -1) {
    cout << "Неправильний запис римських цифр" << endl;
  }

    return 0;
}

