// st1/algo1.cpp
// 展现算法的使用方式
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    using std::cout;
    using std::endl;
    
    std::vector<int> coll;
    std::vector<int>::iterator pos;

    // insert elements from 1 to 6 in arbitrary order
    coll.push_back(2);
    coll.push_back(5);
    coll.push_back(4);
    coll.push_back(1);
    coll.push_back(6);
    coll.push_back(3);

    // find and print minimum and maximum elements
    pos = min_element(coll.begin(), coll.end());
    cout << "min: " << *pos << endl;
    pos = max_element(coll.begin(), coll.end());
    cout << "max: " << *pos << endl;

    // sort all elements
    std::sort(coll.begin(), coll.end());

    // find the first element with value 3
    pos = find(coll.begin(), coll.end(), 3);   // range, value

    // reverse the order of the found element with value 3 and all following elements
    std::reverse(pos, coll.end());

    // print all elements
    for(pos = coll.begin(); pos != coll.end(); ++pos)
        cout << *pos << ' ';
    cout << endl;

    system("pause");
    return 0;
}