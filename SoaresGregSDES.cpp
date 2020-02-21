/*
Author: Greg Soares
Program - S-DES cipher
*/

#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <cassert>
#include <sstream>

using std::cout;
using std::endl;
using std::string;

int loc[2];
//Key Generation Function
string keyGen(string, int); //10 bit key in, 16 bit key out, first half = k1, Second half = k2

// Key generation operations:
string leftShift(string, int); //Left shift by specified round #
string perm10(string);         // Permutate, 10Bit in, 10Bit out
string perm8(string);          // Select&Permutate 10Bit in, 8Bit out

//Encryption Function
string sboxFunc(string);

//Encryption/Decryption Operations:
string initialPerm(string);
string inverseInitialPerm(string);
string perm4(string);
string expandPerm(string);
string switchBox(string);
int *boxLoc(string, string);
string exclusiveOR(string, string);

//Decryption Function

int main()
{
  string plainText = "01110010";
  string key = "1010000010";
  string cipherText = "";

  // Call testingFunc() to test the full process using asserts
  // for easier debugging.

  sboxFunc("00001111");

  return 0;
}

void testingFunc() {
  string plainText = "01110010";
  string key = "1010000010";
  string cipherText = "";

  sboxFunc("00001111");

  cout << "Plaintext: " << plainText << endl;

  // Testing all functions
  string key = "1234567890"; //Use this key to test Key generation+KeyGen Operations
  // Testing: Left Shift
  assert((leftShift(key, 1)) == "2345178906"); //Testing leftShift by 1
  assert((leftShift(key, 2)) == "3451289067"); //Testing leftShift by 2
  assert((leftShift(key, 3)) == "4512390678"); //Testing leftShift by 3
  assert((leftShift(key, 4)) == "5123406789"); //Testing leftShift by 4

  // Testing: P10 Operation
  assert((perm10(key) == "3527401986")); // Without leftShift

  // Testing: P8 Operation
  assert((perm8(key)) == "63748509"); // Without leftShift

  // Testing: keyGen() 1 Round
  assert((keyGen(key, 1)) == "17948306");
  // Testing: keyGen() 2 Round
  assert((keyGen(key, 2)) == "1794830683650291");
  // Testing: keyGen() 3 Round
  assert((keyGen(key, 3)) == "179483068365029117948306");
  // Testing: keyGen() 4 Round
  assert((keyGen(key, 4)) == "17948306836502911794830602179468");

  assert((exclusiveOR("000111", "001011")) == "001100");

  // Generate key using 2 round
  string keys = keyGen(key, 4);
  cout << "Keys: " << keys << endl << endl;

  // Cutting Keys
  string keyOne = keys.substr(0, 8);
  string keyFour = keys.substr(24, 32);
  string keyTwo = "", keyThree = "";

  // Building keyTwo and keyThree one by one because substr() is not working for them.
  keyTwo += keys.at(8);
  keyTwo += keys.at(9);
  keyTwo += keys.at(10);
  keyTwo += keys.at(11);
  keyTwo += keys.at(12);
  keyTwo += keys.at(13);
  keyTwo += keys.at(14);
  keyTwo += keys.at(15);

  keyThree += keys.at(16);
  keyThree += keys.at(17);
  keyThree += keys.at(18);
  keyThree += keys.at(19);
  keyThree += keys.at(20);
  keyThree += keys.at(21);
  keyThree += keys.at(22);
  keyThree += keys.at(23);

  cout << "keyOne: " << keyOne << endl;
  cout << "keyTwo: " << keyTwo << endl;
  cout << "keyThree: " << keyThree << endl;
  cout << "keyFour: " << keyFour << endl;

  cout << "Key-2: " << keys.at(8) << keys.at(9) << keys.at(10) << keys.at(11) << keys.at(12) << keys.at(13) << keys.at(14) << keys.at(15);

  // ------===== End of testing  ------===== 
}

