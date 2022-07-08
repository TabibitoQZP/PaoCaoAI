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

class NOR
{
public:
    NOR(): n(0) {}//默认构造函数
    NOR(istream& in)//流输入构造, 将给定该题数据的txt格式
    {
        //先输入n, 再依次为 graph, graphVar, base, baseVar
        in >> n;
        string tmpString;
        for (size_t i = 0; i < n; i++)
        {
            in >> tmpString;
            names.push_back(tmpString);
        }
        vector<int> tmpVecInt;
        vector<double> tmpVecDouble;
        int tmpInt;
        double tmpDouble;
        for (size_t i = 0; i < n; i++)
        {
            tmpVecInt.clear();
            for (size_t j = 0; j < n; j++)
            {
                in >> tmpInt;
                tmpVecInt.push_back(tmpInt);
            }
            graph.push_back(tmpVecInt);
        }
        for (size_t i = 0; i < n; i++)
        {
            tmpVecDouble.clear();
            for (size_t j = 0; j < n; j++)
            {
                in >> tmpDouble;
                tmpVecDouble.push_back(tmpDouble);
            }
            graphVar.push_back(tmpVecDouble);
        }
        for (size_t i = 0; i < n; i++)
        {
            in >> tmpInt;
            base.push_back(tmpInt);
        }
        for (size_t i = 0; i < n; i++)
        {
            in >> tmpDouble;
            baseVar.push_back(tmpDouble);
        }
    }
    ~NOR() {}//析构函数
    double PFull(vector<bool> p)//全概率计算
    {
        double pSum = 1, tmpP;
        int i, j;
        for (i = 0; i < n; i++)//根节点计算
        {
            if (base[i])
            {
                if (p[i]) pSum *= baseVar[i];
                else pSum *= (1 - baseVar[i]);
            }
        }
        for (i = 0; i < n; i++)//子节点计算
        {
            if (base[i]) continue;
            tmpP = 1;
            for (j = 0; j < n; j++)
            {
                if (graph[j][i] && p[j]) tmpP *= (1 - graphVar[j][i]);
            }
            if (p[i]) pSum *= (1 - tmpP);
            else pSum *= tmpP;
        }
        return pSum;
    }
    //a[i]=1代表该全概率生效, abP=1代表点亮, na用于存储概率表示形式
    double P(vector<bool> a, vector<bool> b, vector<bool> abP, string &na)
    {
        double aSum = 0, bSum = 0;
        vector<vector<bool>> aPs = allP(a, abP), bPs = allP(b, abP);
        int i;
        for (i = 0; i < aPs.size(); i++) aSum += PFull(aPs[i]);
        for (i = 0; i < bPs.size(); i++) bSum += PFull(bPs[i]);
        na.clear();
        na += "P( ";
        for (i = 0; i < n; i++)
        {
            if (a[i])
            {
                na += names[i] + "=";
                if (abP[i]) na += "1 ";
                else na += "0 ";
            }
        }
        na += ") / P( ";
        for (i = 0; i < n; i++)
        {
            if (b[i])
            {
                na += names[i] + "=";
                if (abP[i]) na += "1 ";
                else na += "0 ";
            }
        }
        na += ")";
        return aSum / bSum;
    }
private:
    //用于返回一个bool值的所有组合
    vector<vector<bool>> allP(vector<bool> a, vector<bool> aP)
    {
        int i = 0, j = 0;
        vector<vector<bool>> ans;
        vector<bool> tmp;
        int sn = 0;
        for (i = 0; i < n; i++)
        {
            if (!a[i]) sn++;
        }
        long long s = 1, is, tmpis;
        for (i = 0; i < sn; i++)
        {
            s *= 2;
        }
        for (is = 0; is < s; is++)
        {
            tmp.clear();
            tmpis = is;
            for (j = 0; j < n; j++)
            {
                if (a[j]) tmp.push_back(aP[j]);
                else
                {
                    tmp.push_back(tmpis % 2);
                    tmpis /= 2;
                }
            }
            ans.push_back(tmp);
        }
        return ans;
    }
    int n;//一共n个节点
    vector<string> names;
    vector<vector<int>> graph;//graph[a][b]指代a指向b的图. 1代表a->b, 0代表不连接
    vector<vector<double>> graphVar;//有向图的值, 若不连接默认为0
    vector<int> base;//根节点, 为根节点的值为1, 否则为0
    vector<double> baseVar;//根节点对应的值
};

int main()
{
    ifstream inData("data2_2.txt");
    NOR ans(inData);
    string name;
    double tmpDouble;
    //下列输出与问题a比较检查程序是否有误
    cout << "The code check(compare with question a):" << endl;
    tmpDouble = ans.P({1, 1, 1, 1}, {0, 1, 1, 1}, {1, 0, 0, 0}, name);
    cout << name << " = " << tmpDouble << endl;
    tmpDouble = ans.P({1, 1, 1, 1}, {0, 1, 1, 1}, {1, 1, 0, 0}, name);
    cout << name << " = " << tmpDouble << endl;
    tmpDouble = ans.P({1, 1, 1, 1}, {0, 1, 1, 1}, {1, 0, 1, 0}, name);
    cout << name << " = " << tmpDouble << endl;
    tmpDouble = ans.P({1, 1, 1, 1}, {0, 1, 1, 1}, {1, 0, 0, 1}, name);
    cout << name << " = " << tmpDouble << endl;
    tmpDouble = ans.P({1, 1, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 0}, name);
    cout << name << " = " << tmpDouble << endl;
    tmpDouble = ans.P({1, 1, 1, 1}, {0, 1, 1, 1}, {1, 1, 0, 1}, name);
    cout << name << " = " << tmpDouble << endl;
    tmpDouble = ans.P({1, 1, 1, 1}, {0, 1, 1, 1}, {1, 0, 1, 1}, name);
    cout << name << " = " << tmpDouble << endl;
    tmpDouble = ans.P({1, 1, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}, name);
    cout << name << " = " << tmpDouble << endl;
    //输出条件概率
    cout << endl << "The answer of question b:" << endl;
    tmpDouble = ans.P({0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 1, 0}, name);
    cout << name << " = " << tmpDouble << endl;
    tmpDouble = ans.P({1, 0, 1, 0}, {1, 0, 0, 0}, {0, 0, 1, 0}, name);
    cout << name << " = " << tmpDouble << endl;
    tmpDouble = ans.P({1, 0, 1, 0}, {1, 0, 0, 0}, {1, 0, 1, 0}, name);
    cout << name << " = " << tmpDouble << endl;
    tmpDouble = ans.P({1, 1, 1, 1}, {1, 1, 0, 1}, {1, 0, 1, 0}, name);
    cout << name << " = " << tmpDouble << endl;
    tmpDouble = ans.P({1, 1, 1, 1}, {1, 1, 0, 1}, {1, 1, 1, 1}, name);
    cout << name << " = " << tmpDouble << endl;
    return 0;
}
