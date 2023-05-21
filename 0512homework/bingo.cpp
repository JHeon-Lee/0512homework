#include <iostream>
#include <random>
#include <Windows.h>

using namespace std;

random_device rd;
mt19937_64 mersenne(rd());

void initTable(int* table, int index);
void showTable(int* table, int index);
void playerTurn(int* userTable, int* comTable, int index);
void computerTurn(int* userTable, int* comTable, int index);
int checkBingo(int* table, int index);

int main()
{
	bool isPlaying(true);

	while (isPlaying)
	{
		int scale = 0;
        bool isMyTurn = true;

		cout << "빙고 크기를 정하세요 (최소 3) : ";
		cin >> scale;
		if (scale < 3)
		{
			cout << "다시 정하세요 " << endl;
			continue;
		}

		int* userTable = new int[scale * scale];
		int* comTable = new int[scale * scale];
		int userBingo = 0;
		int comBingo = 0;

		initTable(userTable, scale);
		initTable(comTable, scale);

		while (true)
		{
			system("cls");

            userBingo = checkBingo(userTable, scale);
            comBingo = checkBingo(comTable, scale);

            showTable(userTable, scale);
            cout << "내 빙고 : " << userBingo << endl << endl;
            showTable(comTable, scale);
            cout << "컴퓨터 빙고 : " << comBingo << endl;

            if (userBingo >= 3 && comBingo >= 3)
            {
                cout << "빙고 동시 달성!! 비겼습니다." << endl;
                break;
            }
            else if (userBingo >= 3)
            {
                cout << "빙고 달성!! 이겼습니다." << endl;
                break;
            }
            else if (comBingo >= 3)
            {
                cout << "컴퓨터 빙고 달성!! 졌습니다." << endl;
                break;
            }

            if (isMyTurn)
            {
                playerTurn(userTable, comTable, scale);
                isMyTurn = false;
            }
            else
            {
                computerTurn(userTable, comTable, scale);
                isMyTurn = true;
            }
		}

        delete[] userTable;
        delete[] comTable;

        cout << "0. 게임종료, 1. 새 게임 : ";
        cin >> isPlaying;
	}

	return 0;
}

void initTable(int* table, int index)
{
    uniform_int_distribution<> randNum(0, index * index - 1);

	for (int i = 0; i < index * index; i++)
	{
		table[i] = i + 1;
	}

	int src(0);
	int dest(0);

	for (int i = 0; i < 250; i++)
	{
		src = randNum(mersenne);
		dest = randNum(mersenne);

		int temp = table[src];
		table[src] = table[dest];
		table[dest] = temp;
	}
}

void showTable(int* table, int index)
{
    cout << endl;
    for (int i = 0; i < index * index; i++)
    {
        cout << "|  " << table[i] << "\t|";

        if (i % index == index - 1)
        {
            cout << endl;
            cout << endl;
        }
    }
}

void playerTurn(int* userTable, int* comTable, int index)
{
    int nSelect;

    cout << "선택 (1 ~ " << index * index << ") : ";
    cin >> nSelect;

    // 테이블 전체를 검사
    for (int i = 0; i < index * index; i++)
    {
        if (nSelect == userTable[i])
        {
            // 0 => 테이블에서 이미 선택된 숫자를 의미
            userTable[i] = 0;

            // 컴퓨터 테이블도 똑같은 숫자를 처리
            for (int j = 0; j < 25; j++)
            {
                if (nSelect == comTable[j])
                {
                    comTable[j] = 0;
                    break;
                }
            }
            break;
        }

        // 선택한 숫자를 찾지 못한 경우
        if (i == index * index - 1)
        {
            cout << "제대로 선택하세요. " << endl;
            Sleep(1000);
        }
    }
}

void computerTurn(int* userTable, int* comTable, int index)
{
    int nIndex = 0;
    int nSelect;
    uniform_int_distribution<> randNum(0, index * index - 1);

    while (true) // 이전에 선택된 적이 없는 칸(배열의 Index)을 선택한다.
    {
        nIndex = randNum(mersenne);
        nSelect = comTable[nIndex];

        if (nSelect != 0) // 숫자가 남은 칸을 찾았으면
        {
            for (int i = 0; i < index * index; i++) // 유저 테이블에서 선택된 숫자를 처리하고
            {
                if (nSelect == userTable[i])
                {
                    userTable[i] = 0;
                    break;
                }
            }

            comTable[nIndex] = 0; // 컴퓨터 테이블에서도 선택된 숫자를 처리한다.
            break;
        }
    }

    cout << "컴퓨터 선택 : " << nSelect << endl;
    Sleep(2000);
}

int checkBingo(int* table, int index)
{
    int nBingo = 0;

    // 가로 검사 5
    for (int i = 0; i < index; i++)
    {
        for (int j = 0; j < index; j++)
        {
            if (table[i * index + j] == 0) // 한칸씩 선택 유무 확인
            {
                if (j == index - 1) // 제일 마지막칸까지 선택이 되어있는 경우라면
                {
                    nBingo++;
                }
            }
            else
                break;
        }
    }

    // 세로 검사 5
    for (int i = 0; i < index; i++)
    {
        for (int j = 0; j < index; j++)
        {
            if (table[j * index + i] == 0) // 한칸씩 선택 유무 확인
            {
                if (j == index - 1) // 제일 마지막칸까지 선택이 되어있는 경우라면
                {
                    nBingo++;
                }
            }
            else
                break;
        }
    }

    // 왼쪽 사선
    for (int i = 0; i < index; i++)
    {
        if (table[i * index + i] == 0)
        {
            if (i == index - 1) // 제일 마지막칸까지 선택이 되어있는 경우라면
            {
                nBingo++;
            }
        }
        else
            break;
    }

    // 오른쪽 사선
    for (int i = 0; i < index; i++)
    {
        if (table[(i + 1) * (index - 1)] == 0)
        {
            if (i == index - 1) // 제일 마지막칸까지 선택이 되어있는 경우라면
            {
                nBingo++;
            }
        }
        else
            break;
    }
    return nBingo;
}
