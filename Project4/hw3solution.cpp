void listAll(const Class* c, string path)
{
    if (! path.empty())
        path += "=>";
    path += c->name();
    cout << path << endl;
    for (size_t k = 0; k != c->subclasses().size(); k++)
	listAll(c->subclasses()[k], path);
}

void insertion_sort(vector<Sensor>& s, bool comp(const Sensor&, const Sensor&))
{
    for (size_t k = 1; k < s.size(); k++)
    {
        Sensor currentSensor(s[k]);
        size_t m = k;
        for ( ; m > 0  &&  comp(currentSensor, s[m-1]); m--)
            s[m] = s[m-1];
        s[m] = currentSensor;
    }
}

====RECURSION====
bool allTrue(const double a[], int n)
{
    if (n <= 0)
        return true;
    if ( ! somePredicate(a[0]))
        return false;
    return allTrue(a+1, n-1);
}

int countFalse(const double a[], int n)
{
    if (n <= 0)
        return 0;
    int t = ! somePredicate(a[0]);  // 1 if false, 0 if true
    return t + countFalse(a+1, n-1);
}

int firstFalse(const double a[], int n)
{
    if (n <= 0)
        return -1;
    if ( ! somePredicate(a[0]))
        return 0;
    int k = firstFalse(a+1, n-1);
    if (k == -1)
        return -1;
    return 1 + k;  // element k of "the rest of a" is element 1+k of a
}

int indexOfMin(const double a[], int n)
{
    if (n <= 0)
        return -1;
    if (n == 1)
        return 0;
    int k = 1 + indexOfMin(a+1, n-1);  // indexOfMin can't return -1 here
    return a[0] <= a[k] ? 0 : k;
}

  // If all n2 elements of a2 appear in the n1 element array a1, in
  // the same order (though not necessarily consecutively), then
  // return true
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0)
        return true;
    if (n1 < n2)
        return false;
    if (a1[0] == a2[0])
        return includes(a1+1, n1-1, a2+1, n2-1);  // rest of a1, rest of a2
    else
        return includes(a1+1, n1-1, a2, n2);  // rest of a1, all of a2
}

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr == er  &&  sc == ec)
        return true;

    maze[sr][sc] = '@';  // anything non-'.' will do

    if (maze[sr-1][sc] == '.'  &&  pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
        return true;
    if (maze[sr+1][sc] == '.'  &&  pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
        return true;
    if (maze[sr][sc-1] == '.'  &&  pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
        return true;
    if (maze[sr][sc+1] == '.'  &&  pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
        return true;

    return false;
}
or

Problem 4:

  // Return the number of ways that all n2 elements of a2 appear
  // in the n1 element array a1 in the same order (though not
  // necessarily consecutively).
int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0)
        return 1;
    if (n1 < n2)
        return 0;
    int t = countIncludes(a1+1, n1-1, a2, n2);  // rest of a1, all of a2
    if (a1[0] == a2[0])
        t += countIncludes(a1+1, n1-1, a2+1, n2-1);  // rest of a1, rest of a2
    return t;
}

  // Exchange two doubles
void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}

  //   * for 0 <= i < firstNotGreater, a[i] > splitter
  //   * for firstNotGreater <= i < firstLess, a[i] == splitter
  //   * for firstLess <= i < n, a[i] < splitter
  // All the elements > splitter end up in no particular order.
  // All the elements < splitter end up in no particular order.
======QUICKSORT======
void split(double a[], int n, double splitter,
                                    int& firstNotGreater, int& firstLess)
{
    if (n < 0)
        n = 0;
    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
        if (a[firstUnknown] < splitter)
        {
            firstLess--;
            exchange(a[firstUnknown], a[firstLess]);
        }
        else
        {
            if (a[firstUnknown] > splitter)
            {
                exchange(a[firstNotGreater], a[firstUnknown]);
                firstNotGreater++;
            }
            firstUnknown++;
        }
    }
}


void order(double a[], int n)
{
    if (n <= 1)
        return;
    int firstNotGreater;
    int firstLess;
    split(a, n, a[0], firstNotGreater, firstLess);
    order(a, firstNotGreater);
    order(a+firstLess, n-firstLess);
}