string keyGen(string key, int round)
{
  string keys = "", shiftLeft = "";

  key = perm10(key); //P10 operation to initiate Key generation
  // cout << "P10: " << key << endl;

  //Generate keys and concatenate after each P8 operation
  for (int i = 0; i < round; i++)
  {
    if (i == 0)
    {
      shiftLeft = leftShift(key, i + 1);
    }
    else
    {
      shiftLeft = leftShift(shiftLeft, i + 1);
    }
    keys += perm8(shiftLeft);
  }

  return keys;
}

string leftShift(string key, int round)
{

  // Split key
  string leftHalf = key.substr(0, 5);
  string rightHalf = key.substr(5, 9);
  // Shift Left Half
  string tempKey = leftHalf.substr(round, (5 - round));
  tempKey += leftHalf.substr(0, round);
  leftHalf = tempKey;

  // Shift Right Half
  tempKey = rightHalf.substr(round, (5 - round));
  tempKey += rightHalf.substr(0, round);
  rightHalf = tempKey;

  return (leftHalf + rightHalf);
}

// P10 Operation
string perm10(string key)
{
  //Input Locations:  1 2 3 4 5 6 7 8 9 10
  //Output Locations: 3 5 2 7 4 10 1 9 8 6
  string newKey = "";
  newKey += key.at(2);
  newKey += key.at(4);
  newKey += key.at(1);
  newKey += key.at(6);
  newKey += key.at(3);
  newKey += key.at(9);
  newKey += key.at(0);
  newKey += key.at(8);
  newKey += key.at(7);
  newKey += key.at(5);

  return newKey;
}

//P8 Operation
string perm8(string key)
{
  //Input: 1 2 3 4 5 6 7 8 9 10
  //Output:6 3 7 4 8 5 10 9
  string newKey = "";
  newKey += key.at(5);
  newKey += key.at(2);
  newKey += key.at(6);
  newKey += key.at(3);
  newKey += key.at(7);
  newKey += key.at(4);
  newKey += key.at(9);
  newKey += key.at(8);

  return newKey;
}

string initialPerm(string plainText)
{
  string cipherText = "";
  //Input: 1 2 3 4 5 6 7 8
  //Output:2 6 3 1 4 8 5 7
  cipherText += plainText.at(1);
  cipherText += plainText.at(5);
  cipherText += plainText.at(2);
  cipherText += plainText.at(0);
  cipherText += plainText.at(3);
  cipherText += plainText.at(7);
  cipherText += plainText.at(4);
  cipherText += plainText.at(6);

  return cipherText;
}

string expandPerm(string plainText)
{
  string cipherText = "";

  //Input: 1 2 3 4
  //Output:4 1 2 3 2 3 4 1
  cipherText += plainText.at(3);
  cipherText += plainText.at(0);
  cipherText += plainText.at(1);
  cipherText += plainText.at(2);
  cipherText += plainText.at(1);
  cipherText += plainText.at(2);
  cipherText += plainText.at(3);
  cipherText += plainText.at(0);

  return cipherText;
}

string exclusiveOR(string a, string b)
{
  string result = "";

  //XOR Implementation -> if respective a=b elements
  // are the same then append 0, else append 1 to result variable
  for (int i = 0; i < a.size(); i++)
  {
    if ((a.at(i) == b.at(i)))
    {
      result += "0";
    }
    else
    {
      result += "1";
    }
  }

  return result;
}

string perm4(string plainText)
{
  string cipherText = "";

  //Input: 1 2 3 4
  //Output:2 4 3 1
  cipherText += plainText.at(1);
  cipherText += plainText.at(3);
  cipherText += plainText.at(2);
  cipherText += plainText.at(0);

  return cipherText;
}

//Swap Left half with right half
string switchBox(string input)
{
  string output = "";

  string leftInput = input.substr(0, 4);
  string rightInput = input.substr(4, 7);

  output = rightInput;
  output += leftInput;

  return output;
}

