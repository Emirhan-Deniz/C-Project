#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <sys/types.h>
#include <unistd.h>
//#include <errno.h>
#include <windows.h>
#include <tchar.h>
void kure();
void noktaMesafe();
void dosyaKontrol();
void kup();
void uzaklik();
struct hatali
{
    char fileName[20][50];
    double satirSayisi[20];
};

int main()
{
    system("color E");
    struct hatali hata;
    hata.satirSayisi[0]=-1;

    int secim;
    while(secim!=6)
    {
        menuGoster();
        scanf("%d",&secim);

        switch(secim)
        {
        case 1:
            dosyaKontrol(&hata);
            break;
        case 2:
            if(hata.satirSayisi[0]==-1)
            {
                printf("\n\t\tOnce 1. islemi gerceklestirmelisiniz.");
                break;
            }
            else
            {
                noktaMesafe(&hata);
                break;
            }

        case 3:
            if(hata.satirSayisi[0]==-1)
            {
                printf("\n\t\tOnce 1. islemi gerceklestirmelisiniz.");
                break;
            }
            else
            {
                kup(&hata);
                break;
            }
        case 4:
            if(hata.satirSayisi[0]==-1)
            {
                printf("\n\t\tOnce 1. islemi gerceklestirmelisiniz.");
                break;
            }
            else
            {
                kure(&hata);
                break;
            }
        case 5:
            if(hata.satirSayisi[0]==-1)
            {
                printf("\n\t\tOnce 1. islemi gerceklestirmelisiniz.");
                break;
            }
            else
            {
                uzaklik(&hata);
                break;
            }
            break;
        case 6:
            break;
        default:
            printf("Gecersiz bir deger girdiniz.");
            break;
        }
    }

    return 0;
}
void menuGoster()
{
    printf("\n\n\t\t----Menu----");
    printf("\n\t1-Dosya Kontrolu");
    printf("\n\t2-En Yakin/Uzak Noktalar");
    printf("\n\t3-Kup");
    printf("\n\t4-Kure");
    printf("\n\t5-Nokta Uzakliklari");
    printf("\n\t6-Cikis");
    printf("\n\n\t\tSeciminizi belirtiniz: ");
}
void dosyaKontrol(struct hatali *hata)
{
    int j=0,p=0;
    FILE *yazma;
    FILE *okuma;
    WIN32_FIND_DATA FindFileData;//Hazýr struct yapisi.
    HANDLE hFind;

    TCHAR  *directory = TEXT(".");
    TCHAR patter[MAX_PATH];

    memset(patter, 0x00, MAX_PATH);
    _stprintf(patter, TEXT("%s\\*.nkt"), directory); //Dizindeki .nkt uzantýlý dosyalarý buluyor.
    hFind = FindFirstFile(patter, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("FindFirstFile failed (%d)\n", GetLastError());

    }
    else
    {
        while (FindNextFile(hFind, &FindFileData))
        {
            int g=0,f=0;
            char tipTutucu[6];//Dosyanýn ascii mi binary mi olduðu bilgisini tutmak için
            char b=NULL;// rgb kontrol etmek için
            char a=NULL;// rgb kontrol etmek için
            int satir_sayisi=0,i=0;// dosyanin satir sayisini sayiyoruz, kontrol için.
            int sonuc;// iki stringi karþýlaþtýrýrken sonucu bu deðiþkende tutacaz.
            int satirSayisi=0;// satýr sayýsýný saydýktan sonra struck içine atarken kaybetmemek için tanýmladýk.
            int deneme;// eðer dosya hatalýysa -1 veya -2 atayacaðýz.
            char veri;//karakter karakter okurken o anki karakteri tutacak.
            char fileNokta[10];//dosyada verilen satýr sayýsýnýn verisini tutacak.
            int bosluk=0;//satýr sayýsý verisine ulaþma
            int boslukSayisi=0;//rgb verisine ulaþma
            int boslukSayisi2=0;//dosya tipi verisine ulaþma
            int bosluk2=0;//rgb verisine ulaþma boþluk için
            char str[10];// inteer satir sayisini karþýlaþtýrmak için stringe çevirdiðimizde tutacaðýmýz deðiþken.

            char nameOfFile[50]="Output-";//dosya adýnýn önüne output yazmak.

            //ignore current and parent directories
            /*if(_tcscmp(FindFileData.cFileName, TEXT("."))==0 || _tcscmp(FindFileData.cFileName, TEXT(".."))==0)
                continue;*/
            okuma=fopen(FindFileData.cFileName,"r");//dosya okumak için açýlýyor

            if(strstr(FindFileData.cFileName,"Output")!=0)// output isimli nkt dosyasý varsa atlayacak.
            {
                continue;
            }
            strcat(nameOfFile,FindFileData.cFileName);//output için açýlan dosyanýn baþýna Output- ekliyoruz.
            yazma=fopen(nameOfFile,"w+");//dosya yazmak için açýlýyor.
            fprintf(yazma,"SECIM 1\n");

            /*if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                //ignore directories
            }*/

            //else
            //{
            for(veri=getc(okuma); veri!=EOF; veri=getc(okuma))//karakter karakter okuma
            {
                if(veri=='\n')//bir satirin bitimi
                {
                    satir_sayisi++;
                }

                if(satir_sayisi==2)//rgb mi ?
                {
                    boslukSayisi++;
                    if(boslukSayisi==16)
                    {
                        a=veri;
                    }
                    if(boslukSayisi==17)
                    {
                        b=veri;
                    }
                }
                if(satir_sayisi==4)//binary mi ascii mi?
                {
                    boslukSayisi2++;
                    if(boslukSayisi2>=6)
                    {
                        tipTutucu[g]=veri;
                        g++;
                    }
                }

                if(satir_sayisi==3)//Satir sayisi verisinin oldugu satir.
                {
                    bosluk++;//integer gelene kadar olan char sayisi
                    if(bosluk>=11)
                    {
                        fileNokta[i]=veri;
                        i++;
                    }
                }
                if(a!=NULL)//baþlangýçta NULL a eþitledik. Eðer !=NULL ise deðeri dðeiþmiþtir.
                {
                    if(a=='r'&&b==' ')//rg b þeklinde yazýldýysa hatalý
                    {
                        if(satir_sayisi>5)// nokta verilerinin olduðu satýrlar
                        {
                            if(veri==' ')//rgb li ise ve her satýrda 6 veriden az veri varsa hatalýdýr. 6 veri =5 boþluk
                            {
                                bosluk2++;
                            }
                        }
                        if(veri=='\n'&&satir_sayisi>5)
                        {
                            if(bosluk2<5)
                            {
                                satirSayisi=satir_sayisi;
                                deneme=-1;
                            }
                            bosluk2=0;
                        }
                    }
                    if(a=='r'&&b!=' ')
                    {
                        if(satir_sayisi>5)
                        {
                            if(veri==' ')
                            {
                                bosluk2++;
                            }
                        }
                        if(veri=='\n'&&satir_sayisi>5)
                        {
                            if(bosluk2<5)
                            {
                                deneme=-2;
                            }
                            bosluk2=0;
                        }
                    }
                }

            }
            if(strstr(tipTutucu,"binary")!=0)//Eðer dosta binary ise bunlarý gerçekleþtirecek.
            {
                printf("%s Basarili: Dosyalar uyumlu \n\n", FindFileData.cFileName);
                fprintf(yazma,"%s Basarili: Dosyalar uyumlu.\n",FindFileData.cFileName);
                continue;
            }
            fileNokta[i]='\0';//Rastegele veri atamasýn diye diziye bitirme elemaný ekledik.
            if(satirSayisi!=6)
            {
                if(a=='r')//rgb li ise
                {
                    if(deneme==-2)//hatalý ise
                    {
                        strcpy(hata->fileName[j],FindFileData.cFileName);
                        printf("\n%s Basarisiz rgb bilgileri yanlis verilmis\n",FindFileData.cFileName);
                        fprintf(yazma,"%s Basarisiz rgb bilgileri yanlis verilmis\n",FindFileData.cFileName);
                        j++;
                        continue;
                    }
                    else if(deneme==-1)//hataý ise
                    {
                        strcpy(hata->fileName[j],FindFileData.cFileName);
                        printf("\n%s Basarisiz %d. satirda rgb bilgileri verilmemistir.\n",FindFileData.cFileName,satirSayisi);
                        fprintf(yazma,"%s Basarisiz %d. satirda rgb bilgileri verilmemistir.\n",FindFileData.cFileName,satirSayisi);
                        j++;
                        continue;
                    }
                }
            }


            printf("\n");

            sprintf(str, "%d", satir_sayisi-4);//integer sayiyi stringe cevirme.
            sonuc=strcmp(str,fileNokta);//iki string veriyi karsilastirma.

            if(sonuc==0)//karsilastirmanin sonucu 0 ise iki string birbirine esittir.
            {
                hata->satirSayisi[p]=satir_sayisi;
                p++;
                printf("%s Basarili: Dosyalar uyumlu \n", FindFileData.cFileName);
                fprintf(yazma,"%s Basarili: Dosyalar uyumlu.\n",FindFileData.cFileName);
            }
            else//deðilse nokta sayýsý geçersþz
            {
                strcpy(hata->fileName[j],FindFileData.cFileName);
                printf("%s Basarisiz: Nokta sayisi gecersiz.\n",FindFileData.cFileName);
                fprintf(yazma,"%s Basarisiz: Nokta sayisi gecersiz.\n",FindFileData.cFileName);
                j++;
            }
            //}
            //list the Files
            //1printf("%s",FindFileData.cFileName);
            /*_tprintf (TEXT("%s\n"),
                      FindFileData.cFileName);*/
        }

    }

}
void noktaMesafe(struct hatali *hata)
{
    FILE *yazma;
    FILE *okuma;
    FILE *binary;

    int satirTutucu[2][2];//en yakın/uzan noktaların satır sayısını tutmak için tanımladık.
    int j=0;
    int k=0;//matrisi tanimlamakta satir sayisini almak icin kullandik.


    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    TCHAR  *directory = TEXT(".");//main.c nin bulunduğu dizindeki çnkt uzantılı dosyalar
    TCHAR patter[MAX_PATH];

    memset(patter, 0x00, MAX_PATH);
    _stprintf(patter, TEXT("%s\\*.nkt"), directory);
    hFind = FindFirstFile(patter, &FindFileData);//ilk nkt uzantili dosyayi bulma
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("FindFirstFile failed (%d)\n", GetLastError());
    }
    else
    {
        while (FindNextFile(hFind, &FindFileData))
        {
            int q=0,w=0;
            float arr;//binary dosyayı fread ile okurken lazım.
            float dizi[20][6];//binary dosyanın verilerini tutuyor
            int boslukSayisi2=0;//tip tutucu için tanımladık
            int x=0;
            char tipTutucu[6];//binary mi ascii mi
            double enYakin[2][3];//en yazkin 2 nokta verielri
            double enUzak[2][3];//en uzak 2 nokta verileri
            double oAnkiMesafe=0;//2 for içerisinde o anki mesafeyi tutan değişken
            double ek=0;// en kısa mesafe verisi
            double eb=0;// en uzaun mesafe verisi
            char a=NULL;
            int i=0;
            double r,g,b;//rgb verilierini tutmak için tnaımladık
            char veri;
            int  satir_sayisi=0;
            int boslukSayisi=0;
            char nameOfFile[50]="Output-";

            if(strcmp(FindFileData.cFileName,hata->fileName[j])==0)//hatalı strucktan hatalı olan dosyanın ismi varsa o dosyayı okumadan geçiyor
            {
                printf("%s isimli dosya hatalidir. islem yapilmaz\n\n",FindFileData.cFileName);
                j++;
                continue;
            }
            if(strstr(FindFileData.cFileName,"Output")!=0)//içerisinde output geen dosyaları okumuyor
            {
                continue;
            }

            binary=fopen(FindFileData.cFileName,"r");//dosya binary mi dğeil mi ona bakıyor

            for(veri=getc(binary); veri!=EOF; veri=getc(binary))
            {
                if(veri=='\n')//bir satirin bitimi
                {
                    satir_sayisi++;
                }
                if(satir_sayisi==2)
                {
                    boslukSayisi++;
                    if(boslukSayisi==16)
                    {
                        a=veri;
                    }
                }
                if(satir_sayisi==4)
                {
                    boslukSayisi2++;
                    if(boslukSayisi2>=6)
                    {
                        tipTutucu[x]=veri;//dosya tipini aldık
                        x++;
                    }
                }
            }
            //printf("%s\n",tipTutucu);
            fclose(binary);
            satir_sayisi=0;
            if(strstr(tipTutucu,"binary")!=0)//eğer binary ise bu işlemi yapacak
            {
                strcat(nameOfFile,FindFileData.cFileName);
                yazma=fopen(nameOfFile,"a");
                fprintf(yazma,"SECIM 2");
                FILE *okuyucu;
                okuyucu=fopen(FindFileData.cFileName,"rb");
                if(a==NULL)
                {
                    for(veri=getc(okuyucu); veri!=EOF; veri=getc(okuyucu))//binary dosyanın okunması
                    {
                        if(veri=='\n')//bir satirin bitimi
                        {
                            satir_sayisi++;
                        }
                        if(satir_sayisi>=5)
                        {
                            while( fread(&arr, sizeof(arr), 1, okuyucu) == 1 )
                            {

                                dizi[w][q]=arr;//verilerin diziye atılması
                                q++;
                                if(q%3==0)
                                {
                                    w++;
                                    q=0;
                                }
                            }
                        }

                    }
                }
                else//rgb li binary ise bu işlem yapılır
                {
                    for(veri=getc(okuyucu); veri!=EOF; veri=getc(okuyucu))
                    {
                        if(veri=='\n')//bir satirin bitimi
                        {
                            satir_sayisi++;
                        }
                        if(satir_sayisi>=5)
                        {
                            while( fread(&arr, sizeof(arr), 1, okuyucu) == 1 )
                            {

                                dizi[w][q]=arr;
                                q++;
                                if(q%6==0)
                                {
                                    w++;
                                    q=0;
                                }
                            }
                        }

                    }
                }


                ek=sqrt(((dizi[0][0]-dizi[1][0])*(dizi[0][0]-dizi[1][0]))+((dizi[0][1]-dizi[1][1])*(dizi[0][1]-dizi[1][1]))+((dizi[0][2]-dizi[1][2])*(dizi[0][2]-dizi[1][2])));
                satirTutucu[0][0]=0;
                satirTutucu[1][0]=0;
                satirTutucu[0][1]=0;
                satirTutucu[1][1]=0;
                //ek=sqrt((matris[0][0]-matris[1][0])*(matris[0][0]-matris[1][0]));
                eb=ek;
                for(int z=0; z<w; z++)//en yakın en uzak bulma
                {
                    for(int h=z+1; h<w; h++)
                    {
                        oAnkiMesafe=sqrt(((dizi[z][0]-dizi[h][0])*(dizi[z][0]-dizi[h][0]))+((dizi[z][1]-dizi[h][1])*(dizi[z][1]-dizi[h][1]))+((dizi[z][2]-dizi[h][2])*(dizi[z][2]-dizi[h][2])));
                        if(ek>=oAnkiMesafe)
                        {
                            satirTutucu[0][0]=z;
                            satirTutucu[0][1]=h;
                            ek=oAnkiMesafe;
                            enYakin[0][0]=dizi[z][0];
                            enYakin[0][1]=dizi[z][1];
                            enYakin[0][2]=dizi[z][2];
                            enYakin[1][0]=dizi[h][0];
                            enYakin[1][1]=dizi[h][1];
                            enYakin[1][2]=dizi[h][2];
                        }
                        if(eb<=oAnkiMesafe)
                        {
                            satirTutucu[1][0]=z;
                            satirTutucu[1][1]=h;
                            eb=oAnkiMesafe;
                            enUzak[0][0]=dizi[z][0];
                            enUzak[0][1]=dizi[z][1];
                            enUzak[0][2]=dizi[z][2];
                            enUzak[1][0]=dizi[h][0];
                            enUzak[1][1]=dizi[h][1];
                            enUzak[1][2]=dizi[h][2];
                        }
                    }
                }
                printf("Birbirine en uzak olan noktalar:\n%d.Satir: %lf\t%lf\t%lf\n%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[1][0]+1,enUzak[0][0],enUzak[0][1],enUzak[0][2],satirTutucu[1][1]+1,enUzak[1][0],enUzak[1][1],enUzak[1][2]);
                printf("Birbirine en yakin olan noktalar:\n%d.Satir: %lf\t%lf\t%lf\n%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[0][0]+1,enYakin[0][0],enYakin[0][1],enYakin[0][2],satirTutucu[0][1]+1,enYakin[1][0],enYakin[1][1],enYakin[1][2]);
                fprintf(yazma,"\nBirbirine en yakin olan noktalar:\n%d.Satir: %lf\t%lf\t%lf\n%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[0][0]+1,enYakin[0][0],enYakin[0][1],enYakin[0][2],satirTutucu[0][1]+1,enYakin[1][0],enYakin[1][1],enYakin[1][2]);
                fprintf(yazma,"Birbirine en uzak olan noktalar:\n%d.Satir: %lf\t%lf\t%lf\n%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[1][0]+1,enUzak[0][0],enUzak[0][1],enUzak[0][2],satirTutucu[1][1]+1,enUzak[1][0],enUzak[1][1],enUzak[1][2]);

            }
            else//binary olmayan dosya işlemi
            {
                strcat(nameOfFile,FindFileData.cFileName);
                yazma=fopen(nameOfFile,"a");
                fprintf(yazma,"SECIM 2");
                double matris[(int)hata->satirSayisi[k]][3];//matris tanımlama
                k++;
                /* if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                 {
                     //ignore directories
                 }*/

                // else
                //{


                okuma=fopen(FindFileData.cFileName,"r");
                satir_sayisi=0;
                for(veri=getc(okuma); veri!=EOF; veri=getc(okuma))
                {
                    if(veri=='\n')//bir satirin bitimi
                    {
                        satir_sayisi++;
                    }

                    if(satir_sayisi>=5)
                    {
                        if(a=='r')
                        {
                            while(fscanf(okuma,"%lf%lf%lf%lf%lf%lf",&matris[i][0],&matris[i][1],&matris[i][2],&r,&g,&b)!=EOF)//verilerin matrise atılması rgb li
                            {
                                i++;
                            }
                        }
                        if(a==NULL)
                        {
                            while(fscanf(okuma,"%lf%lf%lf",&matris[i][0],&matris[i][1],&matris[i][2])!=EOF)//rgb siz verielrin matrise atılamsı
                            {
                                i++;
                            }
                        }
                    }

                }
                if(a==NULL)//rgb siz ise işlemler
                {
                    ek=sqrt(((matris[0][0]-matris[1][0])*(matris[0][0]-matris[1][0]))+((matris[0][1]-matris[1][1])*(matris[0][1]-matris[1][1]))+((matris[0][2]-matris[1][2])*(matris[0][2]-matris[1][2])));
                    satirTutucu[0][0]=0;
                    satirTutucu[1][0]=0;
                    satirTutucu[0][1]=0;
                    satirTutucu[1][1]=0;
                    //ek=sqrt((matris[0][0]-matris[1][0])*(matris[0][0]-matris[1][0]));
                    eb=ek;
                    for(int z=0; z<i; z++)
                    {
                        for(int h=z+1; h<i; h++)
                        {
                            oAnkiMesafe=sqrt(((matris[z][0]-matris[h][0])*(matris[z][0]-matris[h][0]))+((matris[z][1]-matris[h][1])*(matris[z][1]-matris[h][1]))+((matris[z][2]-matris[h][2])*(matris[z][2]-matris[h][2])));
                            if(ek>=oAnkiMesafe)
                            {
                                satirTutucu[0][0]=z;
                                satirTutucu[0][1]=h;
                                ek=oAnkiMesafe;
                                enYakin[0][0]=matris[z][0];
                                enYakin[0][1]=matris[z][1];
                                enYakin[0][2]=matris[z][2];
                                enYakin[1][0]=matris[h][0];
                                enYakin[1][1]=matris[h][1];
                                enYakin[1][2]=matris[h][2];
                            }
                            if(eb<=oAnkiMesafe)
                            {
                                satirTutucu[1][0]=z;
                                satirTutucu[1][1]=h;
                                eb=oAnkiMesafe;
                                enUzak[0][0]=matris[z][0];
                                enUzak[0][1]=matris[z][1];
                                enUzak[0][2]=matris[z][2];
                                enUzak[1][0]=matris[h][0];
                                enUzak[1][1]=matris[h][1];
                                enUzak[1][2]=matris[h][2];
                            }
                        }
                    }

                    printf("Birbirine en yakin olan noktalar:\n%d.Satir: %lf\t%lf\t%lf\n%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[0][0]+6,enYakin[0][0],enYakin[0][1],enYakin[0][2],satirTutucu[0][1]+6,enYakin[1][0],enYakin[1][1],enYakin[1][2]);

                    printf("Birbirine en uzak olan noktalar:\n%d.Satir: %lf\t%lf\t%lf\n%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[1][0]+6,enUzak[0][0],enUzak[0][1],enUzak[0][2],satirTutucu[1][1]+6,enUzak[1][0],enUzak[1][1],enUzak[1][2]);
                    fprintf(yazma,"\nBirbirine en yakin olan noktalar:\n%d.Satir: %lf\t%lf\t%lf\n%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[0][0]+6,enYakin[0][0],enYakin[0][1],enYakin[0][2],satirTutucu[0][1]+6,enYakin[1][0],enYakin[1][1],enYakin[1][2]);
                    fprintf(yazma,"Birbirine en uzak olan noktalar:\n%d.Satir: %lf\t%lf\t%lf\n%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[1][0]+6,enUzak[0][0],enUzak[0][1],enUzak[0][2],satirTutucu[1][1]+6,enUzak[1][0],enUzak[1][1],enUzak[1][2]);

                }
                else//rgb li işlemler
                {

                    ek=sqrt(((matris[0][0]-matris[1][0])*(matris[0][0]-matris[1][0]))+((matris[0][1]-matris[1][1])*(matris[0][1]-matris[1][1]))+((matris[0][2]-matris[1][2])*(matris[0][2]-matris[1][2])));
                    satirTutucu[0][0]=0;
                    satirTutucu[1][0]=0;
                    satirTutucu[0][1]=0;
                    satirTutucu[1][1]=0;
                    //ek=sqrt((matris[0][0]-matris[1][0])*(matris[0][0]-matris[1][0]));
                    eb=ek;
                    for(int z=0; z<i; z++)
                    {
                        for(int h=z+1; h<i; h++)
                        {
                            oAnkiMesafe=sqrt(((matris[z][0]-matris[h][0])*(matris[z][0]-matris[h][0]))+((matris[z][1]-matris[h][1])*(matris[z][1]-matris[h][1]))+((matris[z][2]-matris[h][2])*(matris[z][2]-matris[h][2])));
                            if(ek>=oAnkiMesafe)
                            {
                                satirTutucu[0][0]=z;
                                satirTutucu[0][1]=h;
                                ek=oAnkiMesafe;
                                enYakin[0][0]=matris[z][0];
                                enYakin[0][1]=matris[z][1];
                                enYakin[0][2]=matris[z][2];
                                /*enYakin[0][3]=matris[z][3];
                                enYakin[0][4]=matris[z][4];
                                enYakin[0][5]=matris[z][5];*/

                                enYakin[1][0]=matris[h][0];
                                enYakin[1][1]=matris[h][1];
                                enYakin[1][2]=matris[h][2];
                                /*enYakin[1][3]=matris[h][3];
                                enYakin[1][4]=matris[h][4];
                                enYakin[1][5]=matris[h][5];*/
                            }
                            if(eb<=oAnkiMesafe)
                            {
                                satirTutucu[1][0]=z;
                                satirTutucu[1][1]=h;
                                eb=oAnkiMesafe;
                                enUzak[0][0]=matris[z][0];
                                enUzak[0][1]=matris[z][1];
                                enUzak[0][2]=matris[z][2];
                                /*enUzak[0][3]=matris[z][3];
                                enUzak[0][4]=matris[z][4];
                                enUzak[0][5]=matris[z][5];*/

                                enUzak[1][0]=matris[h][0];
                                enUzak[1][1]=matris[h][1];
                                enUzak[1][2]=matris[h][2];
                                /*enUzak[1][3]=matris[h][3];
                                enUzak[1][4]=matris[h][4];
                                enUzak[1][5]=matris[h][5];*/
                            }
                        }
                    }

                    printf("Birbirine en yakin olan noktalar:\n%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[0][0]+6,enYakin[0][0],enYakin[0][1],enYakin[0][2]);
                    printf("%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[0][1]+6,enYakin[1][0],enYakin[1][1],enYakin[1][2]);

                    printf("Birbirine en uzak olan noktalar:\n%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[1][0]+6,enUzak[0][0],enUzak[0][1],enUzak[0][2]);
                    printf("%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[1][1]+6,enUzak[1][0],enUzak[1][1],enUzak[1][2]);

                    fprintf(yazma,"\nBirbirine en yakin olan noktalar:\n%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[0][0]+6,enYakin[0][0],enYakin[0][1],enYakin[0][2]);
                    fprintf(yazma,"%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[0][1]+6,enYakin[1][0],enYakin[1][1],enYakin[1][2]);

                    fprintf(yazma,"Birbirine en uzak olan noktalar:\n%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[1][0]+6,enUzak[0][0],enUzak[0][1],enUzak[0][2]);
                    fprintf(yazma,"%d.Satir: %lf\t%lf\t%lf\n",satirTutucu[1][1]+6,enUzak[1][0],enUzak[1][1],enUzak[1][2]);
                }
                fclose(okuma);
            }

        }
    }

}
void kup(struct hatali *hata)
{
    double EksenBuyuk[3];//x,y,z için ayrı ayrı en büyük buluyoruz.
    double EksenKucuk[3];//x,y,z için ayrı ayrı en küçük buluyoruz.
    double enBuyuk;//bu en büyükler içerisindeki en büyük.
    double enKucuk;//bu en küçükle içerisindeki en küçük.
    FILE *yazma;
    FILE *binary;
    //int satirTutucu[2][2];
    int v=0;
    int j=0;
    int k=0;

    FILE *okuma;
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    TCHAR  *directory = TEXT(".");
    TCHAR patter[MAX_PATH];

    memset(patter, 0x00, MAX_PATH);
    _stprintf(patter, TEXT("%s\\*.nkt"), directory);
    hFind = FindFirstFile(patter, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("FindFirstFile failed (%d)\n", GetLastError());
    }
    else
    {
        while (FindNextFile(hFind, &FindFileData))
        {
            int w=0,q=0,x=0;
            int boslukSayisi2=0;
            float arr;
            float dizi[20][6];
            char tipTutucu[6];
            double toplam=0;
            char a=NULL;
            int i=0;
            int r,g,b;
            char veri;
            int  satir_sayisi=0;
            int boslukSayisi=0;
            char nameOfFile[50]="Output-";

            if(strcmp(FindFileData.cFileName,hata->fileName[j])==0)
            {
                printf("%s isimli dosya hatalidir. islem yapilmaz\n\n",FindFileData.cFileName);
                j++;
                continue;
            }
            if(strstr(FindFileData.cFileName,"Output")!=0)
            {
                continue;
            }

            binary=fopen(FindFileData.cFileName,"r");

            for(veri=getc(binary); veri!=EOF; veri=getc(binary))
            {
                if(veri=='\n')//bir satirin bitimi
                {
                    satir_sayisi++;
                }
                if(satir_sayisi==2)
                {
                    boslukSayisi++;
                    if(boslukSayisi==16)
                    {
                        a=veri;
                    }
                }
                if(satir_sayisi==4)
                {
                    boslukSayisi2++;
                    if(boslukSayisi2>=6)
                    {
                        tipTutucu[x]=veri;
                        x++;
                    }
                }
            }
            // printf("%s\n",tipTutucu);
            fclose(binary);
            satir_sayisi=0;
            if(strstr(tipTutucu,"binary")!=0)
            {
                strcat(nameOfFile,FindFileData.cFileName);
                yazma=fopen(nameOfFile,"a");
                fprintf(yazma,"SECIM 3");
                FILE *okuyucu;
                okuyucu=fopen(FindFileData.cFileName,"rb");
                if(a==NULL)
                {
                    for(veri=getc(okuyucu); veri!=EOF; veri=getc(okuyucu))
                    {
                        if(veri=='\n')//bir satirin bitimi
                        {
                            satir_sayisi++;
                        }
                        if(satir_sayisi>=5)
                        {
                            while( fread(&arr, sizeof(arr), 1, okuyucu) == 1 )
                            {

                                dizi[w][q]=arr;
                                q++;
                                if(q%3==0)
                                {
                                    w++;
                                    q=0;
                                }
                            }
                        }
                    }
                }
                else
                {
                    for(veri=getc(okuyucu); veri!=EOF; veri=getc(okuyucu))
                    {
                        if(veri=='\n')//bir satirin bitimi
                        {
                            satir_sayisi++;
                        }
                        if(satir_sayisi>=5)
                        {
                            while( fread(&arr, sizeof(arr), 1, okuyucu) == 1 )
                            {

                                dizi[w][q]=arr;
                                q++;
                                if(q%6==0)
                                {
                                    w++;
                                    q=0;
                                }
                            }
                        }
                    }



                }

                EksenBuyuk[0]=dizi[0][0];//x,y,z lerin 1. satırdaki elemanlarını atadık en büüyğü bulmak için.
                EksenBuyuk[1]=dizi[0][1];
                EksenBuyuk[2]=dizi[0][2];

                EksenKucuk[0]=dizi[0][0];//x,y,z lerin 1. satırdaki elemanlarını atadık en Küçüğü bulmak için
                EksenKucuk[1]=dizi[0][1];
                EksenKucuk[2]=dizi[0][2];



                for(int z=0; z<w; z++)//En Büyükleri bulduk
                {
                    if(dizi[z][0]>=EksenBuyuk[0])
                    {
                        EksenBuyuk[0]=dizi[z][0];
                    }
                    if(dizi[z][1]>=EksenBuyuk[1])
                    {
                        EksenBuyuk[1]=dizi[z][1];
                    }
                    if(dizi[z][2]>=EksenBuyuk[2])
                    {
                        EksenBuyuk[2]=dizi[z][2];
                    }
                }

                for(int z=0; z<w; z++)
                {
                    if(dizi[z][0]<=EksenKucuk[0])
                    {
                        EksenKucuk[0]=dizi[z][0];
                    }
                    if(dizi[z][1]<=EksenKucuk[1])
                    {
                        EksenKucuk[1]=dizi[z][1];
                    }
                    if(dizi[z][2]<=EksenKucuk[2])
                    {
                        EksenKucuk[2]=dizi[z][2];
                    }
                }
                enBuyuk=EksenBuyuk[0];
                enKucuk=EksenKucuk[0];
                for(int b=0; b<3; b++)
                {
                    if(EksenBuyuk[b]>=enBuyuk)
                    {
                        enBuyuk=EksenBuyuk[b];
                    }
                    if(EksenKucuk[b]<=enKucuk)
                    {
                        enKucuk=EksenKucuk[b];
                    }
                }
                printf("%s isimli dosyanin kup kose noktalari\n",FindFileData.cFileName);
                printf("%lf\t%lf\t%lf\n",enBuyuk,enKucuk,enBuyuk);
                printf("%lf\t%lf\t%lf\n",enBuyuk,enBuyuk,enBuyuk);
                printf("%lf\t%lf\t%lf\n",enBuyuk,enBuyuk,enKucuk);
                printf("%lf\t%lf\t%lf\n",enBuyuk,enKucuk,enKucuk);
                printf("%lf\t%lf\t%lf\n",enKucuk,enKucuk,enKucuk);
                printf("%lf\t%lf\t%lf\n",enKucuk,enBuyuk,enBuyuk);
                printf("%lf\t%lf\t%lf\n",enKucuk,enBuyuk,enKucuk);
                printf("%lf\t%lf\t%lf\n",enKucuk,enKucuk,enBuyuk);

                fprintf(yazma,"\n%lf\t%lf\t%lf\n",enBuyuk,enKucuk,enBuyuk);
                fprintf(yazma,"%lf\t%lf\t%lf\n",enBuyuk,enBuyuk,enBuyuk);
                fprintf(yazma,"%lf\t%lf\t%lf\n",enBuyuk,enBuyuk,enKucuk);
                fprintf(yazma,"%lf\t%lf\t%lf\n",enBuyuk,enKucuk,enKucuk);
                fprintf(yazma,"%lf\t%lf\t%lf\n",enKucuk,enKucuk,enKucuk);
                fprintf(yazma,"%lf\t%lf\t%lf\n",enKucuk,enBuyuk,enBuyuk);
                fprintf(yazma,"%lf\t%lf\t%lf\n",enKucuk,enBuyuk,enKucuk);
                fprintf(yazma,"%lf\t%lf\t%lf\n",enKucuk,enKucuk,enBuyuk);
            }




            else
            {
                //ignore current and parent directories
                /*  if(_tcscmp(FindFileData.cFileName, TEXT("."))==0 || _tcscmp(FindFileData.cFileName, TEXT(".."))==0)
                      continue;*/
                okuma=fopen(FindFileData.cFileName,"r");
                strcat(nameOfFile,FindFileData.cFileName);
                yazma=fopen(nameOfFile,"a");
                fprintf(yazma,"SECIM 3");
                double matris[(int)hata->satirSayisi[k]][3];
                k++;
                /* if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                 {
                     //ignore directories
                 }*/

                // else
                //{
                for(veri=getc(okuma); veri!=EOF; veri=getc(okuma))
                {
                    if(veri=='\n')//bir satirin bitimi
                    {
                        satir_sayisi++;
                    }


                    if(satir_sayisi>=5)
                    {
                        if(a=='r')
                        {
                            while(fscanf(okuma,"%lf%lf%lf%lf%lf%lf",&matris[i][0],&matris[i][1],&matris[i][2],&r,&g,&b)!=EOF)
                            {
                                i++;
                            }
                        }
                        if(a==NULL)
                        {
                            while(fscanf(okuma,"%lf%lf%lf",&matris[i][0],&matris[i][1],&matris[i][2])!=EOF)
                            {
                                i++;
                            }
                        }
                    }

                }

                EksenBuyuk[0]=matris[0][0];//x,y,z lerin 1. satırdaki elemanlarını atadık en büüyğü bulmak için.
                EksenBuyuk[1]=matris[0][1];
                EksenBuyuk[2]=matris[0][2];

                EksenKucuk[0]=matris[0][0];//x,y,z lerin 1. satırdaki elemanlarını atadık en Küçüğü bulmak için.
                EksenKucuk[1]=matris[0][1];
                EksenKucuk[2]=matris[0][2];



                for(int z=0; z<i; z++)//en büyük değeri bulma işlemi
                {
                    if(matris[z][0]>=EksenBuyuk[0])
                    {
                        EksenBuyuk[0]=matris[z][0];
                    }
                    if(matris[z][1]>=EksenBuyuk[1])
                    {
                        EksenBuyuk[1]=matris[z][1];
                    }
                    if(matris[z][2]>=EksenBuyuk[2])
                    {
                        EksenBuyuk[2]=matris[z][2];
                    }
                }
                for(int z=0; z<i; z++)//en büyük değeri bulma işlemi
                {
                    if(matris[z][0]<=EksenKucuk[0])
                    {
                        EksenKucuk[0]=matris[z][0];
                    }
                    if(matris[z][1]<=EksenKucuk[1])
                    {
                        EksenKucuk[1]=matris[z][1];
                    }
                    if(matris[z][2]<=EksenKucuk[2])
                    {
                        EksenKucuk[2]=matris[z][2];
                    }
                }
                enBuyuk=EksenBuyuk[0];
                enKucuk=EksenKucuk[0];
                for(int b=0; b<3; b++)
                {
                    if(EksenBuyuk[b]>=enBuyuk)
                    {
                        enBuyuk=EksenBuyuk[b];
                    }
                    if(EksenKucuk[b]<=enKucuk)
                    {
                        enKucuk=EksenKucuk[b];
                    }
                }
                printf("%s isimli dosyanin kup kose noktalari\n",FindFileData.cFileName);
                printf("%lf\t%lf\t%lf\n",enBuyuk,enKucuk,enBuyuk);
                printf("%lf\t%lf\t%lf\n",enBuyuk,enBuyuk,enBuyuk);
                printf("%lf\t%lf\t%lf\n",enBuyuk,enBuyuk,enKucuk);
                printf("%lf\t%lf\t%lf\n",enBuyuk,enKucuk,enKucuk);
                printf("%lf\t%lf\t%lf\n",enKucuk,enKucuk,enKucuk);
                printf("%lf\t%lf\t%lf\n",enKucuk,enBuyuk,enBuyuk);
                printf("%lf\t%lf\t%lf\n",enKucuk,enBuyuk,enKucuk);
                printf("%lf\t%lf\t%lf\n",enKucuk,enKucuk,enBuyuk);

                fprintf(yazma,"\n%lf\t%lf\t%lf\n",enBuyuk,enKucuk,enBuyuk);
                fprintf(yazma,"%lf\t%lf\t%lf\n",enBuyuk,enBuyuk,enBuyuk);
                fprintf(yazma,"%lf\t%lf\t%lf\n",enBuyuk,enBuyuk,enKucuk);
                fprintf(yazma,"%lf\t%lf\t%lf\n",enBuyuk,enKucuk,enKucuk);
                fprintf(yazma,"%lf\t%lf\t%lf\n",enKucuk,enKucuk,enKucuk);
                fprintf(yazma,"%lf\t%lf\t%lf\n",enKucuk,enBuyuk,enBuyuk);
                fprintf(yazma,"%lf\t%lf\t%lf\n",enKucuk,enBuyuk,enKucuk);
                fprintf(yazma,"%lf\t%lf\t%lf\n",enKucuk,enKucuk,enBuyuk);
            }



        }
    }
}
void kure(struct hatali *hata)
{

    double cx,cy,cz,cr;
    printf("\nKurenin merkezinin x noktasini giriniz: ");
    scanf("%lf",&cx);
    printf("\nKurenin merkezinin y noktasini giriniz: ");
    scanf("%lf",&cy);
    printf("\nKurenin merkezinin z noktasini giriniz: ");
    scanf("%lf",&cz);
    printf("\nKurenin yaricapini giriniz: ");
    scanf("%lf",&cr);
    FILE *okuma;
    FILE *binary;
    int satirTutucu[2][2];
    int j=0;
    int k=0;

    FILE *yazma;
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    TCHAR  *directory = TEXT(".");
    TCHAR patter[MAX_PATH];

    memset(patter, 0x00, MAX_PATH);
    _stprintf(patter, TEXT("%s\\*.nkt"), directory);
    hFind = FindFirstFile(patter, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("FindFirstFile failed (%d)\n", GetLastError());
    }
    else
    {
        while (FindNextFile(hFind, &FindFileData))
        {
            int w=0,q=0,x=0;
            int boslukSayisi2=0;
            float arr;
            float dizi[20][6];
            char tipTutucu[6];

            char a=NULL;
            int i=0;
            int r,g,b;
            char veri;
            int  satir_sayisi=0;
            int boslukSayisi=0;
            char nameOfFile[50]="Output-";
            if(strstr(FindFileData.cFileName,"Output")!=0)
            {
                continue;
            }
            if(strcmp(FindFileData.cFileName,hata->fileName[j])==0)
            {
                printf("%s isimli dosya hatalidir. islem yapilmaz\n\n",FindFileData.cFileName);
                j++;
                continue;
            }

            binary=fopen(FindFileData.cFileName,"r");

            for(veri=getc(binary); veri!=EOF; veri=getc(binary))
            {
                if(veri=='\n')//bir satirin bitimi
                {
                    satir_sayisi++;
                }
                if(satir_sayisi==2)
                {
                    boslukSayisi++;
                    if(boslukSayisi==16)
                    {
                        a=veri;
                    }
                }
                if(satir_sayisi==4)
                {
                    boslukSayisi2++;
                    if(boslukSayisi2>=6)
                    {
                        tipTutucu[x]=veri;
                        x++;
                    }
                }
            }
            // printf("%s\n",tipTutucu);
            fclose(binary);
            satir_sayisi=0;
            if(strstr(tipTutucu,"binary")!=0)
            {
                strcat(nameOfFile,FindFileData.cFileName);
                yazma=fopen(nameOfFile,"a");

                fprintf(yazma,"SECIM 4\n");
                fprintf(yazma,"cx:%lf\n",cx);
                fprintf(yazma,"cy:%lf\n",cy);
                fprintf(yazma,"cz:%lf\n",cz);
                fprintf(yazma,"cr:%lf\n",cr);


                FILE *okuyucu;
                okuyucu=fopen(FindFileData.cFileName,"rb");

                /****************************************************/


                if(a==NULL)
                {
                    for(veri=getc(okuyucu); veri!=EOF; veri=getc(okuyucu))
                    {
                        if(veri=='\n')//bir satirin bitimi
                        {
                            satir_sayisi++;
                        }
                        if(satir_sayisi>=5)
                        {
                            while( fread(&arr, sizeof(arr), 1, okuyucu) == 1 )
                            {

                                dizi[w][q]=arr;
                                q++;
                                if(q%3==0)
                                {
                                    w++;
                                    q=0;
                                }
                            }
                        }

                    }
                    for(int z=0; z<w; z++)
                    {
                        double mesafe=sqrt(pow(dizi[z][0]-cx,2)+pow(dizi[z][1]-cy,2)+pow(dizi[z][2]-cz,2));//O noktanın küre içinde olması için kürenin merkezine olan uzaklığı yarıçapından daha küçük olmalı

                        if(mesafe<=cr)
                        {
                            printf("%lf\t%lf\t%lf\n",dizi[z][0],dizi[z][1],dizi[z][2]);
                            fprintf(yazma,"%lf\t%lf\t%lf\n",dizi[z][0],dizi[z][1],dizi[z][2]);
                        }

                    }
                }
                else
                {
                    for(veri=getc(okuyucu); veri!=EOF; veri=getc(okuyucu))
                    {
                        if(veri=='\n')//bir satirin bitimi
                        {
                            satir_sayisi++;
                        }
                        if(satir_sayisi>=5)
                        {
                            while( fread(&arr, sizeof(arr), 1, okuyucu) == 1 )
                            {

                                dizi[w][q]=arr;
                                q++;
                                if(q%6==0)
                                {
                                    w++;
                                    q=0;
                                }
                            }
                        }

                    }
                    for(int z=0; z<w; z++)
                    {
                        double mesafe=sqrt(pow(dizi[z][0]-cx,2)+pow(dizi[z][1]-cy,2)+pow(dizi[z][2]-cz,2));//O noktanın küre içinde olması için kürenin merkezine olan uzaklığı yarıçapından daha küçük olmalı


                        if(mesafe<=cr)
                        {
                            printf("%lf\t%lf\t%lf\n",dizi[z][0],dizi[z][1],dizi[z][2]);
                            fprintf(yazma,"%lf\t%lf\t%lf\n",dizi[z][0],dizi[z][1],dizi[z][2]);
                        }

                    }

                }

                /****************************************************/
            }
            else
            {


                //ignore current and parent directories
                /*  if(_tcscmp(FindFileData.cFileName, TEXT("."))==0 || _tcscmp(FindFileData.cFileName, TEXT(".."))==0)
                      continue;*/
                okuma=fopen(FindFileData.cFileName,"r");

                strcat(nameOfFile,FindFileData.cFileName);
                yazma=fopen(nameOfFile,"a");
                fprintf(yazma,"SECIM 4\n");
                fprintf(yazma,"cx:%lf\n",cx);
                fprintf(yazma,"cy:%lf\n",cy);
                fprintf(yazma,"cz:%lf\n",cz);
                fprintf(yazma,"cr:%lf\n",cr);
                double matris[(int)hata->satirSayisi[k]][3];
                k++;
                /* if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                 {
                     //ignore directories
                 }*/

                // else
                //{
                for(veri=getc(okuma); veri!=EOF; veri=getc(okuma))
                {
                    if(veri=='\n')//bir satirin bitimi
                    {
                        satir_sayisi++;
                    }


                    if(satir_sayisi>=5)
                    {
                        if(a=='r')
                        {
                            while(fscanf(okuma,"%lf%lf%lf%lf%lf%lf",&matris[i][0],&matris[i][1],&matris[i][2],&r,&g,&b)!=EOF)
                            {
                                i++;
                            }
                        }
                        if(a==NULL)
                        {
                            while(fscanf(okuma,"%lf%lf%lf",&matris[i][0],&matris[i][1],&matris[i][2])!=EOF)
                            {
                                i++;
                            }
                        }
                    }

                }
                if(a==NULL)
                {
                    for(int z=0; z<i; z++)
                    {
                        double mesafe=sqrt(pow(matris[z][0]-cx,2)+pow(matris[z][1]-cy,2)+pow(matris[z][2]-cz,2));//O noktanın küre içinde olması için kürenin merkezine olan uzaklığı yarıçapından daha küçük olmalı


                        if(mesafe<=cr)
                        {
                            printf("%lf\t%lf\t%lf\n",matris[z][0],matris[z][1],matris[z][2]);
                            fprintf(yazma,"%lf\t%lf\t%lf\n",matris[z][0],matris[z][1],matris[z][2]);
                        }

                    }
                }
                else
                {
                    for(int z=0; z<i; z++)
                    {
                        double mesafe=sqrt(pow(matris[z][0]-cx,2)+pow(matris[z][1]-cy,2)+pow(matris[z][2]-cz,2));//O noktanın küre içinde olması için kürenin merkezine olan uzaklığı yarıçapından daha küçük olmalı


                        if(mesafe<=cr)
                        {
                            printf("%lf\t%lf\t%lf\n",matris[z][0],matris[z][1],matris[z][2]);
                            fprintf(yazma,"%lf\t%lf\t%lf\n",matris[z][0],matris[z][1],matris[z][2]);
                        }

                    }

                }
            }

        }
    }
}
void uzaklik(struct hatali *hata)
{
    double oAnkiMesafe=0;
    int satirTutucu[2][2];
    int v=0;
    int j=0;
    int k=0;

    FILE *yazma;
    FILE *okuma;
    FILE *binary;
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    TCHAR  *directory = TEXT(".");
    TCHAR patter[MAX_PATH];

    memset(patter, 0x00, MAX_PATH);
    _stprintf(patter, TEXT("%s\\*.nkt"), directory);
    hFind = FindFirstFile(patter, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("FindFirstFile failed (%d)\n", GetLastError());
    }
    else
    {
        while (FindNextFile(hFind, &FindFileData))
        {
            int w=0,q=0,x=0;
            int boslukSayisi2=0;
            float arr;
            float dizi[20][6];
            char tipTutucu[6];

            double toplam=0;
            char a=NULL;
            int i=0;
            int r,g,b;
            char veri;
            int  satir_sayisi=0;
            int boslukSayisi=0;
            char nameOfFile[50]="Output-";

            if(strcmp(FindFileData.cFileName,hata->fileName[j])==0)
            {
                printf("%s isimli dosya hatalidir. islem yapilmaz\n\n",FindFileData.cFileName);
                j++;
                continue;
            }


            //ignore current and parent directories
            /*  if(_tcscmp(FindFileData.cFileName, TEXT("."))==0 || _tcscmp(FindFileData.cFileName, TEXT(".."))==0)
                  continue;*/

            if(strstr(FindFileData.cFileName,"Output")!=0)
            {
                continue;
            }

            binary=fopen(FindFileData.cFileName,"r");

            for(veri=getc(binary); veri!=EOF; veri=getc(binary))
            {
                if(veri=='\n')//bir satirin bitimi
                {
                    satir_sayisi++;
                }
                if(satir_sayisi==2)
                {
                    boslukSayisi++;
                    if(boslukSayisi==16)
                    {
                        a=veri;
                    }
                }
                if(satir_sayisi==4)
                {
                    boslukSayisi2++;
                    if(boslukSayisi2>7)
                    {
                        tipTutucu[x]=veri;
                        x++;
                    }
                }
            }
            //printf("%s\n",tipTutucu);
            fclose(binary);
            satir_sayisi=0;
            if(strstr(tipTutucu,"nary")!=0)
            {
                // printf("%s@@@@@@@\n",nameOfFile);
                strcat(nameOfFile,FindFileData.cFileName);
                //printf("%s@@@@@@@\n",nameOfFile);
                yazma=fopen(nameOfFile,"a");
                fprintf(yazma,"SECIM 5\n");

                FILE *okuyucu;
                okuyucu=fopen(FindFileData.cFileName,"rb");
                if(a==NULL)
                {
                    for(veri=getc(okuyucu); veri!=EOF; veri=getc(okuyucu))
                    {
                        if(veri=='\n')//bir satirin bitimi
                        {
                            satir_sayisi++;
                        }
                        if(satir_sayisi>=5)
                        {
                            while( fread(&arr, sizeof(arr), 1, okuyucu) == 1 )
                            {

                                dizi[w][q]=arr;
                                q++;
                                if(q%3==0)
                                {
                                    w++;
                                    q=0;
                                }
                            }
                        }
                    }

                    for(int z=0; z<w; z++)
                    {
                        for(int h=z+1; h<w; h++)
                        {
                            oAnkiMesafe=sqrt(((dizi[z][0]-dizi[h][0])*(dizi[z][0]-dizi[h][0]))+((dizi[z][1]-dizi[h][1])*(dizi[z][1]-dizi[h][1]))+((dizi[z][2]-dizi[h][2])*(dizi[z][2]-dizi[h][2])));
                            toplam+=oAnkiMesafe;
                        }
                    }
                    double sonuc=(toplam/(((w)*(w-1))/2));

                    printf("%s nin ortalamasi: %lf\n",FindFileData.cFileName,sonuc);
                    fprintf(yazma,"%s nin ortalamasi: %lf\n",FindFileData.cFileName,sonuc);

                }
                else
                {
                    for(veri=getc(okuyucu); veri!=EOF; veri=getc(okuyucu))
                    {
                        if(veri=='\n')//bir satirin bitimi
                        {
                            satir_sayisi++;
                        }
                        if(satir_sayisi>=5)
                        {
                            while( fread(&arr, sizeof(arr), 1, okuyucu) == 1 )
                            {

                                dizi[w][q]=arr;
                                q++;
                                if(q%6==0)
                                {
                                    w++;
                                    q=0;
                                }
                            }
                        }
                    }

                    for(int z=0; z<w; z++)
                    {
                        for(int h=z+1; h<w; h++)
                        {
                            oAnkiMesafe=sqrt(((dizi[z][0]-dizi[h][0])*(dizi[z][0]-dizi[h][0]))+((dizi[z][1]-dizi[h][1])*(dizi[z][1]-dizi[h][1]))+((dizi[z][2]-dizi[h][2])*(dizi[z][2]-dizi[h][2])));
                            toplam+=oAnkiMesafe;
                        }
                    }
                    double sonuc=(toplam/(((w)*(w-1))/2));

                    printf("%s nin ortalamasi: %lf\n",FindFileData.cFileName,sonuc);
                    fprintf(yazma,"%s nin ortalamasi: %lf\n",FindFileData.cFileName,sonuc);
                }

            }
            else
            {
                okuma=fopen(FindFileData.cFileName,"r");
                strcat(nameOfFile,FindFileData.cFileName);
                yazma=fopen(nameOfFile,"a");
                fprintf(yazma,"SECIM 5\n");
                double matris[(int)hata->satirSayisi[k]][3];
                k++;
                /* if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                 {
                     //ignore directories
                 }*/

                // else
                //{
                for(veri=getc(okuma); veri!=EOF; veri=getc(okuma))
                {
                    if(veri=='\n')//bir satirin bitimi
                    {
                        satir_sayisi++;
                    }


                    if(satir_sayisi>=5)
                    {
                        if(a=='r')
                        {
                            while(fscanf(okuma,"%lf%lf%lf%lf%lf%lf",&matris[i][0],&matris[i][1],&matris[i][2],&r,&g,&b)!=EOF)
                            {
                                i++;
                            }
                        }
                        if(a==NULL)
                        {
                            while(fscanf(okuma,"%lf%lf%lf",&matris[i][0],&matris[i][1],&matris[i][2])!=EOF)
                            {
                                i++;
                            }
                        }
                    }

                }



                for(int z=0; z<i; z++)
                {
                    for(int h=z+1; h<i; h++)
                    {
                        oAnkiMesafe=sqrt(((matris[z][0]-matris[h][0])*(matris[z][0]-matris[h][0]))+((matris[z][1]-matris[h][1])*(matris[z][1]-matris[h][1]))+((matris[z][2]-matris[h][2])*(matris[z][2]-matris[h][2])));
                        toplam+=oAnkiMesafe;
                    }
                }
                double sonuc=(toplam/(double)(((hata->satirSayisi[v])*(hata->satirSayisi[v]-1))/2));

                printf("%s nin ortalamasi: %lf\n",FindFileData.cFileName,sonuc);
                fprintf(yazma,"%s nin ortalamasi: %lf\n",FindFileData.cFileName,sonuc);
                v++;
            }


        }
    }
}
