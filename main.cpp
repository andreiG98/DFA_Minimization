#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

using namespace std;

class automat
{
    int q;//card stari
    int* Q;//starile
    int a;//card alfabet
    char* A;//alfabet
    int t;//card tranzitii
    int** T;//tranzitii
    int nf;//card stari finale
    int *F;//starile finale
    int si;//int initiala
public:
    automat(char*);
    void minimizareAFD();
    int cautare(vector<vector <int> >,int,int);
};

automat::automat(char* fisier)
{
    ifstream f(fisier);
    f>>q;
    Q=new int[q];
    int i,j;
    for(i=0; i<q; i++)
    {
        f>>Q[i];
    }
    f>>a;
    A=new char[a];
    for(i=0; i<a; i++)
        f>>A[i];
    f>>t;
    T=new int*[t];
    for(i=0; i<q; i++)
    {
        T[i]=new int[a];
        for(j=0; j<a; j++)
        {
            T[i][j]=-1;
        }
    }
    int x,z;
    char y;
    for(i=0; i<t; i++)
    {
        f>>x>>y>>z;
        T[x][y-'a']=z;
    }
    for(i=0; i<q; i++)
    {
        for(j=0; j<a; j++)
        {
            if(T[i][j]==-1)
                T[i][j]=q+1;
        }
    }
    f>>si;
    f>>nf;
    F=new int[nf];
    for(i=0; i<nf; i++)
        f>>F[i];
    f.close();
}

int automat::cautare(vector<vector <int> >St,int i,int k)
{
    int aux=St[i][0];
    int elem=T[aux][k];
    int l,m;
    for(l=0; l<St.size(); l++)
    {
        for(m=0; m<St[l].size(); m++)
        {
            if(elem == St[l][m])
            {
                return l;
            }
        }
    }
}

void automat::minimizareAFD()
{
    int i,j,k;
    int ok;
    int MatrStari[q][q];
    int aux;
    for(i=1; i<q; i++)
    {
        for(j=0; j<i; j++)
        {
            ok=1;
            for(k=0; k<nf; k++)
            {
                if(i==F[k] || j==F[k])  //daca una din stari e finala,marchez casuta cu 1
                    ok=0;
            }
            if(ok==1)
                MatrStari[i][j]=0;
            else
                MatrStari[i][j]=1;
        }
    }
    int modif=1;
    int sc1,sc2;
    while(modif == 1)
    {
        modif=0;
        for(i=1; i<q; i++)
        {
            for(j=0; j<i; j++)
            {
                if(MatrStari[i][j] == 0)
                {
                    for(k=0; k<a; k++)
                    {
                        sc1=T[i][k];
                        sc2=T[j][k];
                        if(sc1<sc2)
                        {
                            int aux=sc1;
                            sc1=sc2;
                            sc2=aux;
                        }
                        if(MatrStari[sc1][sc2]==1)
                        {
                            MatrStari[i][j]=1;
                            modif=1;
                        }
                    }
                }
            }
        }
    }
    vector<vector<int> >St;
    int marcat[q];
    for(i=0; i<q; i++)
        marcat[i]=0;
    for(i=1; i<q; i++)
    {
        for(j=0; j<i; j++)
        {
            if(MatrStari[i][j] == 0)
            {
                vector<int>s1;
                s1.push_back(j);
                s1.push_back(i);
                St.push_back(s1);
                marcat[i]=1;
                marcat[j]=1;
            }
        }
    }
    for(i=0; i<q; i++)
    {
        if(marcat[i]==0)
        {
            vector<int>s1;
            s1.push_back(i);
            St.push_back(s1);
            marcat[i]=1;
        }
    }
    ofstream g("AFD.out");
    g<<St.size()<<endl;
    for(i=0; i<St.size(); i++)
    {
        for(j=0; j<St[i].size(); j++)
        {
            g<<St[i][j];
        }
        g<<' ';
    }
    g<<endl;
    g<<a<<endl;
    for(i=0;i<a;i++)
        g<<A[i]<<' ';
    g<<endl;
    g<<2*St.size()<<endl;
    int l;
    for(i=0; i<St.size(); i++)
    {
        for(k=0; k<a; k++)
        {
            for(j=0; j<St[i].size(); j++)
            {
                g<<St[i][j];
            }
            g<<' '<<A[k]<<' ';
            int linie=cautare(St,i,k);
            for(l=0;l<St[linie].size();l++)
            {
                g<<St[linie][l];
            }
            g<<endl;
        }
    }
    g<<si<<endl;
    g<<nf<<endl;
    for(i=0; i<nf; i++)
    {
        g<<F[i];
    }
    g<<endl;
    g.close();
}

int main()
{
    automat AFD("AFD.in");
    AFD.minimizareAFD();
    return 0;
}
