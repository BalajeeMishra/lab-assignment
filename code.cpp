#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <Windows.h>
#include <bits/stdc++.h>
#include <cctype>
#include<locale>
using namespace std;

char to_upper(char c)
{
    if (c>='a' && c<='z')
    return (c-32);
    return c;
}

class Task
{
public:
	string name;
	float duration;
	int numberOfDependencies = 0;
	string *Dependencies;
	float ES = -1, EF = -1, LS = -1, LF = -1;

public:
	//Task();
	void inputData(int numberOfTasks);
	void calculateEarlyStartAndFinish(int numberOFtask, Task *arrTasks);
	void initTerminalTasks(float maxEF, int numberOfTasks, Task *arrTasks);
	void calculateLateStartAndFinish(float maxEF, int numberOfTasks, Task *arrTasks);
	void printStartAndFinish();
	~Task();
};



void Task::inputData(int numberOfTasks)
{
	cout << setw(25) << "Task's Name: ";
	getline(cin, name);
	transform(name.begin(), name.end(), name.begin(), to_upper);

	cout << setw(25) << "Duration: ";
	cin >> duration;

	do{
		cout << setw(25) << "Dependencies number: ";
		cin >> numberOfDependencies;
		if (numberOfDependencies > numberOfTasks - 1){
			cout << "    Dependencies should be less than number of Tasks!";
			Beep(1200, 800);
			Sleep(1500);
			cout << '\r' << "                                                        " << '\r';
		}
	} while (numberOfDependencies > numberOfTasks - 1);

	cin.ignore();
	Dependencies = new string[numberOfDependencies];

	for (int i = 0; i < numberOfDependencies; i++)
	{
		if (numberOfDependencies == 1)
			cout << setw(25) << "Dependency's Name: ";
		else if (i < 9)
			cout << setw(15) << "Dependency #" << i + 1 << "'s Name: ";
		else if (i >= 9)
			cout << setw(14) << "Dependency #" << i + 1 << "'s Name: ";

		getline(cin, Dependencies[i]);
		transform(Dependencies[i].begin(), Dependencies[i].end(), Dependencies[i].begin(), to_upper);
	}

}

void Task::calculateEarlyStartAndFinish(int numberOfTasks, Task *arrTasks)
{
	if (ES == -1)
	{
		if (numberOfDependencies == 0)
		{
			ES = 0;
		}
		else
		{
			float maxES = 0;

			for (int depNum = 0; depNum < numberOfDependencies; depNum++)
			{
				for (int taskNum = 0; taskNum < numberOfTasks; taskNum++)
				{
					if (Dependencies[depNum] == arrTasks[taskNum].name)
					{
						if (arrTasks[taskNum].EF == -1)
						{
							arrTasks[taskNum].calculateEarlyStartAndFinish(numberOfTasks, arrTasks);
						}
						if (maxES < arrTasks[taskNum].EF)
						{
							maxES = arrTasks[taskNum].EF;
						}
					}
				}
			}

			ES = maxES;
		}

		EF = ES + duration;
	}
}

void Task::initTerminalTasks(float maxEF, int numberOfTasks, Task *arrTasks){

	bool isFound = false;

	for (int taskNum = 0; taskNum < numberOfTasks; taskNum++)
	{
		for (int depNum = 0; depNum < arrTasks[taskNum].numberOfDependencies; depNum++)
		{
			if (name == arrTasks[taskNum].Dependencies[depNum])
			{
				isFound = true;
			}
		}
	}

	if (!isFound)
	{
		LF = maxEF;
		LS = maxEF - duration;
	}
}

void Task::calculateLateStartAndFinish(float maxEF, int numberOfTasks, Task *arrTasks)
{
	float minLS = maxEF;

	for (int taskNum = 0; taskNum < numberOfTasks; taskNum++)
	{
		for (int depNum = 0; depNum <arrTasks[taskNum].numberOfDependencies; depNum++)
		{
			if (name == arrTasks[taskNum].Dependencies[depNum])
			{
				if (arrTasks[taskNum].LS == -1)
				{
					arrTasks[taskNum].calculateLateStartAndFinish(maxEF, numberOfTasks, arrTasks);
				}

				if (minLS > arrTasks[taskNum].LS)
				{
					minLS = arrTasks[taskNum].LS;
				}
			}
		}

		LF = minLS;
	}

	LS = LF - duration;
}



void Task::printStartAndFinish()
{
	cout << name << ":\n";
	cout << "ES: " << setw(6) << left << ES << "EF: " << EF << endl
		<< "LS: " << setw(6) << left << LS << "LF: " << LF << endl << endl;
}

Task::~Task()
{
	delete[]Dependencies;
}


