#include "Target.h"
#include "Shooter.h"

#include <iostream>

using namespace std;

bool validity_check1 (int targetWidth, int targetLength)
{
    if (targetWidth <= 0 || targetLength <= 0)
    {
        cout << "Error. Dimensions cannot be less than or equal to zero. Please enter valid values:\n";
        return true;
    }
    return false;
}
bool validity_check2(int diameter)
{
    if (diameter <= 0)
    {
        cout << "Error. Diameter cannot be less than or equal to zero. Please enter valid values:\n";
        return true;
    }
    return false;
}

int main()
{
    int result;
    bool cycle = true, valid_check;
    AbstractTarget* abstrTarg;
    Shooter shooter;
    do {
        cout << "Choose the target you will shoot at:\n1 - Abstract target; \n2 - Round target; \n3 - A target in the form of a human figure; \n0 - Close program.\n";
        int choice;
        cout << "=> ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "You have selected an abstract target, set its dimensions (first the width, then the length):\n";
            
            int targetWidth, targetLength;
            do {
                cout << "=> ";
                cin >> targetWidth;
                cout << "=> ";
                cin >> targetLength;
                valid_check = validity_check1(targetWidth, targetLength);
            } while (valid_check);

            abstrTarg = new AbstractTarget(targetWidth, targetLength);                    

            result = shooter.callShot(abstrTarg);

            cout << "Your score: " << result << "%" << endl << endl;

            delete abstrTarg;

            break;

        case 2: 
            cout << "You have selected an round target, set its diameter:\n";

            int diameter;
            do {
                cout << "=> ";
                cin >> diameter;
                valid_check = validity_check2(diameter);
            } while (valid_check);

            abstrTarg =  new RoundTarget(diameter);

            result = shooter.callShot(abstrTarg);

            cout << "Your score: " << result << "%" << endl << endl;

            delete abstrTarg;
            break;

        case 3: 
            cout << "You have selected an human target.\n";

            abstrTarg = new HumanTarget();

            result = shooter.callShot(abstrTarg);

            cout << "Your score: " << result << "%" << endl << endl;

            delete abstrTarg;
            break;

        case 0:
            cycle = false;
            cout << "Closing the program...";
            return 1;
            break;
        }
    } while (cycle);

    return 0;
}