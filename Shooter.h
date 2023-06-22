#pragma once

//#include "Target.h"

class Target;

class Shooter {
public:
    int callShot(AbstractTarget* target) {
        std::cout << "Do you want to print the target after every shot? [Y/N]\n";

        bool YN_check = true;
        char isprint;
        do {
            std::cout << "=> ";
            std::cin >> isprint;
            if (isprint == 'Y')
            {
                YN_check = false;
            }
            else if (isprint == 'N')  YN_check = false;
            else {
                std::cout << "Y - Yes, N - No\n";
            }
        } while (YN_check);

        std::cout << "How many times did you shoot in total?\n";
        std::cout << "=> ";
        int numbShots;
        std::cin >> numbShots;

        return target->shot(this, isprint == 'Y', numbShots);
    }
    std::pair<int, int> inputCoordinates(const AbstractTarget* target)
    {
        int shotCoordX, shotCoordY;
        bool coordCheck = true;

        while (coordCheck) {
            std::cout << "Write the coordinates of your shot (if you miss, then enter 0): \n";
            std::cout << "X: ";
            std::cin >> shotCoordY;
            std::cout << "Y: ";
            std::cin >> shotCoordX;

            if (shotCoordX > target->getLength() || shotCoordY > target->getWidth()) {
                std::cout << "There are no such coordinates. Enter valid coordinates.\n";
            }
            else if (shotCoordX < 0 || shotCoordY < 0) {
                std::cout << "Coordinates cannot be less than zero. Enter valid coordinates.\n";
            }
            else {
                coordCheck = false;
            }
        }

        return { shotCoordX, shotCoordY };
    }
};