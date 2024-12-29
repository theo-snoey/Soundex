/*
OVERVIEW HEADER COMMENT:
 * Code Author: Theo Snoey
 * Soundex Algorithm on a large scale aims to take names and outputs soundex code along with
 * other names in data files with same soundex code.
 */
#include <cctype>
#include <fstream>
#include <string>
#include "console.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include "SimpleTest.h"
#include "soundex.h"
#include "strlib.h"
using namespace std;


/* This function is intended to return a string which
 * includes only the letter characters from the original
 * (all non-letter characters are excluded)
 *
 *
 *
 * I believe the bug I fixed at the time I implemented this function was
 * an off by one error. I think I changed the index to start at i = 0 to account for edge cases.
 * If the non alphabetical character was in the first index of the string, it wasn't being taken out.
 * By changing the index to start at zero, it would start looking at the first spot.
 */
string lettersOnly(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}


/* this Function, findDigits, takes an input of a string with only letters
    and returns the equivalent code, unprocessed, by mapping each string to
    its equivalent integer code.
*/
string findDigits(string s) {

    string letters = toUpperCase(s);
    string result = "";

    for (int i = 0; i < letters.length(); i++){
        if (stringContains("AEIOUHWY", letters[i])){
            result += "0";
        }
        if (stringContains("BFPV", letters[i])){
            result += "1";
        }
        if (stringContains("CGJKQSXZ", letters[i])){
            result += "2";
        }
        if (stringContains("DT", letters[i])){
            result += "3";
        }
        if (stringContains("L", letters[i])){
            result += "4";
        }
        if (stringContains("MN", letters[i])){
            result += "5";
        }
        if (stringContains("R", letters[i])){
            result += "6";
        }
    }
    return result;
}


/* This function, processCode,
 * takes the original name in string form, and the unprocessed code where each char in the string name
 * has been mapped to a digit.
 * The function then takes the code and eliminates redundant digits,
 * then replaces the first digit of the code with the first letter in uppercase form of the original name.
 * Then it eliminates the zeros from the code
 * And adds zeros or concatenates the string to make it length 4 and returns the string length 4.
 */

string processCode(string s, string longcode){

    // cut out repeats from longcode to create a new, non-redundant, shortcode
    string shortcode = longcode.substr(0,1);

    for (int i = 1; i < longcode.length(); i++){
        if (longcode[i] != longcode[i-1]){
            shortcode += longcode[i];
        }
    }

    // replace first digit of shortcode with first letter of name
    shortcode[0] = toUpperCase(s[0]);

    // discard zeros in shortcode
    string nozeros = "";
    nozeros += shortcode[0];

    for (int i = 1; i < shortcode.length(); i++){
        if (shortcode[i] != '0'){
            nozeros += shortcode[i];
        }
    }


    // return in case of final has size 4
    if (nozeros.length() == 4){
        return nozeros;
    }


    // final string is larger than size 4
    if (nozeros.length() > 4){
        return nozeros.substr(0,4);
    }


    // final string smaller than size 4
    int add = 4 - nozeros.length();

    string addedzeros = "";
    for (int i = 1; i <= add; i++){
        addedzeros += "0";
    }

    return nozeros + addedzeros;

}




/* This function, soundex, takes a string as an argument and
 * strips it down to its alphabetical characters, converts it to
 * digits, and then returns the final processed code.
 */
string soundex(string s) {
    string letters = lettersOnly(s);
    string digits = findDigits(letters);
    return processCode(letters, digits);
}


/* This function soundexSearch, takes an argument filepath and
 * creates a vector with the data from that file. It then prompts the user
 * to input a surname and uses the soundex algorithm to return the soundex code
 * and a vector of names with the same code. It loops through this process until prompted
 * by the user that it is finished.
 */
void soundexSearch(string filepath) {
    // This provided code opens the specified file
    // and reads the lines into a vector of strings
    ifstream in;
    Vector<string> allNames;

    if (openFile(in, filepath)) {
        allNames = readLines(in);
    }
    cout << "Read file " << filepath << ", "
         << allNames.size() << " names found." << endl;

    //Loop through this name and soundex code retrieving process until boolean mainlooper is set to false
    bool mainlooper = true;
    while (mainlooper){
        // The names read from file are now stored in Vector allNames
        cout << "" << endl;
        string line = getLine("Enter a surname (RETURN to quit): ");
        if (line == ""){
            mainlooper = false;
        }

        // using the all names vector, searches for names with matching codes
        // and creates a new vector, matchingCodes with those names.
        else {
            string namecode = soundex(line);

            Vector<string> matchingCodes;

            for (int i = 0; i < allNames.size(); i++){
                if (soundex(allNames[i]) == namecode){
                    matchingCodes.add(allNames[i]);
                }
            }

            sort(matchingCodes.begin(), matchingCodes.end());

            // output my soundex code and matching names on their respective lines
            cout << "Soundex code is " << namecode << endl;
            cout << "Matches from database: " << matchingCodes << endl;

        }
    }
}



/* * * * * * Test Cases * * * * * */

// Student Tests

STUDENT_TEST("Test findDigits function given different inputs of strings containing only letters"){
    EXPECT_EQUAL(soundex("Amy"), "A500");
    EXPECT_EQUAL(soundex("Josh-Michael"), "J252");
}


STUDENT_TEST("Test findDigits function given different inputs of strings containing only letters"){
    EXPECT_EQUAL(processCode("Curie", "20600"), "C600");
}

STUDENT_TEST("Test findDigits function given different inputs of strings containing only letters"){
    EXPECT_EQUAL(processCode("OConner","0205506"), "O256");
}

STUDENT_TEST("Test findDigits function given different inputs of strings containing only letters"){
    EXPECT_EQUAL(findDigits("Curie"), "20600");
    EXPECT_EQUAL(findDigits("OConner"), "0205506");
}

STUDENT_TEST("Test extraction of letters from a string in lettersOnly function") {
    EXPECT_EQUAL(lettersOnly("!Jean-Christophe"), "JeanChristophe");
    EXPECT_EQUAL(lettersOnly("984!$%"), "");
    EXPECT_EQUAL(lettersOnly(""), "");
}

// Provided Tests

PROVIDED_TEST("Test exclude of punctuation, digits, and spaces") {
    string s = "O'Hara";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "tldr");
}

PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