#include <iostream>
#include <string>
#include <Windows.h>
#include <iomanip>
using namespace std;

void findCP(float maxEF, int numberOfTasks, Task *arrTasks);
int getNextTask(float maxEF, int numberOfTasks, Task *arrTasks, Task &x);

int main()
{
	//SetConsoleTitle(L"CPM - Critical Path Method: C++ Sample Application");
	cout << "\n CPM - Critical Path Method C++ Sample Application\n";
	cout << " This program demonstrates the Critical Path Method's algorithm.\n";
	cout << "------------------------------------------------------------------\n\n";

	int numberOfTasks;
	do{
		cout << setw(25) << "Enter Number of Tasks: ";
		cin >> numberOfTasks;
		if (numberOfTasks <= 2){
			cout << "  Number of tasks should be greater than 2!";
			Beep(1200, 800);
			Sleep(1500);
			cout << '\r' << "                                                        " << '\r';
		}
	} while (numberOfTasks <= 2);
	cin.ignore();


	// Input
	Task *arrTasks = new Task[numberOfTasks];
	for (int i = 0; i < numberOfTasks; i++) {
		cout << "\n\n";
		cout << setw(23) << "Task #" << i + 1 << "\n\n";
		arrTasks[i].inputData(numberOfTasks);
	}

	// Calculate Early Start and Eatly Finish
	for (int i = 0; i < numberOfTasks; i++) {
		arrTasks[i].calculateEarlyStartAndFinish(numberOfTasks, arrTasks);
	}

	// Get Maximum Early Finish of all Tasks
	float maxEF = 0;
	for (int i = 0; i < numberOfTasks; i++)
	{
		if (maxEF < arrTasks[i].EF)
			maxEF = arrTasks[i].EF;
	}

	// Assign Maximum Early Finish to the Late Finish Terminal Tasks
	// and calculate their Late Start
	for (int i = 0; i < numberOfTasks; i++)
	{
		arrTasks[i].initTerminalTasks(maxEF, numberOfTasks, arrTasks);
	}

	// Calculate Late Start and Eatly Finish
	for (int i = 0; i < numberOfTasks; i++)
	{
		arrTasks[i].calculateLateStartAndFinish(maxEF, numberOfTasks, arrTasks);
	}

	cout << "\n\n------------------------------------------------------------------\n\n";

	//Output
	cout << "\nTotal Duration: " << maxEF << "\n";

	cout << "\n\n\nCritical Path(s):\n\n";
	findCP(maxEF, numberOfTasks, arrTasks);

	cout << "\n\n\nTasks' Early/Late Starts and Finishes: \n\n";
	for (int i = 0; i < numberOfTasks; i++) {
		arrTasks[i].printStartAndFinish();
	}

	cout << "\n\n\n";
	delete[]arrTasks;
	system("pause");
}



void findCP(float maxEF, int numberOfTasks, Task *arrTasks)
{
	string CP = "- ";
	for (int i = 0; i < numberOfTasks; i++)
	{
		if (arrTasks[i].ES == 0 && arrTasks[i].LS - arrTasks[i].ES == 0 && arrTasks[i].LF - arrTasks[i].EF == 0)
		{
			while (true)
			{
				CP += arrTasks[i].name + " "; //add first one to Critical Path

				int index = 0;
				index = getNextTask(maxEF, numberOfTasks, arrTasks, arrTasks[i]); //get index of next of first one

				if (index != -1)
				{
					CP += arrTasks[index].name + " "; //add next of first one to Critical Path

					while (true)
					{
						index = getNextTask(maxEF, numberOfTasks, arrTasks, arrTasks[index]); //get index of next of next

						if (index != -1)
							CP += arrTasks[index].name + " "; //add next of next to Critical Path
						else
							break;
					}

					cout << CP << endl;
					CP = "- ";
				}
				else
					break;
			}
		}
	}
}

int getNextTask(float maxEF, int numberOfTasks, Task *arrTasks, Task &x)
{
	/// returns index of next task in the array of Tasks if found
	/// or return -1 if not
	/// N.B.: Task must have v1 = v2 = 0

	for (int i = 0; i < numberOfTasks; i++)
	{
		for (int depNum = 0; depNum < arrTasks[i].numberOfDependencies; depNum++)
		{
			if (arrTasks[i].Dependencies[depNum] == x.name)
			{
				if (arrTasks[i].LS - arrTasks[i].ES == 0 && arrTasks[i].LF - arrTasks[i].EF == 0)
				{
					if (arrTasks[i].LF != maxEF)
						arrTasks[i].Dependencies[depNum] = "";
					return i;
				}
			}
		}
	}
	return -1;
}
