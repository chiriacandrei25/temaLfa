#include <bits/stdc++.h>
#define Edge pair<int, char>

using namespace std;

const int Nmax = 100005;

int nrLetters;
char Letters[Nmax];

class DFA
{
public:
    int nrNodes, startNode;
    vector <int> finalNodes;
    vector < vector <Edge> > Edges;
    DFA(int n) {
        nrNodes = n;
        Edges.resize(2 * n);
    }
    DFA(){}
    int size() const {
        return nrNodes;
    }
    int findNode(int node, char letter) const {
        for(auto it : Edges[node])
            if(it.second == letter)
                return it.first;
        return -1;
    }
    friend istream& operator >> (istream &in, DFA &el);
    friend ostream& operator << (ostream& out, DFA &el);
};

istream& operator >> (istream &cin, DFA &el){
    map <int, int> Nodes;
    cin >> el.nrNodes;
    el.Edges.resize(2 * el.size());
    vector <int> Values(el.nrNodes);
    for(int i = 0; i < el.nrNodes; i++)
    {
        int val;
        cin >> val;
        Nodes[val] = i;
        Values[i] = val;
    }
    cin >> nrLetters;
    for(int i = 1; i <= nrLetters; i++)
        cin >> Letters[i];
    int nrFinalNodes, nrEdges;
    cin >> el.startNode >> nrFinalNodes;
    el.startNode = Nodes[el.startNode];
    for(int i = 1; i <= nrFinalNodes; i++)
    {
        int x;
        cin >> x;
        el.finalNodes.push_back(Nodes[x]);
    }
    cin >> nrEdges;
    char car;
    int node1, node2;
    for(int i = 1; i <= nrEdges; i++)
    {
        cin >> node1 >> car >> node2;
        el.Edges[Nodes[node1]].push_back(make_pair(Nodes[node2], car));
    }
    return cin;
}

ostream& operator << (ostream &cout, const DFA &el){
    cout << el.size() << "\n";
    for(auto it : el.finalNodes)
        cout << it << "\n";
    cout << "-------\n";
    cout << el.startNode << "\n";
    for(int i = 0;  i < el.size(); i++)
        for(auto it : el.Edges[i])
            cout << i << " " << it.second << " " << it.first << "\n";
    return cout;
}

DFA complementDfa(const DFA &el) {
    DFA ans = el;
    vector <bool> isFinal(el.size());
    for(auto it : el.finalNodes)
        isFinal[it] = true;
    ans.finalNodes.clear();
    for(int i = 0; i < ans.size(); i++)
        if(!isFinal[i])
            ans.finalNodes.push_back(i);
    return ans;
}

DFA intersectDfa(const DFA &A, const DFA &B){
    DFA ans(A.size() * B.size());

    vector <bool> isFinalA(A.size());
    for(auto it : A.finalNodes)
        isFinalA[it] = true;

    vector <bool> isFinalB(B.size());
    for(auto it : B.finalNodes)
        isFinalB[it] = true;
    vector <bool> seen(A.size() * B.size());

    queue < pair<int, int> > Q;
    Q.push(make_pair(A.startNode, B.startNode));
    while(!Q.empty())
    {
        pair <int, int> node = Q.front();
        Q.pop();
        for(int i = 0; i < 26; i++) /// alfabet
        {
            int nodA = A.findNode(node.first, char('a' + i));
            int nodB = B.findNode(node.second, char('a' + i));
            if(nodA == -1 && nodB == -1)
                continue;
            if(nodA == -1)
                nodA = node.first;
            if(nodB == -1)
                nodB = node.second;
            pair <int, int> node1 = make_pair(nodA, nodB);
            ans.Edges[node.first * A.size() + node.second].push_back(make_pair(node1.first * A.size() + node1.second, char('a' + i)));
            if(!seen[node1.first * A.size() + node1.second])
            {
                Q.push(node1);
                seen[node1.first * A.size() + node1.second] = true;
            }
        }
    }
    ans.startNode = A.startNode * A.size() + B.startNode;
    for(int i = 0; i < A.size(); i++)
        for(int j = 0; j < B.size(); j++)
            if(isFinalA[i] && isFinalB[j])
                ans.finalNodes.push_back(A.size() * i + j);
    return ans;
}

bool isVoid(const DFA &el){
    vector <bool> isFinal(el.size());
    vector <bool> seen(el.size());
    for(auto it : el.finalNodes)
        isFinal[it] = true;
    queue <int> Q;
    Q.push(el.startNode);
    while(!Q.empty())
    {
        int node = Q.front();
        if(isFinal[node])
            return false;
        Q.pop();
        for(auto it : el.Edges[node])
            if(!seen[it.first])
            {
                Q.push(it.first);
                seen[it.first] = 1;
            }
    }
    return true;
}

int main()
{
    DFA A, B;
    ifstream fin("date.in");
    ofstream fout("date.out");
    fin >> A >> B;
    if(isVoid(intersectDfa(A, complementDfa(B))) && isVoid(intersectDfa(B, complementDfa(A))))
        fout << "YES\n";
    else
        fout << "NO\n";

    return 0;
}
