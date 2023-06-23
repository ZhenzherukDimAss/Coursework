#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <math.h>
#include <vector>

class Shooter;

class AbstractTarget
{
protected:
	int targetWidth, targetLength;
	std::pair<int, int>  centerX, centerY;
	std::vector<int> vecX;
	std::vector<int> vecY;
	std::string** matrix;
	std::string** matrixCopy;

	virtual void calulateCenter()
	{
		centerX = { targetLength / 2 - (targetLength % 2 == 0 ? 1 : 0), targetLength / 2 };
		centerY = { targetWidth / 2 - (targetWidth % 2 == 0 ? 1 : 0), targetWidth / 2 };
	}

	virtual void fillMatrix() {
		for (int i = 0; i < targetLength; i++)
		{
			for (int j = 0; j < targetWidth; j++)
			{
				matrix[i][j] = "0";
			}
		}

		for (int i = 0; i < targetLength; ++i) {
			for (int j = 0; j < targetWidth; ++j) {
				// Вычисление расстояния от каждой ячейки до ближайшего центра
				int dist = std::max(std::min(std::abs(centerX.first - i), std::abs(centerX.second - i)),
					std::min(std::abs(centerY.first - j), std::abs(centerY.second - j)));

				int value = 9 - dist;

				if (value < 1) value = 1;

				matrix[i][j] = std::to_string(value);
			}
		}
	}

public:
	AbstractTarget(int targetWidth, int targetLength) : targetWidth(targetWidth), targetLength(targetLength)
	{		
		matrix = new std::string* [targetLength];
		for (int i = 0; i < targetLength; i++)
		{
			matrix[i] = new std::string[targetWidth];
		}
		calulateCenter();
		fillMatrix();
		matrixCopy = nullptr;
	}	

	virtual void printmatrix()
	{
		for (int i = 0; i < targetWidth * 2 + 3; i++)
		{
			std::cout << "-";
		}
		std::cout << std::endl;
	
		for (int i = 0; i < targetLength; i++)
		{
			std::cout << "|";
			for (int j = 0; j < targetWidth; j++)
			{
				std::cout << std::setw(2) << matrix[i][j];
			}
			std::cout << " " << "|" << std::endl;
		}

		for (int i = 0; i < targetWidth * 2 + 3; i++)
		{
			std::cout << "-";
		}
		std::cout << std::endl << std::endl;
	}

	int getWidth() const {
		return targetWidth;
	}

	int getLength() const {
		return targetLength;
	}

		
	int shot(Shooter* shooter, bool isprint, int numbShots)
	{
		vecX.clear();
		vecY.clear();
		printmatrix();
		makeMatrixCopy();
		
		int result = 0;
					
		bool coordCheck = true;
		for (int i = 0; i < numbShots; i++)
		{
			auto [shotCoordX, shotCoordY] = shooter.inputCoordinates(this);

			auto [hit, points] = shot(shotCoordX, shotCoordY);
			if (!hit) continue;
			result += points;			

			if (isprint) printmatrix();
		}

		findMaxMinAverageRange();

		return result / numbShots;
	}

	std::pair<bool, int> shot(int shotCoordX, int shotCoordY)
	{
		if (shotCoordX < 0 || shotCoordY < 0)
		{
			std::cout << "Coordinates cannot be less than zero.";
			return { false, 0 };
		}
		if (shotCoordX == 0 || shotCoordY == 0)
		{
			return { false, 0 };
		}

		vecX.push_back(shotCoordX - 1);
		vecY.push_back(shotCoordY - 1);

		matrix[shotCoordX - 1][shotCoordY - 1] = "X";
		return { true, std::stoi(matrixCopy[shotCoordX - 1][shotCoordY - 1]) * 100 / 9 };
	}

	void makeMatrixCopy()
	{
		if (matrixCopy == nullptr)
		{
			matrixCopy = new std::string * [targetLength];
			for (int i = 0; i < targetLength; i++) {
				matrixCopy[i] = new std::string[targetWidth];
				for (int j = 0; j < targetWidth; j++) {
					matrixCopy[i][j] = matrix[i][j];
				}
			}
		}
	}	

