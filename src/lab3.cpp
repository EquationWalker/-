#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <stack>
#include <cstdlib>
using namespace std;

class SLR1
{
private:
    string m_actionTable[12][6] = {"S5", "0", "0", "S4", "0", "0",
                                   "0", "S6", "0", "0", "0", "acc",
                                   "0", "r2", "S7", "0", "r2", "r2",
                                   "0", "r4", "r4", "0", "r4", "r4",
                                   "S5", "0", "0", "S4", "0", "0",
                                   "0", "r6", "r6", "0", "r6", "r6",
                                   "S5", "0", "0", "S4", "0", "0",
                                   "S5", "0", "0", "S4", "0", "0",
                                   "0", "S6", "0", "0", "S11", "0",
                                   "0", "r1", "S7", "0", "r1", "r1",
                                   "0", "r3", "r3", "0", "r3", "r3",
                                   "0", "r5", "r5", "0", "r5", "r5"};
    int m_gotoTable[12][3] = {1, 2, 3,
                              0, 0, 0,
                              0, 0, 0,
                              0, 0, 0,
                              8, 2, 3,
                              0, 0, 0,
                              0, 9, 3,
                              0, 0, 10,
                              0, 0, 0,
                              0, 0, 0,
                              0, 0, 0,
                              0, 0, 0};
    string m_VT = "i+*()#";
    string m_VN = "ETF";
    vector<string> m_production = {"E->E+T", "E->T", "T->T*F", "T->F", "F->(E)", "F->i"};
    vector<int> m_stateStack;
    vector<char> m_symbolStack;
    int m_curPos;
    int m_step;
    string m_target;

    char m_errorMsg[100];

    void init(string &s)
    {
        m_target = s;
        m_stateStack.push_back(0);
        m_symbolStack.push_back('#');
        m_curPos = 0;
        m_step = 1;
    }
    void error()
    {
        cout << "Error\t" << m_errorMsg << endl;
        exit(0);
    }

    void printStep(int stateNum, int flag = 0)
    {
        cout << m_step++ << '\t';
        for (int i = 0; i < m_stateStack.size(); i++)
            cout << m_stateStack[i];
        cout << "\t";
        for (int i = 0; i < m_symbolStack.size(); i++)
            cout << m_symbolStack[i];
        cout << '\t';
        cout << m_target.substr(m_curPos) << '\t';
        if (flag == 0)
            cout << "\tS" << stateNum << endl;
        else if (flag == 1)
        {
            // ִ�й�Լ
            reduction(stateNum);
            cout << "\tr" << stateNum << '\t';
            if (m_stateStack.back() == 0)
                error();
            else cout << m_stateStack.back() << endl;
        }
        else if (flag == 2)
            cout << "\tacc" << endl;
        else 
            error();
    }

    int getPos(char ch, bool isVT = true)
    {
        string::size_type res;
        if (isVT)
            res = m_VT.find(ch);
        else
            res = m_VN.find(ch);
        if (res == string::npos)
        {
            sprintf(m_errorMsg, "The input symbol string contains illegal characters!");
            error();
        }
        return res;
    }
    void shift(int stateNum)
    {
        char symbol = m_target[m_curPos++];
        m_stateStack.push_back(stateNum);
        m_symbolStack.push_back(symbol);
    }

    void reduction(int stateNum)
    {
        string production = m_production[stateNum - 1];
        int len = production.size() - 3;
        for (int i = 0; i < len; i++)
        {
            m_stateStack.pop_back();
            m_symbolStack.pop_back();
        }
        m_symbolStack.push_back(production[0]);
        int pos = getPos(production[0], false);
        int topState = m_stateStack.back();
        int newState = m_gotoTable[topState][pos];
        // goto������
        if (newState == 0)
            sprintf(m_errorMsg, "Goto(%d, %c) is null!", topState, production[0]);
        m_stateStack.push_back(newState);
        
    }

public:
    void analyse(string &s)
    {
        init(s);
        while (m_curPos < m_target.size())
        {
            int t = getPos(m_target[m_curPos]);
            int i = m_stateStack.back();
            if (m_actionTable[i][t] == "acc")
            {
                printStep(i, 2);
                break;
            }
            // action������
            else if (m_actionTable[i][t] == "0")
            {
                sprintf(m_errorMsg, "Action(%d, %d) is null!", i, t);
                printStep(i, 3);
                break;
            }
            else if (m_actionTable[i][t][0] == 'S')
            {
                int stateNum = 0;
                for (int j = 1; j < m_actionTable[i][t].size(); j++)
                    stateNum = stateNum * 10 + m_actionTable[i][t][j] - '0';
                printStep(stateNum, 0);
                shift(stateNum); 
            }
            else
            {
                int stateNum = 0;
                for (int j = 1; j < m_actionTable[i][t].size(); j++)
                    stateNum = stateNum * 10 + m_actionTable[i][t][j] - '0';
                // ��Լ���̷��ڴ�ӡ��,��Ϊ��Ҫ���Gotoֵ�Ͳ���ǰ��ջ״̬
                printStep(stateNum, 1); 
            }
        }
    }
};
string s;
int main()
{
    SLR1 analyer;
    cout << endl << "SLR(1)��������,������:ƫƫϲ����" << endl;
    cout << "����һ����#�����ķ��Ŵ�(����+��*/()i#):" << endl;
    cin >> s;
    cout << "�����������:" << endl;
    cout << "����"
         << "\t"
         << "״̬ջ"
         << "\t"
         << "����ջ"
         << "\t"
         << "ʣ�����봮"
         << "\t"
         << "Action"
         << "\t"
         << "Goto" << endl;
    analyer.analyse(s);
    return 0;
}
