#include<Windows.h>
#include<stdio.h>
#include<Commctrl.h>
#include<conio.h>
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include <fstream>
#include <Shlobj.h>
#include"resource.h"

#pragma comment(lib, "shell32.lib")

SYSTEMTIME st;

LRESULT CALLBACK MENU(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK VETS(HWND hWnd, UINT mes, WPARAM Wparam, LPARAM Param);

LRESULT CALLBACK CITAS(HWND cit, UINT mas, WPARAM paw, LPARAM pam);

LRESULT CALLBACK EDICION(HWND edi, UINT msg, WPARAM paw, LPARAM pam);

LRESULT CALLBACK AGENDA(HWND agen, UINT msg, WPARAM wParam, LPARAM lParam);

char especies[5][8] = { "Perro", "Gato", "Conejo", "Pez", "Ave" };

char estatu[3][10] = { "Pendiente","Cancelada","Efectuada" };

int manguito = 0;

//imagen
HBITMAP imagen = NULL;

HBITMAP Getimagen()
{
    return imagen;
}


//auxiliares para los vets
char wasa[20] ;
char ced[20];
char clav[20];
char contras[20];
char editclave[20];
char edito[20];

char iniclav[20];
char inicontra[20];

char cis[20];
char icos[20];

char doco[20];

struct vetes
{
    vetes* ant;
    char vet[20];
    char ced[20];
    char clavee[20];
    char contra[20];
    vetes* sig;

};
vetes* inicio = 0;


struct cita
{
    cita* ante;
    SYSTEMTIME fecha;
    char dia[5];
    char mes[5];
    char anio[5];
    char hora[20];
    char nombrec[20];
    char nombrem[20];
    char cel[20];
    char especie[20];
    char motivo[20];
    char costo[20];
    char estatus[20];
    int ora;

    cita* sige;
};
cita* in = 0;


//auxiliares para las citas
SYSTEMTIME afecha;
char ahora[20];
char anombrec[20];
char anombrem[20];
char acel[20];
char aespecie[20];
char amotivo[20];
char acosto[20];
char aestatus[20];

SYSTEMTIME filti;

//auxiliar para editar
char pet[20];


HINSTANCE hInstance;
MSG mensaje;


HWND hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(101), NULL, MENU);

HWND hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(110), hwnd, VETS);
void nvet(char[], char[], char[], char[]);
void iniciarsesion(vetes*&, char[],char[]);
void find(vetes *&,char[]);

HWND cit = CreateDialog(hInstance, MAKEINTRESOURCE(112), hwnd, CITAS);
void ncita(SYSTEMTIME,char[], char[], char[], char[], char[], char[], char[],int);

HWND agen = CreateDialog(hInstance, MAKEINTRESOURCE(114), hwnd, CITAS);
void editi(SYSTEMTIME, char[], char[], char[], char[], char[], char[], char[],char[], int);
void found(cita*&, char[]);
void filtrar(SYSTEMTIME);
HWND edi = CreateDialog(hInstance, MAKEINTRESOURCE(121), agen, EDICION);

void savebinariosvets();
void savebinariocita();

void cargarbinariovets();
void cargarbinariocita();

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    cargarbinariovets();
    cargarbinariocita();

    hInstance = hInst;
   
    ShowWindow(hWnd, SW_SHOWDEFAULT);

    /* Bucle de mensajes: */
    while (TRUE == GetMessage(&mensaje, 0, 0, 0))
    {
        TranslateMessage(&mensaje);
        DispatchMessage(&mensaje);
    }

}

