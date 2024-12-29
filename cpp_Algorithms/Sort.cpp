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
        minIndex = i; // min index
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
    for (int i = 0, j, temp; i < len - 1; i++) //loop number
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

int partition(vector<int> &a, int left, int right)
{
    int pivot = a[right];
    int i = left - 1;
    for (int j = left, j < right; j++)
    {
        if (a[j] <= pivot)
        {
            i++;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[right]);
    return i + 1;
}

void quickSort(vector<int> &a)
{
    if (left < right)
    {
        int mid = partition(a, left, right);
        quickSort(a, left, mid - 1);
        quickSort(a, mid + 1, right);
    }
}

void qSort(vector<int> &a)
{
    quickSort(a, 0, a.size() - 1);
}

void mergeSort(vector<int> &a, vector<int> &T, int left, int right)
{
    if (right - left == 1)
        return;
    int mid = left + right >> 1, tmid = left + right >> 1, tleft = left, i = left;
    mergeSort(a, T, left, mid), mergeSort(a, T, mid, right);
    while (tleft < mid || tmid < right)
        {
            if (tmid >= right || (tleft < mid && a[tleft] <= a[tmid]))
            {
                T[i++] = a[tleft++];
            }
            else
            {
                T[i++] = a[tmid++];
            }
        }
        for (int i = left; i < right; i++)
            a[i] = T[i];
}

void mSort(vector<int> &a)
{
    int len = a.size();
    vector<int> T(len);
    mergeSort(a, T, 0, len);
}

void adjustHeap(vector<int> &a, int i, int len)
{
    int maxIndex = i;
    // compare
    if (i * 2 + 1 < len && a[i * 2 + 1] > a[maxIndex])
        maxIndex = i * 2 + 1;
    if (i * 2 + 2 < len && a[i * 2 + 2] > a[maxIndex])
        maxIndex = i * 2 + 2;
    if (maxIndex != i)
    {
        swap(a[maxIndex], a[i]);
        adjustHeap(a, maxIndex, len);
    }
}

void heapSort(vector<int> &a)
{
    int len = a.size();
    // build max Heap
    for (int i = len / 2 - 1; i >= 0; i--)
    {
        adjustHeap(a, i, len);
    }

    // swap the node, adjust the heap
    for (int i = len - 1; i > 0; i--)
        {
            swap[a[0], a[i]);
            adjustHeap(a, 0, i);
        }
}

void CountingSort(vector<int> &a)
{
    int len = a.size();
    if (len == 0)
        return;
    int Min = a[0], Max = a[0];
    for (int i = 1; i < len; i++)
    {
        Max = max(Max, a[i]);
        Min = min(Min, a[i]);
    }
    int bias = 0 - Min;
    vector<int> bucket (Max - Min + 1, 0);
    for (int i = 0; i < len; i++)
    {
        bucket[a[i] + bias]++;
    }
    int index = 0, i = 0;
    while (index < len)
    {
        if (bucket[i])
        {
            a[index] = i - bias;
            bucket[i]--;
            index++;
        }
        else
            i++;
    }
}

void bucketSort(vector<int> &a, int bucketSize)
{
    int len = a.size();
    if (len < 2)
        return;
    int Min = a[0], Max = a[0];
    for (int i = 1; i < len; i++)
    {
        Max = max(Max, a[i]);
        Min = min(Min, a[i]);
    }
    int bucketCount = (Max - Min) / bucketSize + 1;
    vector<int> bucketArr[bucketCount];
    for (int i = 0; i < len; i++)
    {
        bucketArr[ (a[i] - Min) / bucketSize].push_back(a[i]);
    }
    a.clear();
    for (int i = 0; i < bucketCount; i++)
    {
        int tlen = bucketArr[i].size();
        sort(bucketArr[i].begin(), bucketArr[i].end());
        for (int j = 0; j < tlen; j++)
            a.push_back(bucketArr[i][j]);
    }
}

void radixSort(vector<int> &a)
{
    int len = a.size();
    if (len < 2)
        return;
    int Max = a[0];
    for (int i = 1; i < len; i++)
    {
        Max = max(Max, a[i]);
    }
    int maxDigit = log10(Max) + 1;
    // use log10 to get digit for avoiding loop
    int mod = 10, div = 1;
    vector<int> bucketList[10];
    for (int i = 0; i < maxDigit; i++, mod *= 10, div *= 10)
    {
        for (int j = 0; j < len; j++)
        {
            int num = (a[j] % mod) / div;
            bucketList[num].push_back(a[j]);
        }
        int index = 0;
        for (int j = 0; j < 10; j++)
        {
            int tlen = bucketList[j].size();
            for (int k = 0; k < tlen; k++)
                a[index++] = bucketList[j][k];
            bucketList[j].clear();
        }
    }
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
