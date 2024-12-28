#include <iostream>
#include <vector>
using namespace std;

void bubbleSort(vector<int> &a)
{
    int len = a.size();
    for (int i = 0; i < len - 1; i++) // loop number
    {
        for (int j = 0; j < len - 1 - i; j++) //compare number
        {
            if (a[j] > a[j + 1])
            {
                swap(a[j], a[j + 1]); // swap elemnts
            }
        }
    }
}

void selectionSort(vector<int> &a)
{
    int len = a.size();
    for (int i = 0, minIndex; i < len - 1; i++)
    {
        minindex = i; // min index
        for (int j = i + 1; j < len; j++)
        {
            if (a[j] < a[minIndex])
                minIndex = j; // find the minimum element
        }
        swap(a[i], a[minIndex]);
    }
}

void insertionSort(vector<int> &a)
{
    int len = a.size();
    for (int i = 0; j, temp; i < len - 1; i++) //loop number
    {
        j = i;
        temp = a[i + 1];
        while (j >= 0 && a[j] > temp)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = temp;
    }
}

void shellSort(vector<int> &a)
{
    int len = a.size();
    for (int gap = len / 2; gap > 0; gap /= 2)
    {
        for (int i = 0; i < gap; i++)
        {
            for (int j = i + gap, temp, preIndex; j < len; j = j + gap)
            {
                temp = a[j]; //save the temp element
                preIndex = j - gap;
                while (preIndex >= 0 && a[preIndex] > temp)
                    {
                        a[preIndex + gap] = a[preIndex]; // be replaced
                        preIndex -= gap; // go next step
                    }
                a[preIndex + gap] = temp; // recover the replaced element
            }
        }
    }
}

void quickSort(vector<int> &a)
{

}

void printVector(const vector<int> &a)
{
    for (int i = 0; i < a.size(); i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;
}

int main()
{
    // Test case 1: Random unsorted vector
    vector<int> a1 = {64, 34, 25, 12, 22, 11, 90};
    cout << "Before sorting: ";
    printVector(a1);
    bubbleSort(a1);
    cout << "After sorting: ";
    printVector(a1);

    // Test case 2: Already sorted vector
    vector<int> a2 = {1, 2, 3, 4, 5, 6};
    cout << "Before sorting: ";
    printVector(a2);
    bubbleSort(a2);
    cout << "After sorting: ";
    printVector(a2);

    // Test case 3: Reverse sorted vector
    vector<int> a3 = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    cout << "Before sorting: ";
    printVector(a3);
    bubbleSort(a3);
    cout << "After sorting: ";
    printVector(a3);

    // Test case 4: Vector with duplicate elements
    vector<int> a4 = {5, 3, 8, 3, 1, 9, 5};
    cout << "Before sorting: ";
    printVector(a4);
    bubbleSort(a4);
    cout << "After sorting: ";
    printVector(a4);

    // Test case 5: Single element vector
    vector<int> a5 = {42};
    cout << "Before sorting: ";
    printVector(a5);
    bubbleSort(a5);
    cout << "After sorting: ";
    printVector(a5);

    return 0;
}