LRESULT CALLBACK MENU(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  
    switch (msg)                  /* manipulador del mensaje */
    {
    case WM_INITDIALOG:
    {

        SetDlgItemText(hwnd, 1055, doco);

        HBITMAP hBitmap = Getimagen();

        if (hBitmap != NULL) {
            HWND hwndStatic = GetDlgItem(hwnd, 1052);
            SendMessage(hwndStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
        }

    }

    break;

    case WM_COMMAND: {

        if (LOWORD(wParam) == 1001 && HIWORD(wParam) == BN_CLICKED)
        {
            hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(110), hwnd, VETS);
            ShowWindow(hWnd, SW_SHOWDEFAULT);
        }
 
        if (LOWORD(wParam) == 1002 && HIWORD(wParam) == BN_CLICKED)
        {
            SendDlgItemMessage(hwnd, 1004, DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
            cit = CreateDialog(hInstance, MAKEINTRESOURCE(112), hwnd, CITAS);
            ShowWindow(cit, SW_SHOWDEFAULT);
        }

        if (LOWORD(wParam) == 1003 && HIWORD(wParam) == BN_CLICKED)
        {
            agen = CreateDialog(hInstance, MAKEINTRESOURCE(114), hwnd, AGENDA);
            ShowWindow(agen, SW_SHOWDEFAULT);
        }


        if (LOWORD(wParam) == 1006)
        {
            savebinariosvets();
            savebinariocita();
            DestroyWindow(hwnd);

        }





    }break;

    case WM_CLOSE: {

        DestroyWindow(hwnd);
    }break;

    case WM_DESTROY: {
        /* env a un mensaje WM_QUIT a la cola de mensajes */
        PostQuitMessage(0);
    }break;


    }
    return 0;

}

char doctorfoto[1000];

LRESULT CALLBACK VETS(HWND hWnd, UINT mes, WPARAM Wparam, LPARAM Param)
{

    switch (mes)                  /* manipulador del mensaje */
    {


         case WM_INITDIALOG:
         {
             SetDlgItemText(hWnd, 1055, doco);

             HBITMAP hBitmap = Getimagen();

             if (hBitmap != NULL) {
                 HWND hwndStatic = GetDlgItem(hwnd, 1052);
                 SendMessage(hwndStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
             }

        vetes* aux = inicio;

        while (aux != NULL)
        {
            SendMessage(GetDlgItem(hWnd, 1023), LB_INSERTSTRING, 0, (LPARAM)"\n");
            SendMessage(GetDlgItem(hWnd, 1023), LB_INSERTSTRING, 0, (LPARAM)aux->contra);
            SendMessage(GetDlgItem(hWnd, 1023), LB_INSERTSTRING, 0, (LPARAM)aux->clavee);
            SendMessage(GetDlgItem(hWnd, 1023), LB_INSERTSTRING, 0, (LPARAM)aux->ced);
            SendMessage(GetDlgItem(hWnd, 1023), LB_INSERTSTRING, 0, (LPARAM)aux->vet);

            aux = aux->sig;
        }
        delete aux;
    }
         break;
        case WM_COMMAND:
         {

        if (LOWORD(Wparam) == 1025)
        {


            GetDlgItemText(hWnd, 1018, wasa, 20); //nombre del vet

            GetDlgItemText(hWnd, 1019, ced, 20); //cedula

            GetDlgItemText(hWnd, 1020, clav, 20);  //clav

            GetDlgItemText(hWnd, 1021, contras, 20);  //contrase a


            //solo letras pal vet
            for (int i = 0; i < (strlen(wasa)); i++)
            {
                if (isalpha(wasa[i]) == FALSE)
                {
                    manguito = 1;
                }
                else
                {
                    manguito = 0;
                }
            }
            if (manguito == 0)
            {
                nvet(wasa, ced, clav, contras);
                SetDlgItemText(hWnd, 1018, "");
                SetDlgItemText(hWnd, 1019, "");
                SetDlgItemText(hWnd, 1020, "");
                SetDlgItemText(hWnd, 1021, "");

                SendMessage(GetDlgItem(hWnd, 1023), LB_INSERTSTRING, 0, (LPARAM)"\n");
                SendMessage(GetDlgItem(hWnd, 1023), LB_INSERTSTRING, 0, (LPARAM)contras);
                SendMessage(GetDlgItem(hWnd, 1023), LB_INSERTSTRING, 0, (LPARAM)clav);
                SendMessage(GetDlgItem(hWnd, 1023), LB_INSERTSTRING, 0, (LPARAM)ced);
                SendMessage(GetDlgItem(hWnd, 1023), LB_INSERTSTRING, 0, (LPARAM)wasa);

            }
            else {
                int  awass1 = MessageBox(hWnd, "Ingrese letras", "invalido", MB_OK);
            }
        }
        if (LOWORD(Wparam) == 1038)
        {
            GetDlgItemText(hWnd, 1024, editclave, 20);
            find(inicio, editclave);
            SetDlgItemText(hWnd, 1018, wasa);
            SetDlgItemText(hWnd, 1019, ced);
            SetDlgItemText(hWnd, 1020, clav);
            SetDlgItemText(hWnd, 1021, contras);
            SetDlgItemText(hWnd, 1024, "");
        }
        if (LOWORD(Wparam) == 1040)
        {
            SendMessage(GetDlgItem(hWnd, 1023), LB_RESETCONTENT, 0, 0);
            vetes* aux = inicio;
            while (aux != NULL)
            {
                SendMessage(GetDlgItem(hWnd, 1023), LB_INSERTSTRING, 0, (LPARAM)"\n");
                SendMessage(GetDlgItem(hWnd, 1023), LB_INSERTSTRING, 0, (LPARAM)aux->contra);
                SendMessage(GetDlgItem(hWnd, 1023), LB_INSERTSTRING, 0, (LPARAM)aux->clavee);
                SendMessage(GetDlgItem(hWnd, 1023), LB_INSERTSTRING, 0, (LPARAM)aux->ced);
                SendMessage(GetDlgItem(hWnd, 1023), LB_INSERTSTRING, 0, (LPARAM)aux->vet);
                aux = aux->sig;
            }
            delete aux;
        }


        if (LOWORD(Wparam) == 1053)
        {
            GetDlgItemText(hWnd, 1050, iniclav, 20);
            GetDlgItemText(hWnd, 1051, inicontra, 20);
            iniciarsesion(inicio, iniclav, inicontra);
            SetDlgItemText(hWnd, 1050, "");
            SetDlgItemText(hWnd, 1051, "");
            SetDlgItemText(hwnd, 1055, doco);

            HBITMAP hBitmap = Getimagen();

            if (hBitmap != NULL) {
                HWND hwndStatic = GetDlgItem(hwnd, 1052);
                SendMessage(hwndStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
            }
           
        }

        if (LOWORD(Wparam) == 4)
        {
            OPENFILENAME ofn;
            char szFileName[MAX_PATH] = "";

            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFile = szFileName;
            ofn.nMaxFile = sizeof(szFileName);
            ofn.lpstrFilter = "Archivos de Imagen\0*.bmp;\0";
            ofn.nFilterIndex = 1;
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

            // carga de mapa de bits
            HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);

            if (GetOpenFileName(&ofn)) {
                HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);

                if (hBitmap != NULL) {
                    HWND hwndStatic = GetDlgItem(hWnd, 1052);
                    SendMessage(hwndStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);

                    imagen = hBitmap;  // Guarda el identificador del mapa de bits en la variable global
                    strcpy_s(doctorfoto, szFileName);

                    MessageBox(hWnd, "Imagen cargada exitosamente", "CARGA EXITOSA", MB_OK);
                }
                else {
                    MessageBox(hWnd, "Error al cargar la imagen", "ERROR", MB_OK | MB_ICONERROR);
                }
            }

            SetDlgItemText(hwnd, 1055, doco);

        }


         }
         break;
          case WM_CLOSE: {
        DestroyWindow(hWnd);
    }
          break;
    }

    return 0;
}

void nvet( char wasa[], char ced[], char clav[], char contras[])
{
    vetes* ultimo = inicio;
    vetes* ayuda = NULL;

    vetes* nuevo = new vetes;
    strcpy_s(nuevo->vet, wasa);
    strcpy_s(nuevo->ced, ced);
    strcpy_s(nuevo->clavee, clav);
    strcpy_s(nuevo->contra, contras);

    nuevo->sig = NULL;

    vetes* aux = inicio;
    
    
    while (ultimo != NULL && ultimo->sig != NULL)
    {
        ayuda = ultimo;
        ayuda = ultimo->ant;
        ultimo = ultimo->sig;

    }

    if (ultimo != NULL)
    {
        ultimo->sig = nuevo;

    }
    else
    {
        inicio = nuevo;
        inicio->ant = NULL;
    }
}
void find(vetes*& inicio, char editclave[])
{
    strcpy_s(edito, editclave);

        vetes* editar;
        vetes* anterior = NULL;

        editar = inicio;

        while (editar != NULL && strcmp(editar->clavee, edito) != 0)
        {
            anterior = editar;
            editar = editar->sig;
        }

        if (editar == NULL)
        {
            DestroyWindow(hWnd);
        }
        else if(anterior==NULL)
        {
            inicio = inicio->sig;
            strcpy_s(wasa, editar->vet);
            strcpy_s(ced, editar->ced);
            strcpy_s(clav, editar->clavee);
            strcpy_s(contras, editar->contra);
            delete editar;
        }
        else
        {
            anterior->sig = editar->sig;
            strcpy_s(wasa, editar->vet);
            strcpy_s(ced, editar->ced);
            strcpy_s(clav, editar->clavee);
            strcpy_s(contras, editar->contra);
            delete editar;
        }

}
void iniciarsesion(vetes*& inicio, char iniclav[], char incontra[])
{

    strcpy_s(cis,iniclav);
    strcpy_s(icos, inicontra);

    vetes* editar;
    vetes* anterior = NULL;

    editar = inicio;

    while (editar != NULL   && strcmp(editar->clavee, cis) != 0)
    {
        anterior = editar;
        editar = editar->sig;
    }

    if (editar == NULL)
    {
        int  awass1 = MessageBox(hWnd, "uno o ambos datos no coinciden ", "invalido", MB_OK);
       
    }
    else  if (strcmp(editar->contra, icos) != 0)
    {
        int  awass1 = MessageBox(hWnd, "uno o ambos datos no coinciden ", "invalido", MB_OK);
    }
    else if (anterior == NULL)
    {
        int  awass1 = MessageBox(hWnd, "Bienvenidos ", "inicio de sesion correcto", MB_OK);
        strcpy_s(doco, editar->vet);
        SetDlgItemText(hWnd, 1055, doco);
        DestroyWindow(hWnd);
        ShowWindow(hwnd, SW_SHOWDEFAULT);
    }
    else
    {
        int  awass1 = MessageBox(hWnd, "Bienvenidos", "inicio de sesion correcto", MB_OK);
        strcpy_s(doco, editar->vet);
        SetDlgItemText(hWnd, 1055, doco);
        DestroyWindow(hWnd);
         ShowWindow(hwnd, SW_SHOWDEFAULT);
    }

}

LRESULT CALLBACK CITAS(HWND cit, UINT mas, WPARAM paw, LPARAM pam)
{
    switch (mas)                  /* manipulador del mensaje */
    {
        

        case WM_INITDIALOG:
        {
            SetDlgItemText(cit, 1055, doco);

            HBITMAP hBitmap = Getimagen();

            if (hBitmap != NULL) {
                HWND hwndStatic = GetDlgItem(cit, 1052);
                SendMessage(hwndStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
            }




        for (int a = 0; a < 5; a++) {
            SendDlgItemMessage(cit, 1045, CB_INSERTSTRING, a,(LPARAM)especies[a]);
         }
        }
        break;

    case WM_COMMAND: {
        if (LOWORD(paw) == 1036)
        {
            SendDlgItemMessage(cit, 1037, DTM_GETSYSTEMTIME, 0, (LPARAM)&afecha);
            GetDlgItemText(cit, 1035, ahora, 20);
            GetDlgItemText(cit, 1027, anombrec, 20);
            GetDlgItemText(cit, 1029, anombrem, 20);
            GetDlgItemText(cit, 1033, acel, 20);
            GetDlgItemText(cit, 1045, aespecie, 20);
            GetDlgItemText(cit, 1030, amotivo, 20);
            GetDlgItemText(cit, 1031, acosto, 20);
            //telefono
            int pikachu = 0;
            int mia = strlen(acel);    
            if (mia != 8 && mia != 10 && mia != 12)
            {
                int  res13 = MessageBox(cit, "Solo 8, 10, o 12 numeros", "invalido", MB_OK);
                pikachu = 1;
            }
            else
            {
                pikachu = 0;
            }

            //costo
            int max = 1;
            int mazapan = strlen(acosto);
            int morita = 0; 
            int mau = 0;
            int nunu = 0;
            char punto[] = ".";

            for (int i = 0; i < mazapan; i++)
            {
                
                if (isdigit(acosto[i]) == FALSE)
                {
                    max = 0;
                    if ( acosto[i] != punto[i])
                    {
                        mau = mau + 1;
                    }
                }
                else
                {
                    max = 1;
                }
            }

            //nombre
            int mane = 0;
            for (int i = 0; i < (strlen(anombrec)); i++)
            {
                if (isalpha(anombrec[i]) == FALSE)
                {
                    mane = 1;
                }
                else
                {
                    mane = 0;
                }
            }


           
            if (max == 0)
            {
                int  res12 = MessageBox(cit, "Ingrese numeros", "invalido", MB_OK);
            }
            if (mau != 1)
            {
                int  res14 = MessageBox(cit, "Solo se puede un punto", "invalido", MB_OK);
            }
           
            //fechas
            int awas = 0;
            if (afecha.wYear < st.wYear)
            {
                int  res = MessageBox(cit, "Ingrese una fecha posterior", "invalido", MB_OK);
                awas = 2;
            }
            else 
            {
                if (afecha.wMonth < st.wMonth )
                {
                    int  res = MessageBox(cit, "Ingrese una fecha posterior", "invalido", MB_OK);
                    awas = 1;
                } 
                else if (afecha.wMonth > st.wMonth )
                {
                    awas = 0;
                }
                else
                {
                    if (afecha.wDay < st.wDay)
                    {
                        int  res = MessageBox(cit, "Ingrese una fecha posterior", "invalido", MB_OK);
                        awas = 3;
                    }
                    else
                    {
                        awas = 0;
                    }
                }

            }



            int monos = 0;
            int horita = atoi(ahora);
            if (afecha.wDay == st.wDay)
            {
                if (horita < st.wHour || horita == st.wHour)
                {
                    monos = 1;
                    int  res = MessageBox(cit, "Ingrese una hora posterior", "invalido", MB_OK);
                }

            }

            if (max==1 && mau == 1 && awas == 0 && pikachu ==0 && monos==0 && mane==0)
            {
                ncita(afecha, ahora, anombrec, anombrem, acel, aespecie, amotivo, acosto,horita);
                SetDlgItemText(cit, 1035, "");
                SetDlgItemText(cit, 1027, "");
                SetDlgItemText(cit, 1029, "");
                SetDlgItemText(cit, 1030, "");
                SetDlgItemText(cit, 1031, "");
                SetDlgItemText(cit, 1033, "");
                SetDlgItemText(cit, 1045, "");
            }



        }

    }break;

    case WM_CLOSE: {
        DestroyWindow(cit);
    }break;

    }

    return 0;
}
void ncita(SYSTEMTIME afecha, char ahora[], char anombrec[], char anombrem[], char acel[], char aespecie[], char amotivo[], char acosto[], int horita)
{
    cita* ultimo = in;
    cita* nuevo = new cita;
    int owo = 0;

    nuevo->fecha = afecha;
    strcpy_s(nuevo->costo, acosto);
    strcpy_s(nuevo->motivo, amotivo);
    strcpy_s(nuevo->especie, aespecie);
    strcpy_s(nuevo->cel, acel);
    strcpy_s(nuevo->nombrem, anombrem);
    strcpy_s(nuevo->nombrec, anombrec);
    strcpy_s(nuevo->hora, ahora);
    _itoa_s(nuevo->fecha.wYear,nuevo->anio, 10);
    _itoa_s(nuevo->fecha.wMonth, nuevo->mes, 10);
    _itoa_s(nuevo->fecha.wDay, nuevo->dia, 10);
    strcpy_s(nuevo->estatus, "Pendiente");
    nuevo->ora = horita;


    cita* omega = in;
    cita* alfa = NULL;

    while (omega != NULL && omega->fecha.wYear > afecha.wYear)
    {
        alfa = omega;
        omega = omega->sige;
        owo = 1;
    }
    if (owo == 0 || omega !=NULL)
    {
        while (omega != NULL && omega->fecha.wMonth > afecha.wMonth)
        {
            alfa = omega;
            omega = omega->sige;
            owo = 2;
        }
    }
    if (owo == 0|| omega !=NULL )
    {
        while (omega != NULL && omega->fecha.wDay > afecha.wDay)
        {
            alfa = omega;
            omega = omega->sige;
            owo = 1;
        }
    }
    if (owo == 0 || omega != NULL)
    {
        while (omega != NULL && omega->ora > horita)
        {
            alfa = omega;
            omega = omega->sige;
            owo = 2;
        }

    }
    if (in == omega)
    {
        in = nuevo;
    }
    else
    {
        alfa->sige = nuevo;
    }
    nuevo->sige=omega;
}

LRESULT CALLBACK AGENDA(HWND agen, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)                  /* manipulador del mensaje */
    {

    case WM_INITDIALOG:
    {
        SetDlgItemText(agen, 1055, doco);

        HBITMAP hBitmap = Getimagen();

        if (hBitmap != NULL) {
            HWND hwndStatic = GetDlgItem(agen, 1052);
            SendMessage(hwndStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
        }




        cita* aux = in;
        while (aux != NULL)
        {
            SendMessage(GetDlgItem(agen, 1027), LB_INSERTSTRING, 0, (LPARAM)"\n");
            SendMessage(GetDlgItem(agen, 1027), LB_INSERTSTRING, 0, (LPARAM)aux->estatus);
            SendMessage(GetDlgItem(agen, 1027), LB_INSERTSTRING, 0, (LPARAM)aux->costo);
            SendMessage(GetDlgItem(agen, 1027), LB_INSERTSTRING, 0, (LPARAM)aux->motivo);
            SendMessage(GetDlgItem(agen, 1027), LB_INSERTSTRING, 0, (LPARAM)aux->especie);
            SendMessage(GetDlgItem(agen, 1027), LB_INSERTSTRING, 0, (LPARAM)aux->cel);
            SendMessage(GetDlgItem(agen, 1027), LB_INSERTSTRING, 0, (LPARAM)aux->nombrem);
            SendMessage(GetDlgItem(agen, 1027), LB_INSERTSTRING, 0, (LPARAM)aux->nombrec);
            SendMessage(GetDlgItem(agen, 1027), LB_INSERTSTRING, 0, (LPARAM)aux->hora);
            SendMessage(GetDlgItem(agen, 1027), LB_INSERTSTRING, 0, (LPARAM)aux->dia);
            SendMessage(GetDlgItem(agen, 1027), LB_INSERTSTRING, 0, (LPARAM)aux->mes);
            SendMessage(GetDlgItem(agen, 1027), LB_INSERTSTRING, 0, (LPARAM)aux->anio);
            aux = aux->sige;
        }
        delete aux;
    }
    case WM_COMMAND: {

        if (LOWORD(wParam) == 1046)
        {
            GetDlgItemText(agen, 1026,pet ,20);
            found(in, pet);
            edi = CreateDialog(hInstance, MAKEINTRESOURCE(121), hwnd, EDICION);
            ShowWindow(edi, SW_SHOWDEFAULT);
            DestroyWindow(agen);
        }

        if (LOWORD(wParam) == 1058)
        {
            HWND wero = GetDlgItem(agen, 1028);
            SendMessage(wero, LB_RESETCONTENT, 0, 0);
            SendDlgItemMessage(agen, 1059, DTM_GETSYSTEMTIME, 0, (LPARAM)&filti);
            filtrar(filti);
        }

    }break;
    case WM_CLOSE: {
        DestroyWindow(agen);
    }break;

    }
    return 0;

}

void filtrar(SYSTEMTIME filti)
{
    cita* hayar;
    cita* anterior = NULL;

    hayar = in;

   

    while (hayar != NULL)
    {
        int awas = 0;
        
        if (hayar->fecha.wYear < filti.wYear)
        {
            awas = 2;
        }
        else
        {
            if (hayar->fecha.wMonth < filti.wMonth)
            {
                awas = 1;
            }
            else if (hayar->fecha.wMonth > filti.wMonth)
            {
                awas = 0;
            }
            else
            {
                if (hayar->fecha.wDay < filti.wDay)
                {
                    awas = 5;
                }
                else
                {
                    awas = 0;
                }
            }
        }

        if (awas == 0)
        {
            SendMessage(GetDlgItem(agen, 1028), LB_INSERTSTRING, 0, (LPARAM)"\n");
            SendMessage(GetDlgItem(agen, 1028), LB_INSERTSTRING, 0, (LPARAM)hayar->estatus);
            SendMessage(GetDlgItem(agen, 1028), LB_INSERTSTRING, 0, (LPARAM)hayar->costo);
            SendMessage(GetDlgItem(agen, 1028), LB_INSERTSTRING, 0, (LPARAM)hayar->motivo);
            SendMessage(GetDlgItem(agen, 1028), LB_INSERTSTRING, 0, (LPARAM)hayar->especie);
            SendMessage(GetDlgItem(agen, 1028), LB_INSERTSTRING, 0, (LPARAM)hayar->cel);
            SendMessage(GetDlgItem(agen, 1028), LB_INSERTSTRING, 0, (LPARAM)hayar->nombrem);
            SendMessage(GetDlgItem(agen, 1028), LB_INSERTSTRING, 0, (LPARAM)hayar->nombrec);
            SendMessage(GetDlgItem(agen, 1028), LB_INSERTSTRING, 0, (LPARAM)hayar->hora);
            SendMessage(GetDlgItem(agen, 1028), LB_INSERTSTRING, 0, (LPARAM)hayar->dia);
            SendMessage(GetDlgItem(agen, 1028), LB_INSERTSTRING, 0, (LPARAM)hayar->mes);
            SendMessage(GetDlgItem(agen, 1028), LB_INSERTSTRING, 0, (LPARAM)hayar->anio);

            anterior = hayar;
            hayar = hayar->sige;
        }
        else
        {
            anterior = hayar;
            hayar = hayar->sige;
        }
    }

}


void found(cita*& in, char [])
{
    strcpy_s(edito,pet );

    cita* editar;
    cita* anterior = NULL;

    editar = in;

    while (editar != NULL && strcmp(editar->nombrem, edito) != 0)
    {
        anterior = editar;
        editar = editar->sige;
    }

    if (editar == NULL)
    {
        DestroyWindow(cit);
    }
    else if (anterior == NULL)
    {
        in = in->sige;
        strcpy_s(ahora,editar->hora);
        strcpy_s(anombrec, editar->nombrec);
        strcpy_s(anombrem, editar->nombrem);
        strcpy_s(acel, editar->cel);
        strcpy_s(aespecie, editar->especie);
        strcpy_s(amotivo, editar->motivo);
        strcpy_s(acosto, editar->costo);
        strcpy_s(aestatus, editar->estatus);
        afecha.wYear =editar->fecha.wYear;
        afecha.wMonth = editar->fecha.wMonth;
        afecha.wDay = editar->fecha.wDay;
        delete (editar);
        return;
    }
    else
    {
        anterior->sige = editar->sige;
        strcpy_s(ahora, editar->hora);
        strcpy_s(anombrec, editar->nombrec);
        strcpy_s(anombrem, editar->nombrem);
        strcpy_s(acel, editar->cel);
        strcpy_s(aespecie, editar->especie);
        strcpy_s(amotivo, editar->motivo);
        strcpy_s(acosto, editar->costo);
        strcpy_s(aestatus, editar->estatus);
        afecha.wYear = editar->fecha.wYear;
        afecha.wMonth = editar->fecha.wMonth;
        afecha.wDay = editar->fecha.wDay;
        delete (editar);
        return;
    }

}

LRESULT CALLBACK EDICION(HWND edi, UINT msg, WPARAM paw, LPARAM pam)
{
    switch (msg)                  /* manipulador del mensaje */
    {


    case WM_INITDIALOG:
    {
        SetDlgItemText(edi, 1055, doco);

        HBITMAP hBitmap = Getimagen();

        if (hBitmap != NULL) {
            HWND hwndStatic = GetDlgItem(edi, 1052);
            SendMessage(hwndStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
        }

        SendDlgItemMessage(edi, 1048, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&afecha);

        SetDlgItemText(edi, 1035, ahora);
        SetDlgItemText(edi, 1027, anombrec);
        SetDlgItemText(edi, 1029, anombrem);
        SetDlgItemText(edi, 1030, amotivo);
        SetDlgItemText(edi, 1031, acosto);
        SetDlgItemText(edi, 1033, acel);
        SetDlgItemText(edi, 1045, aespecie);
        SetDlgItemText(edi, 1049, aestatus);

        for (int a = 0; a < 5; a++) {
            SendDlgItemMessage(edi, 1045, CB_INSERTSTRING, a, (LPARAM)especies[a]);
        }

        for (int a = 0; a < 3; a++) {
            SendDlgItemMessage(edi, 1049, CB_INSERTSTRING, a, (LPARAM)estatu[a]);
        }
        

    }
    break;

    case WM_COMMAND: {
        if (LOWORD(paw) == 1036)
        {
            SendDlgItemMessage(edi, 1048, DTM_GETSYSTEMTIME, 0, (LPARAM)&afecha);
            GetDlgItemText(edi, 1035, ahora, 20);
            GetDlgItemText(edi, 1027, anombrec, 20);
            GetDlgItemText(edi, 1029, anombrem, 20);
            GetDlgItemText(edi, 1033, acel, 20);
            GetDlgItemText(edi, 1045, aespecie, 20);
            GetDlgItemText(edi, 1030, amotivo, 20);
            GetDlgItemText(edi,1031, acosto, 20);
            GetDlgItemText(edi, 1049, aestatus,20);
            
            //telefono
            int pikachu = 0;
            int mia = strlen(acel);
            if (mia != 8 && mia != 10 && mia != 12)
            {
                int  res13 = MessageBox(cit, "Solo 8, 10, o 12 numeros", "invalido", MB_OK);
                pikachu = 1;
            }
            else
            {
                pikachu = 0;
            }

            //costo
            int max = 1;
            int mazapan = strlen(acosto);
            int morita = 0;
            int mau = 0;
            int nunu = 0;
            char punto[] = ".";

            for (int i = 0; i < mazapan; i++)
            {

                if (isdigit(acosto[i]) == FALSE)
                {
                    max = 0;
                    if (acosto[i] != punto[i])
                    {
                        mau = mau + 1;
                    }
                }
                else
                {
                    max = 1;
                }
            }


            if (max == 0)
            {
                int  res12 = MessageBox(edi, "Ingrese numeros", "invalido", MB_OK);
            }
            if (mau != 1)
            {
                int  res14 = MessageBox(edi, "Solo se puede un punto", "invalido", MB_OK);
            }

            //fechas
            int awas = 0;
            if (afecha.wYear < st.wYear)
            {
                int  res = MessageBox(edi, "Ingrese una fecha posterior", "invalido", MB_OK);
                awas = 2;
            }
            else
            {
                if (afecha.wMonth < st.wMonth)
                {
                    int  res = MessageBox(edi, "Ingrese una fecha posterior", "invalido", MB_OK);
                    awas = 1;
                }
                else if (afecha.wMonth > st.wMonth)
                {
                    awas = 0;
                }
                else
                {
                    if (afecha.wDay < st.wDay)
                    {
                        int  res = MessageBox(edi, "Ingrese una fecha posterior", "invalido", MB_OK);
                        awas = 3;
                    }
                    else
                    {
                        awas = 0;
                    }
                }

            }
            int monos = 0;
            int horita = atoi(ahora);
            if (afecha.wDay == st.wDay)
            {
                if (horita < st.wHour || horita == st.wHour)
                {
                    monos = 1;
                    int  res = MessageBox(cit, "Ingrese una hora posterior", "invalido", MB_OK);
                }
            }

            int mane = 0;
            for (int i = 0; i < (strlen(anombrec)); i++)
            {
                if (isalpha(anombrec[i]) == FALSE)
                {
                    mane = 1;
                }
                else
                {
                    mane = 0;
                }
            }


            if (max == 1 && mau == 1 && awas == 0 && pikachu == 0 && anombrec== 0)
            {
                editi(afecha, ahora, anombrec, anombrem, acel, aespecie, amotivo, acosto,aestatus,horita);
                SetDlgItemText(edi, 1035, "");
                SetDlgItemText(edi, 1027, "");
                SetDlgItemText(edi, 1029, "");
                SetDlgItemText(edi, 1030, "");
                SetDlgItemText(edi, 1031, "");
                SetDlgItemText(edi, 1033, "");
                SetDlgItemText(edi, 1045, "");
                SetDlgItemText(edi, 1049, "");
                DestroyWindow(edi);
            }



        }

        if (LOWORD(paw) == 1038)
        {
            DestroyWindow(edi);
        }

    }break;

    case WM_CLOSE: {
        DestroyWindow(edi);
    }break;

    }

    return 0;
}

void editi(SYSTEMTIME afecha, char ahora[], char anombrec[], char anombrem[], char acel[], char aespecie[], char amotivo[], char acosto[],char aestatus[],int horita)
{
    cita* ultimo = in;
    cita* nuevo = new cita;
    int owo = 0;

    nuevo->fecha = afecha;
    strcpy_s(nuevo->costo, acosto);
    strcpy_s(nuevo->motivo, amotivo);
    strcpy_s(nuevo->especie, aespecie);
    strcpy_s(nuevo->cel, acel);
    strcpy_s(nuevo->nombrem, anombrem);
    strcpy_s(nuevo->nombrec, anombrec);
    strcpy_s(nuevo->hora, ahora);
    _itoa_s(nuevo->fecha.wYear, nuevo->anio, 10);
    _itoa_s(nuevo->fecha.wMonth, nuevo->mes, 10);
    _itoa_s(nuevo->fecha.wDay, nuevo->dia, 10);
    strcpy_s(nuevo->estatus, aestatus);
    nuevo->ora = horita;

    cita* omega = in;
    cita* alfa = NULL;

    while (omega != NULL && omega->fecha.wYear > afecha.wYear)
    {
        alfa = omega;
        omega = omega->sige;
        owo = 1;
    }
    if (owo == 0 || omega != NULL)
    {
        while (omega != NULL && omega->fecha.wMonth > afecha.wMonth)
        {
            alfa = omega;
            omega = omega->sige;
            owo = 2;
        }
    }
    if (owo == 0 || omega != NULL)
    {
        while (omega != NULL && omega->fecha.wDay > afecha.wDay)
        {
            alfa = omega;
            omega = omega->sige;
            owo = 1;
        }
    }
    if (owo == 0 || omega != NULL)
    {
        while (omega != NULL && omega->ora > horita)
        {
            alfa = omega;
            omega = omega->sige;
            owo = 2;
        }

    }
    if (in == omega)
    {
        in = nuevo;
    }
    else
    {
        alfa->sige = nuevo;
    }
    nuevo->sige = omega;
}

void savebinariosvets()
{
    wchar_t documentosDir[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_MYDOCUMENTS, nullptr, 0, documentosDir)))
    {
        std::wstring documentosPath(documentosDir);
       
        std::wstring archivoPath = documentosPath + L"\\PIA_PROGRA_DYAC_JDFH\\vetes.bin";

        std::ofstream archivovets(archivoPath, std::ios::binary | std::ios::out);

        if (!archivovets.is_open())
        {
            MessageBox(NULL, "Error al guardar", "No se pudo", MB_OK | MB_ICONERROR);
            return;
        }

        vetes* este = inicio;
        while (este != nullptr)
        {
            archivovets.write(reinterpret_cast<const char*>(este), sizeof(vetes));
            este = este->sig;
        }

        archivovets.close();
    }
    else
    {
        MessageBox(NULL, "Error al obtener la ruta de documentos", "No se pudo", MB_OK | MB_ICONERROR);
    }
}
void savebinariocita()
{

    wchar_t documentosDir[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_MYDOCUMENTS, nullptr, 0, documentosDir)))
    {
        std::wstring documentosPath(documentosDir);

        std::wstring archivoPath = documentosPath + L"\\PIA_PROGRA_DYAC_JDFH\\citas.bin";

        std::ofstream archivocitas(archivoPath, std::ios::binary | std::ios::out);

        if (!archivocitas.is_open())
        {
            MessageBox(NULL, "Error al guardar", "No se pudo", MB_OK | MB_ICONERROR);
            return;
        }

        cita* este = in;
        while (este != nullptr)
        {
            archivocitas.write(reinterpret_cast<const char*>(este), sizeof(cita));
            este = este->sige;
        }

        archivocitas.close();
    }
    else
    {
        MessageBox(NULL, "Error al obtener la ruta de documentos", "No se pudo", MB_OK | MB_ICONERROR);
    }
}

