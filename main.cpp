/*
    PROJET : Fichier mat.txt qui contient les informations qui concernent une matrice
    nbL nbC contenu

    A l'initialisation il y aura une création de deux matrices creuses
    A partir de deux fichiers textes 

    Les fonctions seront :
    1 - L'affichage 
    2 - Somme
    3 - Produit 
    4 - Puissance 
    5 - Sigma (de M a la puiss i)

    
    Ebauche du pseudo-code de puissance
    copier(M,S);
    pour i=0 a p-1 faire
        Produit(S,M,R);
        liberer(S);
        copier(R,S);
        liberer(R);
    fpour

*/

#include<iostream>
#include<vector>
#include<fstream>
#include<string>

using namespace std;


struct Cellule
{
    int i, j, info;
    Cellule* suivL;
    Cellule* suivC;
};

typedef Cellule* PCellule;

struct Mat
{
    vector<Cellule*> MH, MHq, MV, MVq; 
    int nbL=0, nbC=0;
};

typedef Mat* PMat;

void init_mat(PMat &mat, int nbL, int nbC)
{
        int i;
        for (i=0;i<nbL;i++) 
        {
            mat->MV.push_back(NULL);
            mat->MVq.push_back(NULL);
        }
        for (i=0;i<nbC;i++)
        {
            mat->MH.push_back(NULL);
            mat->MHq.push_back(NULL);
        }
}

void load_ffile(PMat &mat, string name)
{
    PCellule cell;
    int elem, nbL, nbC;
    ifstream file(name, ios::in);
    if (!file) cerr << "Ouverture du fichier impossible";
    else 
    {
        file >> nbL >> nbC;
        mat->nbC = nbC;
        mat->nbL = nbL;
        init_mat(mat, nbL, nbC);
        for (int i=0;i<nbL;i++)
        {
            for (int j=0;j<nbC;j++) 
            {
                file >> elem;
                if (elem != 0)
                {
                    cell = new Cellule;
                    cell->i = i;
                    cell->j = j;
                    cell->info = elem;
                    cell->suivC = NULL;
                    cell->suivL = NULL;
                    // First case
                    if (mat->MV[i] == NULL) mat->MV[i] = cell;
                    else mat->MVq[i]->suivC = cell;

                    mat->MVq[i] = cell;

                    if (mat->MH[j] == NULL) mat->MH[j] = cell;
                    else mat->MHq[j]->suivL = cell;

                    mat->MHq[j] = cell;
                }
            }
        }
        file.close();
    }
}
void display_mat(PMat mat)
{
    for (int i=0;i<mat->nbL;i++)
    {
        PCellule cell = mat->MV[i];
        while (cell != NULL)
        {
            cout << cell->info;
            cout << " ";
            cell = cell->suivC;
        }        
        cout << endl;
    }
} 
void AjouterM(Mat* &m, int i, int j, int info)
{
    PCellule cell = new Cellule;
    cell->i = i;
    cell->j = j;
    cell->info = info;

    if (m->MH[i] == NULL) 
    {
        m->MH[i] = cell;
        m->MHq[i] = cell;
    } 
    else
    {
        PCellule p = m->MH[i];
        while (p->suivL != NULL && p->suivL->j < cell->j) p = p->suivL;
        
        if (p->j == cell->j) p->info += cell->info;
        else 
        {
            cell->suivL = p->suivL;
            p->suivL = cell;
        }
    }

    if (m->MV[j] == NULL) 
    {
        m->MV[j] = cell;
        m->MVq[j] = cell;
    } 
    else
    {
        PCellule p = m->MV[j];
        while (p->suivC != NULL && p->suivC->i < cell->i) p = p->suivC;
        
        if (p->i == cell->i) p->info += cell->info;
        else 
        {
            cell->suivC = p->suivC;
            p->suivC = cell;
        }
    }
}

void copier(Mat* mat, Mat* &copie)
{
    for (int i=0;i<mat->nbL;i++)
    {
        PCellule p = mat->MH[i];
        while (p != NULL)
        {
            PCellule cell = new Cellule;
            cell->i = p->i;
            cell->j = p->j;
            cell->info = p->info;
            cell->suivC = NULL;
            cell->suivL = NULL;
            
            if (copie->MV[i] == NULL) copie->MV[i] = cell;
            else copie->MVq[i]->suivC = cell;

            copie->MVq[i] = cell;

            if (copie->MH[cell->j] == NULL) copie->MH[cell->j] = cell;
            else copie->MHq[cell->j]->suivL = cell;

            copie->MHq[cell->j] = cell; 
            p = p->suivC;       
        }
    }  
}

Mat* somme_mat(Mat* m1, Mat* m2)
{
    if (m1->nbL != m2->nbL || m1->nbC != m2->nbC) cout << "Matrice incompatible";
    else
    {
        Mat* m3 = new Mat;
        m3->nbL = m1->nbL;
        m3->nbC = m1->nbC;
        init_mat(m3, m3->nbL, m3->nbC);

        PCellule c, l;
        copier(m1, m3);
        return m3;
    }
}

Mat* prod(Mat* m1, Mat* m2) 
{
    if (m1->nbC != m2->nbL) cout << "Matrice incompatible";
    else
    {
        Mat* m3;
        m3->nbL = m1->nbL;
        m3->nbC = m2->nbC;
        init_mat(m3, m3->nbL, m3->nbC);

        PCellule c, l;
        for (int i=0;i<m1->nbL;i++)
        {
            for (int j=0;j<m2->nbC;j++) {
            int somme=0;
            l=m1->MH[j];
            c=m2->MV[i];
            while (c != NULL && j != NULL) 
            {
                if (c->j == l->i) {
                    somme += c->info*l->info;
                    c = c->suivC;
                    l = l->suivL;
                }
                else if (c->j > l->i) l = l->suivL;
                else if (c->j < l->i) c = c->suivC;
            }

            if (somme != 0) AjouterM(m3, i, j, somme);
            }
        }
    }
}

int main() 
{
    PMat mat2 = new Mat;
    load_ffile(mat2, "mat2.txt");

    PMat mat = new Mat;
    load_ffile(mat, "mat.txt");

    cout << "\n SOMME :\n";
    PMat smat = new Mat;
    smat = somme_mat(mat, mat2);
    display_mat(smat);
    return 0;
}