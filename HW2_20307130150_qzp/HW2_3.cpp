#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::istream;
using std::ostream;
using std::ofstream;
using std::ifstream;
using std::string;

//定义的HangmanModule类仅能用于此题计算, 而非通用bayesnet模板
class HangmanModule
{
public:
    HangmanModule(): sum(0) {}//默认构造函数
    //流输入构造, 用于读取文件hw2_word_counts_05.txt并初始化参数列表
    HangmanModule(istream& in): sum(0)
    {
        string w;
        long long tmp;
        int i, j;
        while (in >> w)
        {
            words.push_back(w);
            in >> tmp;
            times.push_back(tmp);
            sum += tmp;
        }
        for (i = 0; i < words.size(); i++)
        {
            for (j = i + 1; j < words.size(); j++)
            {
                if (times[j] < times[i])
                {
                    tmp = times[j];
                    times[j] = times[i];
                    times[i] = tmp;
                    w = words[j];
                    words[j] = words[i];
                    words[i] = w;
                }
            }
        }
        for (i = 0; i < times.size(); i++)
        {
            p.push_back(((long double) times[i]) / sum);
        }
    }
    /*
    计算题b的成员函数, 其中ans用于保存最大概率的那个字母,a为correctly guessed
    中的参数列表, 其中'-'用'\0'表示以便进行bool运算; notIn为incorrectly guessed
    中的参数列表
    */
    long double pE(char& ans, vector<char> a, vector<char> notIn)
    {
        /*
        subW用于保存样式为correctly guessed中的单词, 而subT以及subP为其对应的
        次数和频率, alphaBetP用于存储猜测符合条件的单词的次数
        */
        vector<string> subW;
        vector<long long> subT, alphaBetP;
        vector<long double> subP;
        //alphaBet为顺序26个字母表, 声明为静态变量以减小增删开销
        static string alphaBet("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        long long tmpLI = 0, tmpFI = 0;
        int i, j, k;
        bool pin;
        /*
        将correctly guessed中猜对的字母纳入notIn, 代表在后面的猜测中不需要考虑
        已经猜对的正确字母
        */
        for (i = 0; i < 5; i++)
        {
            if (a[i]) notIn.push_back(a[i]);
        }
        //筛选符合条件的单词, 初始化subW, subT, subP
        for (i = 0; i < words.size(); i++)
        {
            pin = true;
            for (j = 0; j < 5; j++)
            {
                if (a[j])
                {
                    if (a[j] != words[i][j])
                    {
                        pin = false;
                        break;
                    }
                }
                else
                {
                    for (k = 0; k < notIn.size(); k++)
                    {
                        if (words[i][j] == notIn[k])
                        {
                            pin = false;
                            break;
                        }
                    }
                }
            }
            if (pin)
            {
                subW.push_back(words[i]);
                subT.push_back(times[i]);
                subP.push_back(p[i]);
            }
        }
        //计算alphaBetP
        for (i = 0; i < subT.size(); i++)
        {
            tmpFI += subT[i];
        }
        for (i = 0; i < 26; i++)
        {
            pin = true;
            for (j = 0; j < notIn.size(); j++)
            {
                if (alphaBet[i] == notIn[j])
                {
                    pin = false;
                    break;
                }
            }
            if (!pin)
            {
                alphaBetP.push_back(0);
                continue;
            }
            tmpLI = 0;
            for (j = 0; j < subW.size(); j++)
            {
                pin = false;
                for (k = 0; k < 5; k++)
                {
                    if (!a[k])
                    {
                        if (alphaBet[i] == subW[j][k])
                        {
                            pin = true;
                            break;
                        }
                    }
                }
                if (pin) tmpLI += subT[j];
            }
            alphaBetP.push_back(tmpLI);
        }
        tmpLI = alphaBetP[0];
        j = 0;
        //找出符合条件的最大alphaBetP成员
        for (i = 0; i < alphaBetP.size(); i++)
        {
            if (alphaBetP[i] > tmpLI)
            {
                j = i;
                tmpLI = alphaBetP[i];
            }
        }
        ans = alphaBet[j];//将引用值归为最大的那个字母
        return ((long double)tmpLI) / tmpFI;
    }
    //下列三个为访问器函数
    vector<string> Words()
    {
        return words;
    }
    vector<long long> Times()
    {
        return times;
    }
    vector<long double> P()
    {
        return p;
    }
    //析构函数
    ~HangmanModule() {}
private:
    /*
    words存储使用频率由少到多的所有单词
    times存储对应words中单词的出现次数, 使用长整型防止溢出
    p存储对应words中单词的频率, 其中使用长双精度防止截断误差
    sum存储总单词数, 使用长整型防止溢出
    */
    vector<string> words;
    vector<long long> times;
    vector<long double> p;
    long long sum;
};

int main()
{
    long long sum = 0, tmp;
    ifstream data("hw2_word_counts_05.txt");
    HangmanModule ans(data);
    vector<string> w;
    vector<long long> t;
    vector<long double> p;
    w = ans.Words();
    t = ans.Times();
    p = ans.P();
    cout.precision(4);//将输出浮点数精度设置为4
    //输出频率最高的20个单词及其相应的次数和频率(担心有相同次数的情况)
    cout << "The Most" << endl << "Word\tCount\tProbability" << endl;
    for (int i = w.size() - 1; i > w.size() - 21; i--)
    {
        cout << w[i] << "\t" << t[i] << "\t" << p[i] << endl;
    }
    cout << endl;
    //输出频率最低的20个单词及其对应的次数和频率(担心有相同次数的情况)
    cout << "The Least" << endl << "Word\tCount\tProbability" << endl;
    for (int i = 0; i < 20; i++)
    {
        cout << w[i] << "\t" << t[i] << "\t" << p[i] << endl;
    }
    cout << endl;
    //ansDouble暂存概率, ansChar暂存概率最大的字母(未编写排版函数, 所以有些混乱)
    long double ansDouble;
    char ansChar;
    cout << "Guess\tIncorrect\tBest\tP" << endl;
    ansDouble = ans.pE(ansChar, {'\0', '\0', '\0', '\0', '\0'}, {});
    cout << "-----\t{}\t\t" << ansChar << '\t' << ansDouble << endl;
    ansDouble = ans.pE(ansChar, {'\0', '\0', '\0', '\0', '\0'}, {'E', 'O'});
    cout << "-----\t{E,O}\t\t" << ansChar << '\t' << ansDouble << endl;
    ansDouble = ans.pE(ansChar, {'Q', '\0', '\0', '\0', '\0'}, {});
    cout << "Q----\t{}\t\t" << ansChar << '\t' << ansDouble << endl;
    ansDouble = ans.pE(ansChar, {'Q', '\0', '\0', '\0', '\0'}, {'U'});
    cout << "Q----\t{U}\t\t" << ansChar << '\t' << ansDouble << endl;
    ansDouble = ans.pE(ansChar, {'\0', '\0', 'Z', 'E', '\0'}, {'A', 'D', 'I', 'R'});
    cout << "--ZE-\t{A,D,I,R}\t" << ansChar << '\t' << ansDouble << endl;
    ansDouble = ans.pE(ansChar, {'\0', '\0', '\0', '\0', '\0'}, {'E', 'O'});
    cout << "-----\t{E,O}\t\t" << ansChar << '\t' << ansDouble << endl;
    ansDouble = ans.pE(ansChar, {'D', '\0', '\0', 'I', '\0'}, {});
    cout << "D--I-\t{}\t\t" << ansChar << '\t' << ansDouble << endl;
    ansDouble = ans.pE(ansChar, {'D', '\0', '\0', 'I', '\0'}, {'A'});
    cout << "D--I-\t{A}\t\t" << ansChar << '\t' << ansDouble << endl;
    ansDouble = ans.pE(ansChar, {'\0', 'U', '\0', '\0', '\0'}, {'A', 'E', 'I', 'O', 'S'});
    cout << "-U---\t{A,E,I,O,S}\t" << ansChar << '\t' << ansDouble << endl;
    return 0;
}
