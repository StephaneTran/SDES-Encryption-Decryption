/* Contains functions for S-DES encryption and decryption.
** Author: Stephane Vien Tran
** Student Number: 19062639
*/

#ifndef S_DES_H
#define S_DES_H

#include <string>
#include <vector>

using namespace std;

/* NOTE
** "TESTING PURPOSES ONLY" are for code that check functionality (checks the calculations involved).
** Remove "//" at the start of the line to check each function.
*/

class S_DES {
public:
    string K1, K2, KEY, PT, key, leftEXOR, CT;

//Initialising the 10-bit key
    void declareKey()
    {
        cout << "Enter the 10-bit key: ";
        cin >> key;
        while(key.size() != 10) {
            cout << "Error: Invalid key length of " << key.size() << "!" << " It must be 10 bits.\nPlease re-enter the 10-bit key: ";
            cin >> key;
        }
        KEY = key;
        generateKeys();
    }//end of declareKey()

//Generating K1 and K2
    void generateKeys()
    {
        string P10_output = P10(KEY);
        //cout << "P10 output is: " << P10_output << endl; //TESTING PURPOSES ONLY

        string P10_left = P10_output.substr(0,5);
        string P10_right = P10_output.substr(5,5);
        string LS_1_left = leftShift(P10_left, 1);
        string LS_1_right = leftShift(P10_right, 1);
        //cout << "After LS-1: " << LS_1_left << "|" << LS_1_right << endl; //TESTING PURPOSES ONLY

        string LS_1_output = LS_1_left + LS_1_right;
        K1 = P8(LS_1_output);
        //cout << "K1 is: " << K1 << endl;    //TESTING PURPOSES ONLY

        string LS_2_left = leftShift(LS_1_left, 2);
        string LS_2_right = leftShift(LS_1_right, 2);
        //cout << "After LS-2: " << LS_2_left << "|" << LS_2_right << endl; //TESTING PURPOSES ONLY

        string LS_2_output = LS_2_left + LS_2_right;
        K2 = P8(LS_2_output);
        //cout << "K2 is: " << K2 << endl;    //TESTING PURPOSES ONLY
    }//end of generateKeys()

//Encrypting 8-bit plaintext
    string encryption()
    {
        cout << "Enter 8-bit plaintext to encrypt: ";
        cin >> PT;
        while (PT.size() != 8) {
            cout << "Error: Invalid plaintext length of " << PT.size() << "!" << " It must be 8 bits.\nPlease re-enter the 8-bit plaintext: ";
            cin >> PT;
        }

        string IP_output = IP(PT);
        string IP_left = IP_output.substr(0,4);
        string IP_right = IP_output.substr(4,4);
        //cout << "IP output is: " << IP_left << "|" << IP_right << endl;  //TESTING PURPOSES ONLY

        leftEXOR = EXOR(IP_left, fBox(IP_right, K1));   //apply EXOR on left half and fbox of right half
        //cout << "First cycle: " << leftEXOR << "\n";    //TESTING PURPOSES ONLY

        //swap sides (SW)
        string temp;
        temp = leftEXOR;
        leftEXOR = IP_right;
        IP_right = temp;

        leftEXOR = EXOR(leftEXOR, fBox(IP_right, K2));  //apply EXOR on left half and fbox of right half
        //cout << "Second cycle: " << leftEXOR << "\n";   //TESTING PURPOSES ONLY

        //inverse IP to get output
        string output = leftEXOR + IP_right;
        string InvIP_output = InvIP(output);

        return InvIP_output;
    }//end of encryption()

//Decrypting 8-bit ciphertext. Exactly the same as encryption() EXCEPT K2 is used first in EXOR() and K1 is used second.
    string decryption()
    {
        cout << "Enter 8-bit ciphertext to decrypt: ";
        cin >> CT;
        while (CT.size() != 8) {
            cout << "Error: Invalid ciphertext length of " << CT.size() << "!" << " It must be 8 bits.\nPlease re-enter the 8-bit ciphertext: ";
            cin >> CT;
        }

        string IP_output = IP(CT);
        string IP_left = IP_output.substr(0,4);
        string IP_right = IP_output.substr(4,4);
        //cout << "IP output is: " << IP_left << "|" << IP_right << endl;  //TESTING PURPOSES ONLY

        leftEXOR = EXOR(IP_left, fBox(IP_right, K2));   //apply EXOR on left half and fbox of right half
        //cout << "First cycle: " << leftEXOR << "\n";    //TESTING PURPOSES ONLY

        //swap sides (SW)
        string temp;
        temp = leftEXOR;
        leftEXOR = IP_right;
        IP_right = temp;

        leftEXOR = EXOR(leftEXOR, fBox(IP_right, K1));  //apply EXOR on left half and fbox of right half
        //cout << "Second cycle: " << leftEXOR << "\n";   //TESTING PURPOSES ONLY

        //inverse IP to get output
        string output = leftEXOR + IP_right;
        string InvIP_output = InvIP(output);

        return InvIP_output;

    }

// ------------------------------ FUNCTIONS FOR generateKeys() ------------------------------
//P10 function
    string P10(string input)
    {
        int P10_perm[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
        string output = "";
        for (int i = 0; i < 10; ++i)
        output += input[P10_perm[i]-1];
        return output;
    }//end of P10()

//P8 function == K1
    string P8(string input)
    {
        int P8_perm[8] = {6, 3, 7, 4, 8, 5, 10, 9};
        string output = "";
        for (int i = 0; i < 8; ++i)
            output += input[P8_perm[i]-1];
        return output;
    }//end of P8()

//LS-1 function
    string leftShift(string input, int n)
    {
        string output = input;
        char fbit;
        while (n--) {
            fbit = output[0];
            output = output.substr(1, output.size()-1);
            output += fbit;
        }
        return output;
    }//end of LS-1()

// ------------------------------ FUNCTIONS FOR encryption() & decryption() ------------------------------
//IP function
    string IP(string input)
    {
        int IP_perm[8] = {2, 6, 3, 1, 4, 8, 5, 7};
        string output = "";
        for (int i = 0; i < 8; ++i)
            output += input[IP_perm[i] - 1];
        return output;
    }// end of IP()

//fBox function
    string fBox(string input, string k)
    {
        string EP_output = EP(input);
        //cout << "EP output is: " << EP_output << endl; //TESTING PURPOSES ONLY

        string firstEXOR_output = EXOR(EP_output, k);
        string firstEXOR_left = firstEXOR_output.substr(0,4);
        string firstEXOR_right = firstEXOR_output.substr(4,4);
        //cout << "First EXOR output is: " << firstEXOR_left << "|" << firstEXOR_right << endl; //TESTING PURPOSES ONLY

        string s0_output = sBox(firstEXOR_left, 0); //first 4 bits goes to s0
        string s1_output = sBox(firstEXOR_right, 1); //last 4 bits goes to s1
        //cout << "s0: " << s0_output << "\ns1: " << s1_output << endl; //TESTING PURPOSES ONLY

        string temp = s0_output + s1_output;
        string P4_output = P4(temp);
        //cout << "P4 output is: " << P4_output << endl;  //TESTING PURPOSES ONLY

        return P4_output;
    }//end of fBox()

//EP function
    string EP(string input)
    {
        int EP_perm[8] = {4, 1, 2, 3, 2, 3, 4, 1};
        string output = "";
        for (int i= 0; i < 8; ++i)
            output += input[EP_perm[i] - 1];
        return output;
    }// end of EP()

//EXOR function
    string EXOR(string a, string b)
    {
        string output = "";
        for (int i = 0; i < a.size(); ++i) {
            if(a[i] == b[i])
                output += "0";
            else
                output += "1";
        }
        return output;
    }//end of EXOR()

// convert binary to decimal
    int BinToDec(string input)
    {
        int output = 0, pow = 1;
        for (int i = input.length() - 1; i >= 0; --i, pow <<= 1)
            output += (input[i] - '0') * pow;
        return output;
    }//end of BinToDec()

//sBox function
    string sBox(string input, int s_box_number)
    {
        string output = "";

        vector<vector<string>> createBox;
        //SBox0 definition
        if(s_box_number == 0)
        {
            vector<string> r;
            r.push_back("01");
            r.push_back("00");
            r.push_back("11");
            r.push_back("10");
            createBox.push_back(r);

            r.clear();
            r.push_back("11");
            r.push_back("10");
            r.push_back("01");
            r.push_back("00");
            createBox.push_back(r);

            r.clear();
            r.push_back("00");
            r.push_back("10");
            r.push_back("01");
            r.push_back("11");
            createBox.push_back(r);

            r.clear();
            r.push_back("11");
            r.push_back("01");
            r.push_back("11");
            r.push_back("10");
            createBox.push_back(r);
        } else if(s_box_number == 1) {
            //SBox1 definition
            std::vector<std::string> r;

            r.push_back("00");
            r.push_back("01");
            r.push_back("10");
            r.push_back("11");
            createBox.push_back(r);

            r.clear();
            r.push_back("10");
            r.push_back("00");
            r.push_back("01");
            r.push_back("11");
            createBox.push_back(r);

            r.clear();
            r.push_back("11");
            r.push_back("00");
            r.push_back("01");
            r.push_back("00");
            createBox.push_back(r);

            r.clear();
            r.push_back("10");
            r.push_back("01");
            r.push_back("00");
            r.push_back("10");
            createBox.push_back(r);
        }

        //Using Bit-2 and Bit-3 for column
        string col("");
        col += input[1];
        col += input[2];
        int col_bin = BinToDec(col);

        //Using Bit-1 and Bit-4 for row
        string row("");
        row += input[0];
        row += input[3];
        int row_bin = BinToDec(row);

        //Return element from the sbox
        return createBox[row_bin][col_bin];
    }//end of sBox()

//P4 function
    string P4(string input)
    {
        int P4_perm[4] = {2, 4, 3, 1};
        string output = "";
        for (int i = 0; i < 4; ++i)
            output += input[P4_perm[i]-1];
        return output;
    }//end of P4()

//InvIP function
    string InvIP(string input)
    {
        int InvIP_perm[8] = {4, 1, 3, 5, 7, 2, 8, 6};
        string output = "";
        for (int i = 0; i < 8; ++i)
            output += input[InvIP_perm[i] - 1];
        return output;
    }// end of InvIP()

};

#endif // S_DES_H
