#pragma comment(lib, "winmm.lib")
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <mmsystem.h>
#include <vector>
#include <algorithm>
#include <fstream>
#define TIMER1 988
#define TIMER2 989
HBRUSH hBrush ;
PAINTSTRUCT ps ;
RECT rect;
static int uvod = 1;
static int pozicijaxtemp;
static int counter;
static int vertikalno1, vertikalno2;
static int niz2[10]={0,0,0,0,0,0,0,0,0,0};
static int niz3[10]={0,0,0,0,0,0,0,0,0,0};
static int nv=0;
int nn=1;
std::vector<int>scores;
static int  random, random1;
static int ispaljen1=0;
static int ispaljen2=0;
static int metak1x,metak2x;
static int gameover=0;
typedef struct ObjectInfo
{
    int width;
    int height;
    int dx;
    int dy;
    double x;
    double y;

} Object;

typedef struct
{
    int pozicijax;
    int pozicijay;
    int pogodjen;
} neprijatelj;

static HDC hdc, hdcMem, hdcMem1;
static HBITMAP hbmpSlika, hbmpMaska, MetakMaska;
static HBITMAP Pozadina, Brod, Maska, Laser;
static HBITMAP Alien1, Alien2, Alien3, AlienMaska;
static HBITMAP Eksplozija, EksplozijaVelika, EksplozijaVelikaMask, EksplozijaMaska;
static HBITMAP Metak;
static HBITMAP hbmpStari, hbmpMemStari,Menu;
static BITMAP bitmap;
static int i = 470, zivoti, y = 0, ymove = 0, level = 1;
static int vertikalno, poeni = 0, smjer = 1, x = 0, xtemp;
static int metakx, ispaljen = 0;
double highscore=0;
int highscore2=0;
static neprijatelj niz[4][10];
char buffer[5];
HFONT Font1,Font2, Font3, hTemp;
Object menu,avion;
SIZE sz;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK AboutDlgProc (HWND, UINT, WPARAM, LPARAM) ;
  WNDCLASSEX wincl;        /* Data structure for the windowclass */
/*  Make the class name into a global variable  */
char szClassName[ ] = "CodeBlocksWindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */


    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               "Alien Resurrection",       /* Title Text */
               WS_OVERLAPPED | WS_SYSMENU, /* default window */
               5/*CW_USEDEFAULT*/,       /* Windows decides the position */
               5/*CW_USEDEFAULT*/,       /* where the window ends up on the screen */
               1000,                 /* The programs width */
               710,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

             int bb=0;
std::string myText;
    std::ifstream MyReadFile("SCORES.txt");


if( MyReadFile.peek() == std::ifstream::traits_type::eof()){
scores.push_back(0);
}
else{

while (getline (MyReadFile, myText)) {

  scores.push_back(stoi(myText));

}
}
// Close the file
MyReadFile.close();



highscore=scores[scores.size()-1];
highscore2=(int) highscore;
printf("high score je %d\n",highscore2);


    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
      //  printf("I am infinity loop\n");

        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */

BOOL provjera(neprijatelj nule[4][10])
{
    for (int k = 0 ; k < 4 ; k++)
    {
        for (int j = 0 ; j < 10 ; j++)
        {
            if (nule[k][j].pogodjen != 0)
            {
                return FALSE;
                break;
            }
        }

    }
   PlaySoundW(L"LevelUp.wav", NULL, SND_FILENAME | SND_ASYNC);

    level++;
    for(int z=0;z<10;z++){
         niz3[z]=0;
    niz2[z]=0;

    }

    return TRUE;
}