	void findMaxMinAverageRange()
	{
		if (vecX.size() == 1) std::cout << "Not enough information to calculate range.\n";
		else
		{
			int rangeMax = std::numeric_limits<int>::min();
			int rangeMin = std::numeric_limits<int>::max();
			int rangeSum = 0;

			for (int i = 0; i < vecX.size() - 1; i++) {
				int distance = static_cast<int>(std::sqrt(std::pow(vecX[i] - vecX[i + 1], 2) + std::pow(vecY[i] - vecY[i + 1], 2)));

				rangeMax = std::max(rangeMax, distance);
				rangeMin = std::min(rangeMin, distance);

				rangeSum += distance;
			}

			int averageDistance = rangeSum / ((int)vecX.size() - 1);

			std::cout << "Maximum shot range: " << rangeMax << std::endl;
			std::cout << "Minimum shot range: " << rangeMin << std::endl;
			std::cout << "Average shot range: " << averageDistance << std::endl;
		}				
	}

	void destroy()
	{
		destroyCopy();
		for (int i = 0; i < targetLength; i++) {
			delete[] matrix[i];
		}
		delete[] matrix;
	}

	void destroyCopy()
	{
		if (matrixCopy != nullptr)
		{
			for (int i = 0; i < targetLength; i++) {
				delete[] matrixCopy[i];
			}
			delete[] matrixCopy;
			matrixCopy = nullptr;
		}
	}
	virtual ~AbstractTarget() { destroy(); }
};

class RoundTarget : public AbstractTarget
{
private:
	int diameter;

protected:
	void fillMatrix() override
	{
		double center = (diameter - 1) / 2.0;
		double radius = diameter % 2 == 0 ? diameter / 2 : diameter / 2 - 0.5;

		for (int i = 0; i < diameter; ++i) {
			for (int j = 0; j < diameter; ++j) {
				double dist = std::sqrt(std::pow(i - center, 2) + std::pow(j - center, 2));
				int distInt = static_cast<int>(dist + 0.5);
				int value;
				if (distInt <= radius) {
					if (diameter % 2 == 0) value = 10 - distInt;
					else value = 9 - distInt;
					if (value < 1) value = 1;
					matrix[i][j] = std::to_string(value);
				}
				else {
					matrix[i][j] = "0";
				}
			}
		}
	}
public:
	RoundTarget(int diameter) : AbstractTarget(diameter, diameter), diameter(diameter) { fillMatrix(); }
};

class HumanTarget : public AbstractTarget
{
public:
	HumanTarget() : AbstractTarget(30, 21)
	{
		std::string target[] = {
			"....,,,......;77*,.....,,,....",
			"..:;;;;;:,..?7887#:..:;;;;;:,.",
			".;;;?%?;;;.:789987?.:;;?%%+;;,",
			".;;;%%S+;;.,789987+.;;;%%S*;;,",
			".,;;;+;;;,..:7887*..,:;;+;;;:.",
			"...,,,,....,,6776;,....,,,,...",
			"......:11123456654321111,.....",
			".....11111234566543211111:....",
			"....1111123456776543211111,...",
			"...:!1112345678876543211111...",
			"...111123456789987654321111:..",
			"..,11111:3456788765432111111..",
			"..:1111..234567765432:.:1111..",
			"..+111;..;23456654321...1111..",
			"..;111:..+12345543211...1111..",
			"...111...111234432111:..111;..",
			"...;11:.,1111233211111.,111...",
			"...;111;;1111122111111,1111...",
			"...,1111111111111111111111;...",
			"....,11111111111111111111:....",
			"...,,.,,11111111111111;,.,,..."
		};

		for (int i = 0; i < targetLength; i++)
		{
			for (int j = 0; j < targetWidth; j++)
			{
				matrix[i][j] = std::string(1, target[i][j]);
			}
		}
	}

	void printmatrix() override
	{
		for (int i = 0; i < targetWidth + 2; i++)
		{
			std::cout << "-";
		}
		std::cout << std::endl;

		for (int i = 0; i < targetLength; i++)
		{
			std::cout << "|";
			for (int j = 0; j < targetWidth; j++)
			{
				std::cout  << matrix[i][j];
			}
			std::cout << "|" << std::endl;
		}

		for (int i = 0; i < targetWidth + 2; i++)
		{
			std::cout << "-";
		}
		std::cout << std::endl << std::endl;
	}
};
