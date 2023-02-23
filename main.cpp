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
        mat->nbL = nbL;
        mat->nbC = nbC;
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
            cout << "\t|";
            cell = cell->suivC;
        }        
        cout << endl;
    }
} 

// Produit de deux matrices
void NAjouterM(Mat* &m, int i, int j, int x)
{
    PCellule p = new Cellule;
    p->info = x;
    p->i = i;
    p->j = j;
    p->suivC = NULL;
    p->suivL = NULL;
    if (m->MH[j] == NULL) 
    {
        m->MH[j] = p;
        m->MHq[j] = p;
    } 
    else
    {
        m->MHq[j]->suivL = p;
        m->MHq[j] = p;
    }

    if (m->MV[i] == NULL) 
    {
        m->MV[i] = p;
        m->MVq[i] = p;
    } 
    else
    {
        m->MVq[i]->suivC = p;
        m->MVq[i] = p;
    }
}

// Somme de deux matrices
void Somme(Mat* m1, Mat* m2, Mat* &m3)
{
    if (m1->nbL != m2->nbL || m1->nbC != m2->nbC) cout << "Matrice incompatible";
    else
    {
    int i, s;
    PCellule p1, p2, r;
    m3->nbL = m1->nbL;
    m3->nbC = m1->nbC;  
    init_mat(m3, m3->nbL, m3->nbC);
    for (i=0;i<m1->nbL;i++)
    {
        p1 = m1->MV[i];
        p2 = m2->MV[i];
        while (p1 != NULL && p2 != NULL)
        {
            if (p1->j == p2->j)
            {
                s = p1->info + p2->info;
                if (s != 0) NAjouterM(m3, i, p1->j, s);
                p1 = p1->suivC;
                p2 = p2->suivC;
            }
            else if (p1->j < p2->j)
            {
                NAjouterM(m3, i, p1->j, p1->info);
                p1 = p1->suivC;
            }
            else
            {
                NAjouterM(m3, i, p2->j, p2->info);
                p2 = p2->suivC;
            }
        }
        while (p1 != NULL)
        {
            NAjouterM(m3, i, p1->j, p1->info);
            p1 = p1->suivC;
        }
        while (p2 != NULL)
        {
            NAjouterM(m3, i, p2->j, p2->info);
            p2 = p2->suivC;
        }
    }
}
}
// Produit de deux matrices
void copier(Mat* mat, Mat* &copie)
{
    init_mat(copie, mat->nbL, mat->nbC);
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
// Produit de deux matrices
PMat produit_matrice(PMat mat1, PMat mat2)
{
    if (mat1->nbC != mat2->nbL) cout << "Matrice incompatible";
    else
    {
        PMat mat3 = new Mat;
        mat3->nbL = mat1->nbL;
        mat3->nbC = mat2->nbC;
        init_mat(mat3, mat3->nbL, mat3->nbC);

        PCellule cell1, cell2;
        for (int i=0;i<mat1->nbL;i++)
        {
            for (int j=0;j<mat2->nbC;j++) {
            int somme=0;
            cell1=mat1->MH[j];
            cell2=mat2->MV[i];
            while (cell2 != NULL && cell1 != NULL) 
            {
                if (cell2->j == cell1->i) {
                    somme += cell2->info*cell1->info;
                    cell2 = cell2->suivC;
                    cell1 = cell1->suivL;
                }
                else if (cell2->j > cell1->i) cell1 = cell1->suivL;
                else if (cell2->j < cell1->i) cell2 = cell2->suivC;
            }

            if (somme != 0) NAjouterM(mat3, i, j, somme);
            }
        }
        
        return mat3;
    }
    return NULL;
}
//Fonction qui libère la mémoire d'une matrice
void liberer_mat(PMat mat)
{
    for (int i=0;i<mat->nbL;i++)
    {
        PCellule p = mat->MV[i];
        while (p != NULL)
        {
            PCellule cell = p;
            p = p->suivC;
            delete cell;
        }
    }
    delete mat;
}

//Fonction qui calcule la puissance d'une matrice
PMat puissance_matrice(PMat mat, int n)
{
    PMat mat2 = new Mat;
    copier(mat, mat2);
    for (int i=0;i<n-1;i++)
    {
        PMat mat3 = produit_matrice(mat, mat2);
        liberer_mat(mat2);
        mat2 = mat3;
    }
    return mat2;
}

//Fonction qui calcule la matrice identité
PMat matrice_identite(int n)
{
    PMat mat = new Mat;
    init_mat(mat, n, n);
    for (int i=0;i<n;i++)
    {
        NAjouterM(mat, i, i, 1);
    }
    return mat;
}

//Fonction qui calcule la somme de deux matrices
PMat sigma_matrice(PMat mat, int n)
{
    
    PMat mat2 = new Mat;
    init_mat(mat2, mat->nbL, mat->nbC);
    copier(mat, mat2);
    for (int i=0;i<n-1;i++)
    {
        if (i == 0) mat2 = matrice_identite(mat->nbL);
        else
        {
            PMat mat3 = produit_matrice(mat, mat2);
            PMat mat4 = new Mat;
            Somme(mat2, mat3, mat4);
            liberer_mat(mat2);
            liberer_mat(mat3);
            mat2 = mat4;
        }
    }
    return mat2;
}


int main() 
{
    PMat mat2 = new Mat;
    load_ffile(mat2, "mat2.txt");

    PMat mat = new Mat;
    load_ffile(mat, "mat.txt");

    PMat smat = new Mat;
    Somme(mat, mat2, smat);

    cout << "\n SOMME :\n";

    display_mat(smat);

    PMat prod = new Mat;
    prod = produit_matrice(mat, mat2);

    cout << "\n PRODUIT :\n";
    display_mat(prod);

    PMat puiss = new Mat;
    puiss = puissance_matrice(mat, 3);

    cout << "\n PUISSANCE :\n";
    display_mat(puiss);

    PMat sigma = new Mat;
    sigma = sigma_matrice(mat, 3);

    cout << "\n SIGMA :\n";
    display_mat(sigma);
    
    liberer_mat(mat);
    liberer_mat(mat2);
    liberer_mat(smat);
    liberer_mat(prod);
    liberer_mat(puiss);
    liberer_mat(sigma);
    return 0;
}