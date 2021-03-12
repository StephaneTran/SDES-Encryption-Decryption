/* Contains menu of program.
** Author: Stephane Vien Tran
** Student Number: 19062639
hello
*/

#include <iostream>
#include "S_DES.h"

using namespace std;

int main()
{
    S_DES a;
    int choice = 0;
    while (choice != 4)
    {
        cout << "1. Encryption\n2. Decryption\n3. Help\n4. Exit\nEnter your choice: ";
        cin >> choice;
            if(choice == 1)
            {
                a.declareKey();
                cout << "Ciphertext (C): " << a.encryption() << "\n" << endl;
            }
            else if(choice == 2)
            {
                a.declareKey();
                cout << "Plaintext (P): " << a.decryption() << "\n" << endl;
            }
            else if(choice == 3)
            {
                cout << "This program uses S-DES to encrypt an 8-bit plaintext and decrypt an 8-bit ciphertext.\n" <<
                "Encryption takes two inputs; the 10-bit key and the 8-bit plaintext, and returns the encrypted 8-bit ciphertext as an output.\n" <<
                "Decryption takes two inputs; the 10-bit key and the 8-bit ciphertext, and returns the decrypted 8-bit plaintext as an output.\n" << endl;
            }
            else if(choice > 4)
            {
                cout << "Invalid choice. Please choose between 1 - 4.\n";
            }
    }
    cout << "Program has been terminated. . . Goodbye!" << endl;
}
