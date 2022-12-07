float averageArray(int *array, int elems)
{
    long sum = 0L;
    for (int i = 0; i < elems - 1; i++)
        sum += array[i];
    return ((float)sum) / elems;
}