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

		cout << "���� ũ�⸦ ���ϼ��� (�ּ� 3) : ";
		cin >> scale;
		if (scale < 3)
		{
			cout << "�ٽ� ���ϼ��� " << endl;
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
            cout << "�� ���� : " << userBingo << endl << endl;
            showTable(comTable, scale);
            cout << "��ǻ�� ���� : " << comBingo << endl;

            if (userBingo >= 3 && comBingo >= 3)
            {
                cout << "���� ���� �޼�!! �����ϴ�." << endl;
                break;
            }
            else if (userBingo >= 3)
            {
                cout << "���� �޼�!! �̰���ϴ�." << endl;
                break;
            }
            else if (comBingo >= 3)
            {
                cout << "��ǻ�� ���� �޼�!! �����ϴ�." << endl;
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

        cout << "0. ��������, 1. �� ���� : ";
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

    cout << "���� (1 ~ " << index * index << ") : ";
    cin >> nSelect;

    // ���̺� ��ü�� �˻�
    for (int i = 0; i < index * index; i++)
    {
        if (nSelect == userTable[i])
        {
            // 0 => ���̺��� �̹� ���õ� ���ڸ� �ǹ�
            userTable[i] = 0;

            // ��ǻ�� ���̺� �Ȱ��� ���ڸ� ó��
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

        // ������ ���ڸ� ã�� ���� ���
        if (i == index * index - 1)
        {
            cout << "����� �����ϼ���. " << endl;
            Sleep(1000);
        }
    }
}

void computerTurn(int* userTable, int* comTable, int index)
{
    int nIndex = 0;
    int nSelect;
    uniform_int_distribution<> randNum(0, index * index - 1);

    while (true) // ������ ���õ� ���� ���� ĭ(�迭�� Index)�� �����Ѵ�.
    {
        nIndex = randNum(mersenne);
        nSelect = comTable[nIndex];

        if (nSelect != 0) // ���ڰ� ���� ĭ�� ã������
        {
            for (int i = 0; i < index * index; i++) // ���� ���̺��� ���õ� ���ڸ� ó���ϰ�
            {
                if (nSelect == userTable[i])
                {
                    userTable[i] = 0;
                    break;
                }
            }

            comTable[nIndex] = 0; // ��ǻ�� ���̺����� ���õ� ���ڸ� ó���Ѵ�.
            break;
        }
    }

    cout << "��ǻ�� ���� : " << nSelect << endl;
    Sleep(2000);
}

int checkBingo(int* table, int index)
{
    int nBingo = 0;

    // ���� �˻� 5
    for (int i = 0; i < index; i++)
    {
        for (int j = 0; j < index; j++)
        {
            if (table[i * index + j] == 0) // ��ĭ�� ���� ���� Ȯ��
            {
                if (j == index - 1) // ���� ������ĭ���� ������ �Ǿ��ִ� �����
                {
                    nBingo++;
                }
            }
            else
                break;
        }
    }

    // ���� �˻� 5
    for (int i = 0; i < index; i++)
    {
        for (int j = 0; j < index; j++)
        {
            if (table[j * index + i] == 0) // ��ĭ�� ���� ���� Ȯ��
            {
                if (j == index - 1) // ���� ������ĭ���� ������ �Ǿ��ִ� �����
                {
                    nBingo++;
                }
            }
            else
                break;
        }
    }

    // ���� �缱
    for (int i = 0; i < index; i++)
    {
        if (table[i * index + i] == 0)
        {
            if (i == index - 1) // ���� ������ĭ���� ������ �Ǿ��ִ� �����
            {
                nBingo++;
            }
        }
        else
            break;
    }

    // ������ �缱
    for (int i = 0; i < index; i++)
    {
        if (table[(i + 1) * (index - 1)] == 0)
        {
            if (i == index - 1) // ���� ������ĭ���� ������ �Ǿ��ִ� �����
            {
                nBingo++;
            }
        }
        else
            break;
    }
    return nBingo;
}
