#include <string>
#include <vector>
#include <iostream>

using namespace std;
// hour: 11, 12
// minute: 14, 15
// second: 17, 18 + 20, 21, 22
// time: 24~ s������

int main()
{
    vector<string> lines;

    lines.push_back("2016-09-15 01:00:04.001 2.0s");
    lines.push_back("2016-09-15 01:00:07.000 2s");

    int answer = 0;
    vector<pair<double, double>> start_end;
    vector<pair<double, double>> section;

    // ���� ó��: ���� �ð�(total ��), �� �ð�(total ��) ��ȯ
    for (string s : lines)
    {
        int hour = stoi(s.substr(11, 2));
        int minute = stoi(s.substr(14, 2));
        double second = stod(s.substr(17, 6));
        double time = stod(s.substr(24)) - (double)0.001;

        double endSec = (hour * 3600 + minute * 60 + second);
        double startSec = endSec - time;
        printf("startSec = %.100lf\n", startSec);
        start_end.push_back(make_pair(startSec, endSec));             // ���۰� ��
        section.push_back(make_pair(endSec, endSec + (double)0.999)); // ����(���� �����ϹǷ�)
    }

    // ���� ó��
    int max = 0;
    for (int i = 0; i < section.size(); i++)
    {
        int count = 0;
        int start = section[i].first;
        int end = section[i].second;
        for (int j = 0; j < start_end.size(); j++)
        {
            cout << "first1 = " << start_end[j].first << endl;
            printf("first2 = %.100lf\n", start_end[j].first);
            if (start_end[j].first == end)
            {
                cout << "�ٸ���" << endl;
            }
            if (start_end[j].second >= start &&
                start_end[j].first <= end)
            {
                // ���� �ȿ� ����
                count++;
            }
            else if (start_end[j].first - end > (double)2.999)
                break;
        }
        if (max < count)
            max = count;
    }

    cout << max << endl;
    return 0;
}