void cargarbinariovets()
{
    wchar_t documentosDir[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_MYDOCUMENTS, nullptr, 0, documentosDir)))
    {
        // Convertir la ruta a un formato de cadena de C++
        std::wstring documentosPath(documentosDir);

        // Concatenar el nombre del archivo al directorio de documentos
        std::wstring archivoPath = documentosPath + L"\\PIA_PROGRA_DYAC_JDFH\\vetes.bin";

        // Abrir el archivo en modo binario para lectura
        std::ifstream archivovets(archivoPath, std::ios::binary | std::ios::in);

        if (!archivovets.is_open())
        {
            return;
        }

        while (inicio != nullptr)
        {
            vetes* temp = inicio;
            inicio = inicio->sig;
            delete temp;
        }

        vetes leido;
        while (archivovets.read(reinterpret_cast<char*>(&leido), sizeof(vetes)))
        {
            vetes* nuevovet = new vetes;
            memcpy(nuevovet, &leido, sizeof(vetes));

            nuevovet->sig = inicio;
            nuevovet->ant = nullptr;

            if (inicio != nullptr)
            {
                inicio->ant = nuevovet;
            }
            inicio = nuevovet;
        }

        archivovets.close();
    }
}

void cargarbinariocita()
{
    wchar_t documentosDir[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_MYDOCUMENTS, nullptr, 0, documentosDir)))
    {
        // Convertir la ruta a un formato de cadena de C++
        std::wstring documentosPath(documentosDir);

        // Concatenar el nombre del archivo al directorio de documentos
        std::wstring archivoPath = documentosPath + L"\\PIA_PROGRA_DYAC_JDFH\\citas.bin";

        // Abrir el archivo en modo binario para lectura
        std::ifstream archivocitas(archivoPath, std::ios::binary | std::ios::in);

        if (!archivocitas.is_open())
        {
            return;
        }

        while (in!= nullptr)
        {
            cita* temp = in;
            in = in->sige;
            delete temp;
        }

        cita leido;
        while (archivocitas.read(reinterpret_cast<char*>(&leido), sizeof(cita)))
        {
            cita* nuevacita = new cita;
            memcpy(nuevacita, &leido, sizeof(cita));

            nuevacita->sige = in;
            nuevacita->ante = nullptr;

            if (in != nullptr)
            {
                in->ante = nuevacita;
            }
            in = nuevacita;
        }

        archivocitas.close();
    }
}