void postaviflotu(neprijatelj nule[4][10], int c, int d)
{
    for (int k = 0 ; k < 4 ; k++)
    {
        for (int j = 0 ; j < 10 ; j++)
        {
            if (d == 0)
            {
                if (k == 0)
                    niz[k][j].pogodjen = 3;
                if (k == 1)
                    niz[k][j].pogodjen = 2;
                if (k == 2 || k == 3)
                    niz[k][j].pogodjen = 1;
            }
            niz[k][j].pozicijay = k * 50 + 45;
        }
    }
    ymove = 0;
    vertikalno = 600;
    vertikalno1 = -10;
    if( c == 0)
        zivoti = 3;
}

int create(HINSTANCE hInstance,HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){

PlaySoundW(L"UnrealTournament.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

        postaviflotu(niz, 0, 0);
        hInstance = ((LPCREATESTRUCT) lParam)->hInstance ;
        SetTimer (hwnd, TIMER1, 20, NULL);
        SetTimer (hwnd, TIMER2, 100, NULL);
        Pozadina=(HBITMAP) LoadImage(NULL, "scenario2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        Brod=(HBITMAP) LoadImage(NULL, "spaceship.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        Maska=(HBITMAP) LoadImage(NULL, "spaceshipBLLACK.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        Laser=(HBITMAP) LoadImage(NULL, "laser.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        Metak=(HBITMAP) LoadImage(NULL, "imagesbijela.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        MetakMaska=(HBITMAP) LoadImage(NULL, "imagescrna.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        Alien1=(HBITMAP) LoadImage(NULL, "enemy23.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        Alien3=(HBITMAP) LoadImage(NULL, "enemy21.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        Alien2=(HBITMAP) LoadImage(NULL, "enemy22.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        AlienMaska=(HBITMAP) LoadImage(NULL, "alienmaska.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        Eksplozija=(HBITMAP) LoadImage(NULL, "eksplozija.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        EksplozijaMaska=(HBITMAP) LoadImage(NULL, "eksplozijam.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        EksplozijaVelika=(HBITMAP) LoadImage(NULL, "eksplozijabig.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        EksplozijaVelikaMask=(HBITMAP) LoadImage(NULL, "eksplozijabigm.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        Menu=(HBITMAP) LoadImage(NULL, "Menu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);



if(AlienMaska==NULL ){

        MessageBox(0, "Couldn't load the image", "Error", MB_OK);
}

 BITMAP bitmap;
        GetObject(Menu, sizeof(BITMAP), &bitmap);
        menu.width = bitmap.bmWidth;
        menu.height = bitmap.bmHeight;

         GetObject(Brod, sizeof(BITMAP), &bitmap);
        avion.width = bitmap.bmWidth;
        avion.height = (bitmap.bmHeight) ;
        avion.y=550;
        avion.x=350;

        return 0;

}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{



   static HINSTANCE hInstance ;
    RECT r;
    GetClientRect(hwnd, &r);
    int xx = LOWORD(lParam);
    int yy = HIWORD(lParam);

    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:

         create(hInstance,hwnd,  message,  wParam, lParam);
          break;
    case WM_TIMER:
        switch (wParam)
        {
        case TIMER1:


            if(uvod==1){
                //    printf("udje ovdje\n");
HDC hdcBuffer = CreateCompatibleDC(hdc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
    HBITMAP hbmOldBuffer = (HBITMAP) SelectObject(hdcBuffer, hbmBuffer);
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmOld1 = (HBITMAP) SelectObject(hdcMem, Menu);
    BitBlt(hdcBuffer, menu.x, menu.y,menu.width, menu.height, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdc, hbmOld1);

    SelectObject(hdcMem, hbmOld1);
    DeleteDC(hdcMem);
    BitBlt(hdc, 0,0, rect.right, rect.bottom, hdcBuffer, 0,0, SRCCOPY);
    DeleteObject(hbmBuffer);
    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteDC(hdcBuffer);
    DeleteObject(hbmOldBuffer);

            }


            InvalidateRect (hwnd, NULL, FALSE) ;
            if(smjer == 0)
            {
                if (x <= 0)
                {

                    smjer = 1;
                    ymove += 15;
                }
                if (level == 1)
                    x--;
                else
                    x -= log(level) * 10 / 4;
            }
            if(smjer == 1)
            {
                if (x >= 400)
                {


                    smjer = 0;
                    ymove += 15;
                }
                if (level == 1)
                    x++;
                else
                    x += log(level) * 10 / 4;
            }

            if(GetAsyncKeyState(VK_LEFT))
            {
                if(i >= -20)
                    i -= 10;
                    avion.x-=10;
            }
            if(GetAsyncKeyState(VK_RIGHT))
            {
                if(i <= 950)
                    i += 10;
                       avion.x+=10;
            }
            if(GetAsyncKeyState(VK_SPACE))
            {
                if (ispaljen == 0)
                {
                    metakx = xtemp;
                    ispaljen = 1;
                    PlaySoundW(L"LaserGun.wav", NULL, SND_FILENAME | SND_ASYNC);
                }
            }

            break;
        case TIMER2:
            random = rand() % 40 + 1;
            random1 = rand() % 40 + 1;

            break;
        }

          case WM_CHAR:
            {


                if (wParam=='r' || wParam=='R'){

                    if(gameover==1){
                            gameover=0;


       std::string myText;
    std::ifstream MyReadFile("SCORES.txt");


if( MyReadFile.peek() == std::ifstream::traits_type::eof()){
scores.push_back(0);
}
else{

while (getline (MyReadFile, myText)) {

  scores.push_back(stoi(myText));

}
}
// Close the file
MyReadFile.close();



highscore=scores[scores.size()-1];
highscore2=(int) highscore;

nv=0;
nn=1;
poeni=0;
zivoti=3;
level=1;
  for(int z=0;z<10;z++){
         niz3[z]=0;
    niz2[z]=0;

    }
create(hInstance,hwnd,  message,  wParam, lParam);

                }
                }
                break;
            }


        case WM_KEYUP:
    {
        if(wParam=='r' || wParam=='R'){

        }
        break;
    }


    case WM_PAINT:
        hdc = BeginPaint (hwnd, &ps);
        GetClientRect (hwnd, &rect);

        hdc = GetDC (hwnd);
        hdcMem = CreateCompatibleDC (hdc);
        hbmpStari = (HBITMAP) SelectObject (hdcMem, CreateCompatibleBitmap (hdc, rect.right, rect.bottom));
        if(uvod==1){
        hbmpSlika = (HBITMAP) Menu;
        }
        if(uvod==0){
            hbmpSlika = (HBITMAP) Pozadina;

        }
        GetObject (hbmpSlika, sizeof(BITMAP), &bitmap);
        hdcMem1 = CreateCompatibleDC (hdc);
        hbmpMemStari = (HBITMAP) SelectObject (hdcMem1, hbmpSlika);
        BitBlt (hdcMem, 0, y - 710, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCCOPY);
        BitBlt (hdcMem, 0, y, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCCOPY);
        SelectObject (hdcMem1, hbmpMemStari);
        DeleteDC (hdcMem1);

        if (y >= 710)
            y = 0;
        else
            y += 2;

        if (uvod == 0)
        {

            hbmpSlika = Brod;
            hbmpMaska = Maska;

            GetObject (hbmpSlika, sizeof(BITMAP), &bitmap);
            hdcMem1 = CreateCompatibleDC (hdc);
            hbmpMemStari = (HBITMAP) SelectObject (hdcMem1, hbmpSlika);

            SelectObject(hdcMem1, hbmpMaska);
            BitBlt(hdcMem, avion.x, avion.y, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCPAINT);
            SelectObject(hdcMem1, hbmpSlika);
            BitBlt(hdcMem, avion.x, avion.y, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCAND);
            SelectObject (hdcMem1, hbmpMemStari);
            DeleteDC (hdcMem1);
            xtemp = avion.x + 28;

            if (ispaljen == 0)
            {
                hbmpSlika = Metak;
                hbmpMaska = MetakMaska;
                GetObject (hbmpSlika, sizeof(BITMAP), &bitmap);
                hdcMem1 = CreateCompatibleDC (hdc);
                hbmpMemStari = (HBITMAP) SelectObject (hdcMem1, hbmpSlika);
                SelectObject(hdcMem1, hbmpMaska);
                BitBlt(hdcMem, avion.x + 27.5, avion.y - 16, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCPAINT);
                SelectObject(hdcMem1, hbmpSlika);
                BitBlt(hdcMem, avion.x + 27.5, avion.y - 16, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCAND);
                SelectObject (hdcMem1, hbmpMemStari);
                DeleteDC (hdcMem1);
            }
            else if (ispaljen == 1)
            {

                hbmpSlika = Metak;
                hbmpMaska = MetakMaska;
                GetObject (hbmpSlika, sizeof(BITMAP), &bitmap);
                hdcMem1 = CreateCompatibleDC (hdc);
                hbmpMemStari = (HBITMAP) SelectObject (hdcMem1, hbmpSlika);
                SelectObject(hdcMem1, hbmpMaska);
                BitBlt(hdcMem, metakx, vertikalno, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCPAINT);
                SelectObject(hdcMem1, hbmpSlika);
                BitBlt(hdcMem, metakx, vertikalno, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCAND);
                SelectObject (hdcMem1, hbmpMemStari);
                DeleteDC (hdcMem1);
                vertikalno -= 20;

                if (vertikalno < 0)
                {
                    ispaljen = 0;
                    vertikalno = 600;
                }
            }

            for (int k = 0 ; k < 4 ; k++)
            {
                for (int j = 0 ; j < 10 ; j++)
                {
                    if (niz[k][j].pogodjen != 0)
                    {
                        pozicijaxtemp = x + niz[k][j].pozicijax + 60 * j;

                        if (vertikalno >= niz[k][j].pozicijay + ymove && vertikalno <= niz[k][j].pozicijay + ymove + 50)
                        {
                            if (metakx >= pozicijaxtemp && metakx <= pozicijaxtemp + 50)
                            {
                                hbmpSlika = Eksplozija;
                                hbmpMaska = EksplozijaMaska;

                                GetObject (hbmpSlika, sizeof(BITMAP), &bitmap);
                                hdcMem1 = CreateCompatibleDC (hdc);
                                hbmpMemStari = (HBITMAP) SelectObject (hdcMem1, hbmpSlika);
                                SelectObject(hdcMem1, hbmpSlika);
                                BitBlt(hdcMem, pozicijaxtemp, niz[k][j].pozicijay + ymove, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCAND);
                                SelectObject(hdcMem1, hbmpSlika);
                                BitBlt(hdcMem, pozicijaxtemp, niz[k][j].pozicijay + ymove, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCPAINT);
                                SelectObject (hdcMem1, hbmpMemStari);
                                DeleteDC (hdcMem1);


                                  niz[k][j].pogodjen--;


if(k==0){
     niz3[j]+=1;
     if(niz3[j]==1){
        poeni+=50;
     } else if(niz3[j]==2){
     poeni+=30;
     }else if(niz3[j]==3){
     poeni+=10;
     }
}
else if(k==1){
    niz2[j]+=1;
    if(niz2[j]==1){
        poeni+=30;
     } else if(niz2[j]==2){
     poeni+=10;
}
}
else if(k==2 || k==3){
    poeni+=10;
}

if(poeni>highscore2){

    highscore2=poeni;




if(nn==1){

PlaySoundW(L"newhighscore.wav", NULL, SND_FILENAME );
nn=2;
}
    nv=1;
    }



                                ispaljen = 0;
                                vertikalno = 600;
                                PlaySoundW(L"HEADSHOT.wav", NULL, SND_FILENAME | SND_ASYNC);

                                if (poeni % 2000 == 0)
                                    zivoti++;
                                if (provjera (niz) == TRUE)
                                    postaviflotu(niz, 1, 0);
                            }
                        }

                                                  //  printf("niz: %d\n", niz[3][j].pozicijay+ymove+37);


                        if (niz[k][j].pozicijay + ymove + 50 >=599)
                        {
                        //    printf("niz: %d\n", niz[k][j].pozicijay+ymove+37);
                           // printf("avion.y: %d\n", avion.y);

                            hbmpSlika = EksplozijaVelika;
                            hbmpMaska = EksplozijaVelikaMask;

                            GetObject (hbmpSlika, sizeof(BITMAP), &bitmap);
                            hdcMem1 = CreateCompatibleDC (hdc);
                            hbmpMemStari = (HBITMAP) SelectObject (hdcMem1, hbmpSlika);
                            SelectObject(hdcMem1, hbmpSlika);
                            BitBlt(hdcMem, i, 600, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCAND);
                            SelectObject(hdcMem1, hbmpSlika);
                            BitBlt(hdcMem, i, 600, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCPAINT);
                            SelectObject (hdcMem1, hbmpMemStari);
                            DeleteDC (hdcMem1);
                           PlaySoundW(L"explosion.wav", NULL, SND_FILENAME | SND_ASYNC);
                            vertikalno1 = niz[k][j].pozicijay + ymove;
                            vertikalno2 = niz[k][j].pozicijay + ymove;
                            postaviflotu(niz, 1, 1);
                            ispaljen1 = 0;
                            ispaljen2 = 0;
                            zivoti--;
                        }
                        if (niz[k][j].pogodjen == 3){
                    //  printf("pogodjen 3\n");

                            hbmpSlika = Alien3;

                        }
                        if (niz[k][j].pogodjen == 2){
                            //    printf("pogodjen 2\n");
                            hbmpSlika = Alien2;

                        }
                        if (niz[k][j].pogodjen == 1){
                        //printf("pogodjen 1\n");


                        hbmpSlika = Alien1;

                        }


           hbmpMaska = AlienMaska;
                        GetObject (hbmpSlika, sizeof(BITMAP), &bitmap);
                        hdcMem1 = CreateCompatibleDC (hdc);
                        hbmpMemStari = (HBITMAP) SelectObject (hdcMem1, hbmpSlika);
                        SelectObject(hdcMem1, hbmpMaska);
                        BitBlt(hdcMem, x + niz[k][j].pozicijax + 60 * j, niz[k][j].pozicijay + ymove, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCAND);

                        SelectObject(hdcMem1, hbmpSlika);

                        BitBlt(hdcMem, x + niz[k][j].pozicijax + 60 * j, niz[k][j].pozicijay + ymove, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCPAINT);

                        SelectObject (hdcMem1, hbmpMemStari);
                        DeleteDC (hdcMem1);



                        if (counter == random && niz[k][j].pogodjen != 0)
                        {
                            if (ispaljen1 == 0)
                            {
                                metak1x = x + niz[k][j].pozicijax + 60 * j + 20;
                                vertikalno1 = niz[k][j].pozicijay + ymove;
                                ispaljen1 = 1;
                            }

                        }
                        if (counter == random1 && niz[k][j].pogodjen != 0)
                        {
                            if (ispaljen2 == 0)
                            {
                                metak2x = x + niz[k][j].pozicijax + 60 * j + 20;
                                vertikalno2 = niz[k][j].pozicijay + ymove;
                                ispaljen2 = 1;
                            }

                        }
                    }
                    counter++;
                }
            }
            counter = 1;

            if (vertikalno1 >= 600 && vertikalno1 <= 651 && metak1x > avion.x && metak1x < avion.x + 80)
            {
                hbmpSlika = EksplozijaVelika;
                hbmpMaska = EksplozijaVelikaMask;

                GetObject (hbmpSlika, sizeof(BITMAP), &bitmap);
                hdcMem1 = CreateCompatibleDC (hdc);
                hbmpMemStari = (HBITMAP) SelectObject (hdcMem1, hbmpSlika);
                SelectObject(hdcMem1, hbmpSlika);
                BitBlt(hdcMem, avion.x, avion.y, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCAND);
                SelectObject(hdcMem1, hbmpSlika);
                BitBlt(hdcMem, avion.x, avion.y, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCPAINT);
                SelectObject (hdcMem1, hbmpMemStari);
                DeleteDC (hdcMem1);
                PlaySoundW(L"ExplosionSound.wav", NULL, SND_FILENAME | SND_ASYNC);
                vertikalno1 = -10;
                postaviflotu(niz, 1, 1);
                ispaljen1 = 0;
                zivoti--;
              //  printf(" VER1 ZIVOTI: %d\n",zivoti);
            }
            if (vertikalno2 >= 600 && vertikalno2 <= 651 && metak2x > avion.x && metak2x < avion.x + 80)
            {
                hbmpSlika = EksplozijaVelika;
                hbmpMaska = EksplozijaVelikaMask;

                GetObject (hbmpSlika, sizeof(BITMAP), &bitmap);
                hdcMem1 = CreateCompatibleDC (hdc);
                hbmpMemStari = (HBITMAP) SelectObject (hdcMem1, hbmpSlika);
                SelectObject(hdcMem1, hbmpSlika);
                BitBlt(hdcMem, avion.x, avion.y, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCAND);
                SelectObject(hdcMem1, hbmpSlika);
                BitBlt(hdcMem, avion.x, avion.y, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCPAINT);
                SelectObject (hdcMem1, hbmpMemStari);
                DeleteDC (hdcMem1);
             PlaySoundW(L"ExplosionSound.wav", NULL, SND_FILENAME | SND_ASYNC);
                vertikalno2 = -10;
                postaviflotu(niz, 1, 1);
                ispaljen2 = 0;
                zivoti--;


            }

            if (ispaljen1 == 1)
            {
                hbmpSlika = Laser;
                GetObject (hbmpSlika, sizeof(BITMAP), &bitmap);
                hdcMem1 = CreateCompatibleDC (hdc);
                hbmpMemStari = (HBITMAP) SelectObject (hdcMem1, hbmpSlika);
                SelectObject(hdcMem1, hbmpSlika);
                BitBlt(hdcMem, metak1x, vertikalno1, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCCOPY);
                SelectObject (hdcMem1, hbmpMemStari);
                DeleteDC (hdcMem1);
                vertikalno1 += 10;
                if (vertikalno1 >= 710)
                    ispaljen1 = 0;
            }
            if (ispaljen2 == 1)
            {
                hbmpSlika = Laser;
                GetObject (hbmpSlika, sizeof(BITMAP), &bitmap);
                hdcMem1 = CreateCompatibleDC (hdc);
                hbmpMemStari = (HBITMAP) SelectObject (hdcMem1, hbmpSlika);
                SelectObject(hdcMem1, hbmpSlika);
                BitBlt(hdcMem, metak2x, vertikalno2, bitmap.bmWidth, bitmap.bmHeight, hdcMem1, 0, 0, SRCCOPY);
                SelectObject (hdcMem1, hbmpMemStari);
                DeleteDC (hdcMem1);
                vertikalno2 += 10;
                if (vertikalno2 >= 710)
                    ispaljen2 = 0;
            }
            if (zivoti <= 0)
            {
gameover=1;

if(nv==1){

 std::ofstream outfile;

  outfile.open("SCORES.txt", std::ios_base::app); // append instead of overwrite
  outfile <<poeni<<"\n";
}


                KillTimer(hwnd, TIMER1);
                KillTimer(hwnd, TIMER2);
                SetBkMode(hdcMem, TRANSPARENT);
                SetTextColor(hdcMem, RGB(148,238,144));
                Font1 = CreateFont(80, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, "Consolas"/*"SYSTEM_FIXED_FONT"*/);
                Font2 = CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, "Consolas"/*"SYSTEM_FIXED_FONT"*/);
         GetTextExtentPoint32 (hdcMem, buffer, strlen (buffer), &sz);
                if(nv==1){
hTemp = (HFONT) SelectObject(hdcMem, Font2);
                SetTextColor(hdcMem, RGB(255,238,0));
                TextOut (hdcMem, 390, 280, "NEW HIGH SCORE:" , 15);
                TextOut(hdcMem, 600, 280, std::to_string(highscore2).c_str(), strlen(std::to_string(highscore2).c_str()));
                }else{
 hTemp = (HFONT) SelectObject(hdcMem, Font2);
                  TextOut (hdcMem, 420, 280, "HIGH SCORE:" , 11);
                  TextOut(hdcMem, 580, 280, std::to_string(highscore2).c_str(), strlen(std::to_string(highscore2).c_str()));
                }
                hTemp = (HFONT) SelectObject(hdcMem, Font1);

                SetTextColor(hdcMem, RGB(148,238,144));


                TextOut (hdcMem, 350, 300, "GAME OVER", 9);
                SetTextColor(hdcMem, RGB(148,238,144));
                hTemp = (HFONT) SelectObject(hdcMem, Font2);
                SetTextColor(hdcMem, RGB(255,0,100));
                TextOut (hdcMem, 400,380, "PRESS R FOR REPLAY", 18);


                           PlaySoundW(L"GameOver.wav", NULL, SND_FILENAME | SND_ASYNC);

            }

            SetBkMode(hdcMem, TRANSPARENT);
            sprintf (buffer, "%d", poeni);
            GetTextExtentPoint32 (hdcMem, buffer, strlen (buffer), &sz);
            SetTextColor(hdcMem, RGB(144,238,144));
            Font1 = CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0,  "Segoe UI"/*"SYSTEM_FIXED_FONT"*/);
            hTemp = (HFONT) SelectObject(hdcMem, Font1);
            TextOut (hdcMem, 5, 5, "SCORE:", 6);
            TextOut (hdcMem, 80, 5, buffer, strlen (buffer));
            TextOut (hdcMem, 425, 5, "LEVEL:", 6);
            sprintf (buffer, "%d", level);
            TextOut (hdcMem, 490, 5, buffer, strlen (buffer));
            TextOut (hdcMem, 870, 5, "LIVES:", 6);
            sprintf (buffer, "%d", zivoti);
            TextOut (hdcMem, 935, 5, buffer, strlen (buffer));
            BitBlt (hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);
            DeleteObject (SelectObject (hdcMem, hbmpStari));
            DeleteObject(SelectObject(hdcMem,hTemp));
            DeleteObject (hdcMem);

        ReleaseDC (hwnd, hdc);
        EndPaint (hwnd, &ps) ;}
        DeleteObject (hBrush) ;
        return 0 ;
    case WM_MOUSEMOVE:
    {
    int savedX = avion.x, savedY = avion.y;

         if (!(xx  > r.right || xx - avion.width  < 0))
        {
            avion.x = xx - avion.width ;
        }
        if (!(yy > r.bottom || yy - avion.height  < 0))
        {
            avion.y = yy - avion.height;
        }
        if (avion.y > r.bottom  || avion.x + avion.width/2 > r.right || avion.y - avion.height/2 - ymove < 230 ) {
            avion.x = savedX;
            avion.y = savedY;
        }
    }
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            printf("ENTER");
            uvod = 0;
            return 0;
        case VK_ESCAPE:
            exit(0);
            return 0;
        default:
            return 0;
        }
    case WM_DESTROY:
        KillTimer (hwnd, TIMER1);
        KillTimer (hwnd, TIMER2);
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}