string inverseInitialPerm(string input)
{
  string output = "";

  return output;
}
string sboxFunc(string input)
{
  // Arrays: A,B,C,D  = row for S-Box-0
  string A[4] = {"01", "00", "11", "10"}; // Bit2+Bit3 = 00
  string B[4] = {"11", "10", "01", "00"}; // Bit2+Bit3 = 01
  string C[4] = {"00", "10", "01", "11"}; // Bit2+Bit3 = 10
  string D[4] = {"11", "01", "11", "10"}; // Bit2+Bit3 = 11

  // Arrays: E,F,G,H  = row for S-Box-1
  string E[4] = {"00", "01", "10", "11"}; // Bit2+Bit3 = 00
  string F[4] = {"10", "00", "01", "11"}; // Bit2+Bit3 = 01
  string G[4] = {"11", "00", "01", "00"}; // Bit2+Bit3 = 10
  string H[4] = {"10", "01", "00", "11"}; // Bit2+Bit3 = 11

  string output = "", row = "", col = "";
  string leftInput = input.substr(0, 4);  //S-BOX-0
  string rightInput = input.substr(4, 7); //S-BOX-1

  cout << "leftInput: " << leftInput << endl;
  cout << "rightInput: " << rightInput << endl;

  row += rightInput.at(0);
  row += rightInput.at(3);
  col += rightInput.at(1);
  col += rightInput.at(2);
  //Call boxLoc(row, col) and save output before next set of assignments

  cout << "row: " << row << endl;
  cout << "col: " << col << endl;
  int *boxZero = boxLoc(row, col);

  row = leftInput.at(0);
  row += leftInput.at(3);
  col = leftInput.at(1);
  col += leftInput.at(2);

  cout << "row: " << row << endl;
  cout << "col: " << col << endl;

  int *boxOne = boxLoc(row, col);

  cout << "boxZero[0]: " << boxZero[0] << endl;
  cout << "boxZero[1]: " << boxZero[1] << endl;
  if (boxZero[0] == 0)
  {
    output += A[boxZero[1]];
  }
  else if (boxZero[0] == 1)
  {
    output += B[boxZero[1]];
  }
  else if (boxZero[boxZero[1]] == 2)
  {
    output += C[boxZero[1]];
  }
  else if (boxZero[0] == 3)
  {
    output += D[boxZero[1]];
  }
  cout << "boxOne[0]: " << boxOne[0] << endl;
  cout << "boxOne[1]: " << boxOne[1] << endl;

  return output;
}

int *boxLoc(string row, string col)
{
  loc[0] = 0;
  loc[1] = 0;
  if (row == "00")
  {
    if (col == "00")
    {
      loc[0] = 0;
      loc[1] = 0;
      return loc;
    }
    else if (col == "01")
    {
      loc[0] = 0;
      loc[1] = 1;
      return loc;
    }
    else if (col == "10")
    {
      loc[0] = 0;
      loc[1] = 2;
      return loc;
    }
    else
    { //col == 11
      loc[0] = 0;
      loc[1] = 3;
      return loc;
    }
  }
  else if (row == "01")
  {
    if (col == "00")
    {
      loc[0] = 1;
      loc[1] = 0;
      return loc;
    }
    else if (col == "01")
    {
      loc[0] = 1;
      loc[1] = 1;
      return loc;
    }
    else if (col == "10")
    {
      loc[0] = 1;
      loc[1] = 2;
      return loc;
    }
    else
    { //col == 11
      loc[0] = 1;
      loc[1] = 3;
      return loc;
    }
  }
  else if (row == "10")
  {
    if (col == "00")
    {
      loc[0] = 2;
      loc[1] = 0;
      return loc;
    }
    else if (col == "01")
    {
      loc[0] = 2;
      loc[1] = 1;
      return loc;
    }
    else if (col == "10")
    {
      loc[0] = 2;
      loc[1] = 2;
      return loc;
    }
    else
    { //col == 11
      loc[0] = 2;
      loc[1] = 3;
      return loc;
    }
  }
  else
  { //row == 11
    if (col == "00")
    {
      loc[0] = 3;
      loc[1] = 0;
      return loc;
    }
    else if (col == "01")
    {
      loc[0] = 3;
      loc[1] = 1;
      return loc;
    }
    else if (col == "10")
    {

      loc[0] = 3;
      loc[1] = 2;
      return loc;
    }
    else
    { //col == 11
      loc[0] = 3;
      loc[1] = 3;
      cout << "33 here: " << loc[0] << loc[1] << endl; //Returning properly, but not saving on sboxFunc()
      return loc;
    }
  }
}