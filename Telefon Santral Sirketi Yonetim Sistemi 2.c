#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


struct Abone
{
    int tel_no;
    char ad[11];
    char soyad[13];
    char adres[26];
    char abone_tipi;
    struct Abone *tele_gore_sonraki;
    struct Abone *isme_gore_sonraki;
    struct Gorusme *gorusmePtr;//Gorusmelerin tutuldugu bagli listenin baslangic dugum adresini gosterir.
};


struct Gorusme
{
    char tarih[9];
    char aranan_no[11];
    char bas_zamani[5];
    int konusma_suresi;
    struct Gorusme *sonraki;
    struct Gorusme *onceki;
};


typedef struct Abone abone;
typedef struct Gorusme gorusme;
//fonksiyon prototipleri tanimlandi.
void menu();
int secim_menu();
char menuden_cikis();
int tele_gore_hash(int);
int soyadina_gore_hash(char );
abone *tele_gore_arama(abone *,int);
abone *ada_gore_arama(abone,char [13]);
void tele_gore_sirali_ekleme(abone **,abone *);
void soyadina_gore_sirali_ekleme(abone *,abone *);
int soyad_karsilastirma(char [13],char [13]);
void tele_gore_silme(abone **, abone *);
void gorusmenin_tarihinin_cozumlemesi(int *,int *,int *,char [9]);
void nakil_islemleri(abone *[900],int );
void gorusme_ekle(abone *);
void telefon_numarasina_gore_gorusme_sil(abone *);
void ada_gore_abone_silme(abone *,int);
void bir_aboneye_ait_gorusmelerin_listesi(abone *);
void bir_aboneye_ait_ilk_uc_gorusmenin_listesi(abone *);
void ilk_uc_basamaga_gore_bilgi_listeleme(abone *);
void soyadin_bas_harfine_gore_ekrana_yazdirma(abone *);
void soyadin_bir_parcasina_gore_ekrana_yazdirma(abone *,char [13]);
void abone_tipi_donusturme(char,char  [9]);
void tum_abonelerin_listesi(abone *[900]);
void abonelerin_gorusme_analizi(abone *,int [5],int [5]);
void bir_tarihten_beri_gorusme_yapmayanlar(abone *[900],char [9]);
int aranan_tel_no_donusum(char [11]);
void bir_numarayi_arayan_kullanicilarin_listesi(abone [26],int);
void bir_tarihteki_gorusmeler(abone *[900],char [9]);



int main()
{
    char cikis='h',harf,soyad_parca[13],abone_tip[9],tarih[9],cevap;
    int tel_no,hash_tel,hash_isim,i;
    abone *telefona_gore_hash_tablosu[900]= {NULL}; //Telefonlari ilk uc hanesine gore siniflandirip bagli listelerde tuttugumuz hash tablosu.
    abone ada_gore_hash_tablosu[26];//Abonelerin soyadlarinin bas harflerine gore siniflandirip dairesel bagli listelerde tuttugumuz hash tablosu.
    abone *yeni_abone;
    for(i=0; i<26; i++) //dairesel listenin ilk dugumleri ilkleniyor..
        ada_gore_hash_tablosu[i].isme_gore_sonraki=&ada_gore_hash_tablosu[i];

    do
    {
        switch(secim_menu())
        {
        case 1:
            printf("Eklemek istediginiz abonenin numarasini giriniz:\n");
            scanf("%d",&tel_no);
            hash_tel=tele_gore_hash(tel_no);//telefona gore hash anahtar sayi uretiliyor..
            if(tele_gore_arama(telefona_gore_hash_tablosu[hash_tel],tel_no)==NULL) //Girilen numarada abone kayitli degilse islemler yapiliyor..
            {
                yeni_abone=malloc(sizeof(abone));//olusturulacak yeni abone icin yer tahsis ediliyor..
                yeni_abone->tel_no=tel_no;
                printf("Abonenin adini giriniz:\n");
                fflush(stdin);
                gets(yeni_abone->ad);
                printf("Abonenin soyadini giriniz:\n");
                fflush(stdin);
                gets(yeni_abone->soyad);
                printf("Abonenin adresini giriniz:\n");
                fflush(stdin);
                gets(yeni_abone->adres);
                printf("Abonenin abone tipini giriniz[S:Sahis,I:Isyeri,K:Kamu,G:Guvenlik]:\n");
                fflush(stdin);
                scanf("%c",&yeni_abone->abone_tipi);
                //cift bagli dairesel liste ilkleniyor ve yer tahsisi yapiliyor..
                yeni_abone->gorusmePtr=malloc(sizeof(gorusme));
                yeni_abone->gorusmePtr->sonraki=yeni_abone->gorusmePtr;
                yeni_abone->gorusmePtr->onceki=yeni_abone->gorusmePtr;
                //olusturulan abonenin ardindan isme ve tele gore listelere ekleniyor..
                tele_gore_sirali_ekleme(&telefona_gore_hash_tablosu[hash_tel],yeni_abone);
                hash_isim=soyadina_gore_hash(yeni_abone->soyad[0]);
                soyadina_gore_sirali_ekleme(&ada_gore_hash_tablosu[hash_isim],yeni_abone);
            }
            break;
        case 2:
            printf("Nakil yapmak istediginiz abonenin numarasini giriniz:\n");
            scanf("%d",&tel_no);
            nakil_islemleri(telefona_gore_hash_tablosu,tel_no);
            break;
        case 3:
            printf("Gorusmedeki arayan telefon numarasini giriniz:\n");
            scanf("%d",&tel_no);
            do
            {
                //Telefona gore hash degeri bulunup o indiste bizim istedigimiz telefon numarasi kaydi olup olmadigi kontrol ediliyor..
                hash_tel=tele_gore_hash(tel_no);
                yeni_abone=tele_gore_arama(telefona_gore_hash_tablosu[hash_tel],tel_no);
                if(yeni_abone!=NULL) //O abone varsa gorusme ekleniyor..
                {
                    gorusme_ekle(yeni_abone);
                    printf("Gorusme eklendi.\n");
                }
                else printf("Boyle bir numara kayitli degil!Gorusme ekleyemezsiniz!\n");

                printf("Baska gorusme eklemek istiyor musunuz?[Evet:(E/e),Hayir:(H/h)]\n");
                fflush(stdin);
                cevap=getchar();
            }
            while(cevap=='e'||cevap=='E');
            break;
        case 4:
            printf("Silinmesini istediginiz abonenin telefon numarasini giriniz:\n");
            scanf("%d",&tel_no);

            hash_tel=tele_gore_hash(tel_no);
            yeni_abone=tele_gore_arama(telefona_gore_hash_tablosu[hash_tel],tel_no);
            if(yeni_abone!=NULL) //O abone varsa hem isme gore hemde tele gore listelerden siliniyor..
            {
                telefon_numarasina_gore_gorusme_sil(yeni_abone);
                tele_gore_silme(&telefona_gore_hash_tablosu[hash_tel],yeni_abone);
                hash_isim=soyadina_gore_hash(yeni_abone->soyad[0]);
                ada_gore_abone_silme(&ada_gore_hash_tablosu[hash_isim],tel_no);
                printf("abone silindi.\n");
            }
            else printf("%d numarali abone bulunamadi!\n",tel_no);
            break;
        case 5:
            printf("Bilgilerini goruntulemek istediginiz abonenin telefon numarasini giriniz:\n");
            scanf("%d",&tel_no);

            hash_tel=tele_gore_hash(tel_no);
            yeni_abone=tele_gore_arama(telefona_gore_hash_tablosu[hash_tel],tel_no);
            if(yeni_abone!=NULL) //Bu numarali abone varsa bilgisi ekrana yazdiriliyor ve gorusmeleri icin fonksiyon cagriliyor..
            {
                abone_tipi_donusturme(yeni_abone->abone_tipi,abone_tip);
                printf("%-7s %-10s %-12s %-25s %-10s\n","Tel No","Adi","Soyadi","Adresi","Abone Tipi");
                printf("------- ---------- ------------ ------------------------- ----------\n");
                printf("%-7d %-10s %-12s %-25s %-10s\n",tel_no,yeni_abone->ad,yeni_abone->soyad,yeni_abone->adres,abone_tip);
                bir_aboneye_ait_gorusmelerin_listesi(yeni_abone);
            }
            else printf("Bu numaraya kayitli bir abone yok!\n");
            break;

        case 6:
            printf("Bilgilerini goruntulemek istediginiz abonenin telefon numarasini giriniz:\n");
            scanf("%d",&tel_no);

            hash_tel=tele_gore_hash(tel_no);
            yeni_abone=tele_gore_arama(telefona_gore_hash_tablosu[hash_tel],tel_no);
            if(yeni_abone!=NULL) //Bu numarali abone varsa bilgisi ekrana yazdiriliyor ve gorusmeleri icin fonksiyon cagriliyor..
            {
                abone_tipi_donusturme(yeni_abone->abone_tipi,abone_tip);
                printf("%-7s %-10s %-12s %-25s %-10s\n","Tel No","Adi","Soyadi","Adresi","Abone Tipi");
                printf("------- ---------- ------------ ------------------------- ----------\n");
                printf("%-7d %-10s %-12s %-25s %-10s\n",tel_no,yeni_abone->ad,yeni_abone->soyad,yeni_abone->adres,abone_tip);
                bir_aboneye_ait_ilk_uc_gorusmenin_listesi(yeni_abone);
            }
            else printf("Bu numaraya kayitli bir abone yok!\n");
            break;

        case 7:
            printf("Telefon numaralarinin ilk üc hanesini giriniz:\n");
            scanf("%d",&tel_no);
            //Girilen ilk uc haneden 100 cikartiliyor ve hash tablosundaki o numara ile baslayan numaralarin bagli listesine erisiliyr..
            hash_tel=tel_no-100;
            ilk_uc_basamaga_gore_bilgi_listeleme(telefona_gore_hash_tablosu[hash_tel]);
            break;

        case 8:
            printf("Hangi bas harfle baslayan soyadlarin listesini yazdirmak istiyorsaniz o harfi giriniz:\n");
            fflush(stdin);
            harf=getchar();
            //Bas harfine gore isim icin hash uretiliyor..
            hash_isim=soyadina_gore_hash(harf);
            soyadin_bas_harfine_gore_ekrana_yazdirma(&ada_gore_hash_tablosu[hash_isim]);
            break;

        case 9:
            printf("Hangi kelime grubu ile baslyan soyadlarin listesini yazdirmak istiyorsaniz o kismini giriniz:\n");
            fflush(stdin);
            gets(soyad_parca);
            //Alinan soyad parcasinin bas harfinden hash uretiliyor ve soyad parcasi ile o indisteki bas dugumu fonksiyona gonderiliyor..
            hash_isim=soyadina_gore_hash(soyad_parca[0]);
            soyadin_bir_parcasina_gore_ekrana_yazdirma(&ada_gore_hash_tablosu[hash_isim],soyad_parca);
            break;

        case 10:
            tum_abonelerin_listesi(telefona_gore_hash_tablosu);
            break;

        case 11:
            printf("Tarihi giriniz:\n");
            fflush(stdin);
            gets(tarih);
            bir_tarihten_beri_gorusme_yapmayanlar(telefona_gore_hash_tablosu,tarih);
            break;

        case 12:
            printf("Telefon numarasi giriniz:\n");
            scanf("%d",&tel_no);
            bir_numarayi_arayan_kullanicilarin_listesi(ada_gore_hash_tablosu, tel_no);
            break;

        case 13:
            printf("Tarihi giriniz:\n");
            fflush(stdin);
            gets(tarih);
            bir_tarihteki_gorusmeler(telefona_gore_hash_tablosu,tarih);
            break;

        case 14:
            cikis=menuden_cikis();
            break;
        }
    }
    while(cikis=='H'||cikis=='h');
    return 0;
}


void menu()
{
    printf("    MENU\n");
    printf("1)  Yeni bir abonenin eklenmesi.\n");
    printf("2)  Bir abonenin nakil yapilmasi.\n");
    printf("3)  Yapilan gorusmelere iliskin bilgilerin eklenmesi.\n");
    printf("4)  Bir abonenin silinmesi.\n");
    printf("5)  Bir abonenin bilgilerinin ve yapmis oldugu tum gorusmelerin listelenmesi.\n");
    printf("6)  Bir abonenin bilgilerinin ve yapmis oldugu son 3 gorusmenin listelenmesi.\n");
    printf("7)  Telefon numarasinin ilk 3 basamagina gore abone arama.\n");
    printf("8)  Soyadin ilk harfine gore abone arama.\n");
    printf("9)  Soyada gore abone arama.\n");
    printf("10) Tum abonelerin bilgilerinin listelenmesi.\n");
    printf("11) Belirli bir tarihten beridir hic gorusme yapmamis abonelerin bulunmasi.\n");
    printf("12) Belirli bir numarayi arayan abonelerin bulunmasi.\n");
    printf("13) Belirli bir tarihte yapilan tum gorusmelerin listelenmesi.\n");
    printf("14) Cikis\n");
}


int secim_menu()
{
    int giris;
    menu();
    printf("Seciminizi giriniz:\n");
    scanf("%d",&giris);
    system("cls");

    if(giris > 0 && giris <= 13)
        return giris;
    else
    {
        return secim_menu();
        system("cls");
    }
}


char menuden_cikis()
{
    char cikis;
    do
    {
        printf("\nCikmak istediginize emin misiniz(e/E/h/H)?:");
        fflush(stdin);
        cikis=getchar();
        system("cls");
    }
    while (cikis != 'e' && cikis != 'E' && cikis!='h' && cikis!='H');

    return cikis;
}


int tele_gore_hash(int tel_no)
{
    return (tel_no/10000)-100;
}


abone *tele_gore_arama(abone * ilkPtr,int aranan_tel_no)
{
    abone *gecici;

    gecici=ilkPtr;
    while (gecici!=NULL && gecici->tel_no<=aranan_tel_no)
    {
        if (gecici->tel_no==aranan_tel_no)//aranan telefon numarasi bulundu ise abonenin adresi donduruluyor..
            return gecici;
        gecici=gecici->tele_gore_sonraki;
    }
    return NULL;//yok ise null donduruluyor..
}


abone *ada_gore_arama(abone ilk_dugum,char aranan_soyad[13])
{
    abone *suanki;

    suanki=ilk_dugum.isme_gore_sonraki;
    while(suanki!=&ilk_dugum)
    {
        if(strcmp(aranan_soyad,suanki->soyad)==0)
            return suanki;//Aranan soyisimli kisi bulunduysa adresi donduruluyor..
        else if(strcmp(aranan_soyad,suanki->soyad)>0)
            return NULL;;//O listede aradigimiz isimden kucukse artik listedeki abonelerin soyisimleri arama sonlandirilir..
        suanki=suanki->isme_gore_sonraki;
    }
    return NULL;
}


int soyadina_gore_hash(char bas_harf)
{
    bas_harf=tolower(bas_harf);

    return bas_harf-'a';
}


void tele_gore_sirali_ekleme(abone **ilkPtr,abone *yeni_abone)
{
    abone *suanki,*onceki;

    onceki=NULL;
    suanki=*ilkPtr;
    while(suanki!=NULL&&yeni_abone->tel_no>suanki->tel_no)
    {
        onceki=suanki;
        suanki=suanki->tele_gore_sonraki;
    }
    if(onceki==NULL)
    {
        yeni_abone->tele_gore_sonraki= *ilkPtr;
        *ilkPtr=yeni_abone;
    }
    else
    {
        onceki->tele_gore_sonraki=yeni_abone;
        yeni_abone->tele_gore_sonraki=suanki;
    }
}


void soyadina_gore_sirali_ekleme(abone *ilkPtr,abone *yeni_abone)
{
    abone *suanki,*onceki;

    onceki=ilkPtr;
    suanki=ilkPtr->isme_gore_sonraki;
    while(suanki!=ilkPtr&&strncmp(strlwr(yeni_abone->soyad),strlwr(suanki->soyad),strlen(yeni_abone->soyad))>0)
    {
        onceki=suanki;
        suanki=suanki->isme_gore_sonraki;
    }
    yeni_abone->isme_gore_sonraki=suanki;
    onceki->isme_gore_sonraki=yeni_abone;
}


void tele_gore_silme(abone **ilkPtr, abone *cikartilacak)
{
    abone *onceki,*suanki;

    onceki=*ilkPtr;
    if(onceki==cikartilacak)
    {
        *ilkPtr=(*ilkPtr)->tele_gore_sonraki;
        free(onceki);
    }
    else
    {
        suanki=(*ilkPtr)->tele_gore_sonraki;
        while(suanki!=cikartilacak)
        {
            onceki=suanki;
            suanki=suanki->tele_gore_sonraki;
        }
        onceki->tele_gore_sonraki=suanki->tele_gore_sonraki;
        free(suanki);
    }
}


void nakil_islemleri(abone *telefona_gore_hash_tablosu[900],int tel_no)
{
    abone * nakil_abone;
    char cevap;
    abone *gecici;
    int hash_tel;

    hash_tel=tele_gore_hash(tel_no);
    nakil_abone=tele_gore_arama(telefona_gore_hash_tablosu[hash_tel],tel_no);
    if(nakil_abone!=NULL) //girilen numarali abone olup olmadigi arama sonrasi kontrol ediliyor..
    {
        printf("Nakil edilecek abonenin yeni adresini giriniz:\n");
        fflush(stdin);
        gets(nakil_abone->adres);

        printf("Aboneye yeni telefon numarasi almak ister misiniz?[Evet:(E/e),Hayir:(H/h)]:\n");
        fflush(stdin);
        cevap=getchar();
        if(cevap=='e'||cevap=='E') //Abone numarasini degistirmek istiyorsa if e giriliyor..
        {
            gecici=malloc(sizeof(abone));
            *gecici=*nakil_abone;//Aktarilacak abonenin diger kayitleri aktarilip free yapiliyor ve siliniyor..
            tele_gore_silme(&telefona_gore_hash_tablosu[hash_tel],nakil_abone);
            free(nakil_abone);
            do //baska bi aboneye kayitli olmayan bi numara secmesi saglaniyor..
            {
                printf("Abonenin nakil olmak istedigi numarayi giriniz:\n");
                scanf("%d",&tel_no);
            }
            while(tele_gore_arama(telefona_gore_hash_tablosu[hash_tel],tel_no)!=NULL);
            //abone yeni yerine ekleniyor..
            gecici->tel_no=tel_no;
            hash_tel=tele_gore_hash(tel_no);
            tele_gore_sirali_ekleme(&telefona_gore_hash_tablosu[hash_tel],gecici);
        }
        printf("Nakil islemi tamamlanmistir.\n");
    }
    else printf("Girilen numaraya kayitli bir abone yok!\n");
}


void gorusme_ekle(abone *abonePtr)
{
    gorusme *gecici;
    gorusme *yeni_gorusme;
    int gorusme_yili,gorusme_ayi,gorusme_gunu,gecici_tarih_int,tarih_int;
    int gecici_gorusme_yili,gecici_gorusme_ayi,gecici_gorusme_gunu;

    yeni_gorusme=malloc(sizeof(gorusme));
    printf("Gorusme tarihini giriniz:\n");
    fflush(stdin);
    gets(yeni_gorusme->tarih);
    printf("Aranan numarayi giriniz:\n");
    fflush(stdin);
    gets(yeni_gorusme->aranan_no);
    printf("Baslangic zamanini giriniz:\n");
    fflush(stdin);
    gets(yeni_gorusme->bas_zamani);
    printf("Gorusme suresini giriniz:\n");
    scanf("%d",&yeni_gorusme->konusma_suresi);

    gorusmenin_tarihinin_cozumlemesi(&gorusme_yili,&gorusme_ayi,&gorusme_gunu,yeni_gorusme->tarih);
    tarih_int=10000*gorusme_yili+100*gorusme_ayi+gorusme_gunu;//Yil ay gun seklinde bir int sayi uretiliyor eklenecek gorusme icin
    gecici=abonePtr->gorusmePtr;//Geciciye abonenin gorusmelerinin baþlangiç dugumunun adresi gonderiliyor
    if(abonePtr->gorusmePtr==gecici->sonraki) //Listeye ilk gorusme ekleniyor..
    {
        gecici->onceki=yeni_gorusme;
        gecici->sonraki=yeni_gorusme;
        yeni_gorusme->sonraki=gecici;
        yeni_gorusme->onceki=gecici;
    }
    else //Listede baska gorusmeler varsa tarihe gore sirali bir sekilde ekleme yapiliyor...
    {
        gecici=gecici->sonraki;
        gorusmenin_tarihinin_cozumlemesi(&gecici_gorusme_yili,&gecici_gorusme_ayi,&gecici_gorusme_gunu,gecici->tarih);
        gecici_tarih_int=10000*gecici_gorusme_yili+100*gecici_gorusme_ayi+gecici_gorusme_gunu;
        while(gecici!=abonePtr->gorusmePtr&&gecici_tarih_int<tarih_int) //Eklenecek uygun yer bulunuyor..
        {
            gecici=gecici->sonraki;
            gorusmenin_tarihinin_cozumlemesi(&gecici_gorusme_yili,&gecici_gorusme_ayi,&gecici_gorusme_gunu,gecici->tarih);
            gecici_tarih_int=10000*gecici_gorusme_yili+100*gecici_gorusme_ayi+gecici_gorusme_gunu;
        }

        yeni_gorusme->sonraki=gecici;
        yeni_gorusme->onceki=gecici->onceki;
        gecici->onceki->sonraki=yeni_gorusme;
        gecici->onceki=yeni_gorusme;
    }
}


void gorusmenin_tarihinin_cozumlemesi(int *yil,int *ay,int *gun,char tarih[9])
{
    //Verilen tarihin yil,ay,gun bilesenleri ayriliyor..
    (*yil)=1000*(tarih[4]-48)+100*(tarih[5]-48)+10*(tarih[6]-48)+(tarih[7]-48);
    (*ay)=10*(tarih[2]-48)+(tarih[3]-48);
    (*gun)=10*(tarih[0]-48)+(tarih[1]-48);
}
void telefon_numarasina_gore_gorusme_sil(abone *abonePtr)
{
    //Silinecek abonenin gorusmeleride sirasi ile free yapiliyor..
    gorusme *suanki,*onceki;

    onceki=abonePtr->gorusmePtr;
    if(onceki==onceki->sonraki)//Gorusme yoksa baslangic dugumu free yapiliyor..
        free(onceki);
    else
    {
        suanki=onceki->sonraki;
        while(suanki!=abonePtr->gorusmePtr) //listenini hepsi baslangic dugumu haric free yapiliyor..
        {
            free(onceki);
            onceki=suanki;
            suanki=suanki->sonraki;
        }
        free(onceki);//baslangic dugumu free yapiliyor..
    }
}



void ada_gore_abone_silme(abone *ada_gore_bas_dugumu,int tel_no)
{
    abone *suanki,*onceki;
    onceki=ada_gore_bas_dugumu;
    suanki=ada_gore_bas_dugumu->isme_gore_sonraki;
    //listede birtek aradigimiz abone varsa silme islemi gerceklesiyor..
    if(suanki->isme_gore_sonraki==ada_gore_bas_dugumu)
    {
        ada_gore_bas_dugumu->isme_gore_sonraki=ada_gore_bas_dugumu;
        free(suanki);
    }
    else //Abone bulunup siliiyor...
    {
        onceki=suanki;
        suanki=suanki->isme_gore_sonraki;
        while(suanki!=ada_gore_bas_dugumu)
        {
            if(tel_no==suanki->tel_no) //silinicek abone telefon numarasina gore bulunduktan sonra free yapiliyor..
            {
                onceki->isme_gore_sonraki=suanki->isme_gore_sonraki;
                free(suanki);
                break;
            }
            onceki=suanki;
            suanki=suanki->isme_gore_sonraki;
        }
    }
}



void bir_aboneye_ait_gorusmelerin_listesi(abone *abonePtr)
{
    int gorusme_yili,gorusme_ayi,gorusme_gunu,top_gorusme_sayisi=0,top_gorusme_suresi=0,kontrol=0;
    gorusme *gecici;

    gecici=abonePtr->gorusmePtr;//o abonenin gorusmelerinin baslangic dugumunun adresi gonderiliyor..
    gecici=gecici->sonraki;
    gorusmenin_tarihinin_cozumlemesi(&gorusme_yili,&gorusme_ayi,&gorusme_gunu,gecici->tarih);
    while(gecici!=abonePtr->gorusmePtr)
    {
        if(kontrol==0) //Bir kere girdikten sonra bir daha bu ekrana yazdirilmiyor..
        {
            printf("%-10s %-13s %-11s %-4s\n","Tarih","Aranan Tel No","Bas. Zamani","Sure");
            printf("---------- ------------- ----------- ----\n");
            kontrol=1;
        }
        printf("%.2d.%.2d.%.2d %-13s %c%c:%c%c       %d\n",gorusme_gunu,gorusme_ayi,gorusme_yili,gecici->aranan_no
               ,gecici->bas_zamani[0],gecici->bas_zamani[1],gecici->bas_zamani[2],gecici->bas_zamani[3],gecici->konusma_suresi);
        top_gorusme_sayisi++;
        top_gorusme_suresi+=gecici->konusma_suresi;
        gecici=gecici->sonraki;
        gorusmenin_tarihinin_cozumlemesi(&gorusme_yili,&gorusme_ayi,&gorusme_gunu,gecici->tarih);
    }
    if(kontrol==1)
    {
        printf("------------------------------------------\n");
        printf("Toplam gorusme sayisi: %d\n",top_gorusme_sayisi);
        printf("Toplam gorusme suresi: %d\n",top_gorusme_suresi);
        printf("Ortalama gorusme suresi: %.2f\n",(float)top_gorusme_suresi/top_gorusme_sayisi);
    }
    else printf("Bu kullaniciya ait gorusme yoktur.\n");
}



void bir_aboneye_ait_ilk_uc_gorusmenin_listesi(abone *abonePtr)
{
    int gorusme_yili,gorusme_ayi,gorusme_gunu,top_gorusme_sayisi=0,top_gorusme_suresi=0,kontrol=0,i;
    gorusme *gecici;

    gecici=abonePtr->gorusmePtr;//o abonenin gorusmelerinin baslangic dugumunun adresi gonderiliyor..
    gecici=gecici->onceki;
    gorusmenin_tarihinin_cozumlemesi(&gorusme_yili,&gorusme_ayi,&gorusme_gunu,gecici->tarih);
    while(gecici!=abonePtr->gorusmePtr)
    {
        if(kontrol==0) //Bir kere girdikten sonra bir daha bu ekrana yazdirilmiyor..
        {
            printf("%-10s %-13s %-11s %-4s\n","Tarih","Aranan Tel No","Bas. Zamani","Sure");
            printf("---------- ------------- ----------- ----\n");
            kontrol=1;
        }
        for(i=0; i<3; i++)
        {
            printf("%.2d.%.2d.%.2d %-13s %c%c:%c%c       %d\n",gorusme_gunu,gorusme_ayi,gorusme_yili,gecici->aranan_no
                   ,gecici->bas_zamani[0],gecici->bas_zamani[1],gecici->bas_zamani[2],gecici->bas_zamani[3],gecici->konusma_suresi);

            gecici=gecici->onceki;
            gorusmenin_tarihinin_cozumlemesi(&gorusme_yili,&gorusme_ayi,&gorusme_gunu,gecici->tarih);
        }
    }
    if(kontrol==0)
    {
        printf("Bu kullaniciya ait gorusme yoktur.\n");
    }
}



void ilk_uc_basamaga_gore_bilgi_listeleme(abone * ilkPtr)
{
    abone *suanki;
    int kontrol=0;
    char abone_tip[9];

    suanki=ilkPtr;
    while (suanki!=NULL)
    {
        if(kontrol==0)
        {
            printf("%-7s %-10s %-12s %-25s %-10s\n","Tel No","Adi","Soyadi","Adresi","Abone Tipi");
            printf("------- ---------- ------------ ------------------------- ----------\n");
            kontrol=1;
        }
        abone_tipi_donusturme(suanki->abone_tipi,abone_tip);
        printf("%-7d %-10s %-12s %-25s %-10s\n",suanki->tel_no,suanki->ad,suanki->soyad,suanki->adres,abone_tip);
        suanki=suanki->tele_gore_sonraki;
    }

    if(kontrol==0) printf("Bu ilk uc haneye sahip abone yoktur.\n");
}



void soyadin_bas_harfine_gore_ekrana_yazdirma(abone *ilk_dugum)
{
    abone *suanki;
    int kontrol=0;
    char abone_tip[9];

    suanki=ilk_dugum->isme_gore_sonraki;
    while(suanki!=ilk_dugum)
    {
        if(kontrol==0)
        {
            printf("%-7s %-10s %-12s %-25s %-10s\n","Tel No","Adi","Soyadi","Adresi","Abone Tipi");
            printf("------- ---------- ------------ ------------------------- ----------\n");
            kontrol=1;
        }
        abone_tipi_donusturme(suanki->abone_tipi,abone_tip);
        printf("%-7d %-10s %-12s %-25s %-10s\n",suanki->tel_no,suanki->ad,suanki->soyad,suanki->adres,abone_tip);
        suanki=suanki->isme_gore_sonraki;
    }

    if(kontrol==0) printf("Bu harfle baslayan soyadli abone yoktur.\n");
}



void soyadin_bir_parcasina_gore_ekrana_yazdirma(abone *ilk_dugum,char aranan[13])
{
    abone *suanki;
    char abone_tip [9];
    int kontrol=0;

    suanki=ilk_dugum->isme_gore_sonraki;
    while(suanki!=ilk_dugum)
    {
        if(kontrol==0&&(strncmp(aranan,suanki->soyad,strlen(aranan))==0))
        {
            printf("%-7s %-10s %-12s %-25s %-10s\n","Tel No","Adi","Soyadi","Adresi","Abone Tipi");
            printf("------- ---------- ------------ ------------------------- ----------\n");
            kontrol=1;
        }
        if(strncmp(aranan,suanki->soyad,strlen(aranan))==0) //Aranan parcayi icinde barindiran soyisimli aboneler ekrana yazdiriliyor..
        {
            abone_tipi_donusturme(suanki->abone_tipi,abone_tip);
            printf("%-7d %-10s %-12s %-25s %-10s\n",suanki->tel_no,suanki->ad,suanki->soyad,suanki->adres,abone_tip);
        }
        suanki=suanki->isme_gore_sonraki;
    }

    if(kontrol==0) printf("Bu kelime parcasi ile baslayan soyadli abone yoktur.\n");
}



void abone_tipi_donusturme(char harf,char kelime [9])
{
    if(harf==83)
    {
        kelime[0]='S';
        kelime[1]='a';
        kelime[2]='h';
        kelime[3]='i';
        kelime[4]='s';
        kelime[5]='\0';
    }
    else if(harf==73)
    {
        kelime[0]='I';
        kelime[1]='s';
        kelime[2]='y';
        kelime[3]='e';
        kelime[4]='r';
        kelime[5]='i';
        kelime[6]='\0';

    }
    else if(harf==75)
    {
        kelime[0]='K';
        kelime[1]='a';
        kelime[2]='m';
        kelime[3]='u';
        kelime[4]='\0';
    }
    else
    {
        kelime[0]='G';
        kelime[1]='u';
        kelime[2]='v';
        kelime[3]='e';
        kelime[4]='n';
        kelime[5]='l';
        kelime[6]='i';
        kelime[7]='k';
        kelime[8]='\0';
    }
}



void tum_abonelerin_listesi(abone *telefona_gore_hash_tablosu[900])
{
    abone *gecici;
    char abone_tip[9],kontrol=0,kontrol_2=0;
    int abone_sayisi[5],top_gorusme_sayisi[5],top_gorusme_suresi[5],i;

    for(i=0; i<5; i++) //Diziler her cagrilista sifirlaniyor...
    {
        abone_sayisi[i]=0;
        top_gorusme_sayisi[i]=0;
        top_gorusme_suresi[i]=0;
    }
    for(i=0; i<900; i++)
    {
        gecici=telefona_gore_hash_tablosu[i];
        if(telefona_gore_hash_tablosu[i]!=NULL)
        {
            while (gecici!=NULL)
            {
                abone_sayisi[4]++;
                if(gecici->abone_tipi=='K')
                    abone_sayisi[0]++;
                else if(gecici->abone_tipi=='G')
                    abone_sayisi[1]++;
                else if(gecici->abone_tipi=='I')
                    abone_sayisi[2]++;
                else
                    abone_sayisi[3]++;
                abone_tipi_donusturme(gecici->abone_tipi,abone_tip);
                if(kontrol_2==0)
                {
                    printf("%-7s %-10s %-12s %-25s %-10s\n","Tel No","Adi","Soyadi","Adresi","Abone Tipi");
                    printf("------- ---------- ------------ ------------------------- ----------\n");
                    kontrol_2=1;
                }
                printf("%-7d %-10s %-12s %-25s %-10s\n",gecici->tel_no,gecici->ad,gecici->soyad,gecici->adres,abone_tip);
                abonelerin_gorusme_analizi(gecici,top_gorusme_sayisi,top_gorusme_suresi);
                gecici=gecici->tele_gore_sonraki;
            }
            kontrol=1;
        }
    }
    if(kontrol==1)
    {
        printf("%-10s %-12s %-14s %-11s %-13s\n","Abone Tipi","Abone Sayisi","Gorusme Sayisi","Toplam Sure","Ortalama Sure");
        printf("---------- ------------ -------------- ----------- -------------\n");
        printf("%-10s %-12d %-14d %-11d %-13.2f\n","Sahis",abone_sayisi[3],top_gorusme_sayisi[3],top_gorusme_suresi[3],(float)top_gorusme_suresi[3]/top_gorusme_sayisi[3]);
        printf("%-10s %-12d %-14d %-11d %-13.2f\n","Isyeri",abone_sayisi[2],top_gorusme_sayisi[2],top_gorusme_suresi[2],(float)top_gorusme_suresi[2]/top_gorusme_sayisi[2]);
        printf("%-10s %-12d %-14d %-11d %-13.2f\n","Kamu",abone_sayisi[0],top_gorusme_sayisi[0],top_gorusme_suresi[0],(float)top_gorusme_suresi[0]/top_gorusme_sayisi[0]);
        printf("%-10s %-12d %-14d %-11d %-13.2f\n","Guvenlik",abone_sayisi[1],top_gorusme_sayisi[1],top_gorusme_suresi[1],(float)top_gorusme_suresi[1]/top_gorusme_sayisi[1]);
        printf("----------------------------------------------------------------\n");
        printf("%-10s %-12d %-14d %-11d %-13.2f\n","Toplam",abone_sayisi[4],top_gorusme_sayisi[4],top_gorusme_suresi[4],(float)top_gorusme_suresi[4]/top_gorusme_sayisi[4]);
    }
    else printf("Kayitli abone yok.\n");
}



void abonelerin_gorusme_analizi(abone *abonePtr,int top_gorusme_sayisi[5],int top_gorusme_suresi[5])
{
    gorusme *gecici;
    gecici=abonePtr->gorusmePtr;
    gecici=gecici->onceki;

    while(gecici!=abonePtr->gorusmePtr)
    {
        if(abonePtr->abone_tipi=='K')
        {
            top_gorusme_sayisi[0]++;
            top_gorusme_suresi[0]+=gecici->konusma_suresi;
        }
        else if(abonePtr->abone_tipi=='G')
        {
            top_gorusme_sayisi[1]++;
            top_gorusme_suresi[1]+=gecici->konusma_suresi;
        }
        else if(abonePtr->abone_tipi=='I')
        {
            top_gorusme_sayisi[2]++;
            top_gorusme_suresi[2]+=gecici->konusma_suresi;
        }
        else
        {
            top_gorusme_sayisi[3]++;
            top_gorusme_suresi[3]+=gecici->konusma_suresi;
        }
        top_gorusme_sayisi[4]++;
        top_gorusme_suresi[4]+=gecici->konusma_suresi;
        gecici=gecici->onceki;
    }

}



void bir_tarihten_beri_gorusme_yapmayanlar(abone *telefona_gore_hash_tablosu[900],char tarih[9])
{
    int gorusme_yili,gorusme_ayi,gorusme_gunu,kontrol_2=0,kontrol_3=0,i,tarih_int,gecici_tarih_int;
    int gecici_gorusme_yili,gecici_gorusme_ayi,gecici_gorusme_gunu,kontrol=0;
    char abone_tip[9];
    abone *gecici;
    gorusme *gecici_gorusme;

    for(i=0; i<900; i++)
    {
        if(telefona_gore_hash_tablosu[i]!=NULL) //Bu listede abone varsa islemler yapiliyor..
        {
            gecici=telefona_gore_hash_tablosu[i];
            gorusmenin_tarihinin_cozumlemesi(&gorusme_yili,&gorusme_ayi,&gorusme_gunu,tarih);
            tarih_int=10000*gorusme_yili+100*gorusme_ayi+gorusme_gunu;
            while (gecici!=NULL)
            {
                gecici_gorusme=gecici->gorusmePtr;
                gecici_gorusme=gecici_gorusme->onceki;
                while(gecici_gorusme!=gecici->gorusmePtr)
                {
                    gorusmenin_tarihinin_cozumlemesi(&gecici_gorusme_yili,&gecici_gorusme_ayi,&gecici_gorusme_gunu,gecici_gorusme->tarih);
                    gecici_tarih_int=10000*gecici_gorusme_yili+100*gecici_gorusme_ayi+gecici_gorusme_gunu;
                    if(gecici_gorusme_yili>=gorusme_yili)
                    {
                        kontrol=1;
                        break;
                    }
                    gecici_gorusme=gecici_gorusme->onceki;
                }
                if(kontrol==0)
                {
                    if(kontrol_2==0)
                    {
                        printf("%-7s %-10s %-12s %-25s %-10s\n","Tel No","Adi","Soyadi","Adresi","Abone Tipi");
                        printf("------- ---------- ------------ ------------------------- ----------\n");
                        kontrol_2=1;
                    }
                    abone_tipi_donusturme(gecici->abone_tipi,abone_tip);
                    printf("%-7d %-10s %-12s %-25s %-10s\n",gecici->tel_no,gecici->ad,gecici->soyad,gecici->adres,abone_tip);
                }
                gecici=gecici->tele_gore_sonraki;
                kontrol=0;
            }
            kontrol_3=1;
        }
    }

    if(kontrol_3==0) printf("Kayitli abone yok.\n");
    else if(kontrol_2==0) printf("Bu tarihten beri gorusme yapmayan abone yok.\n");
}



void bir_numarayi_arayan_kullanicilarin_listesi(abone ada_gore_hash_tablosu[26],int tel_no)
{
    char abone_tip[9];
    abone *gecici;
    gorusme *gecici_gorusme;
    int kontrol=0,i,kontrol_2=0,kontrol_3=0;

    for(i=0; i<26; i++)
    {
        gecici=(&ada_gore_hash_tablosu[i])->isme_gore_sonraki;
        while (gecici!=&ada_gore_hash_tablosu[i])
        {
            gecici_gorusme=gecici->gorusmePtr;
            gecici_gorusme=gecici_gorusme->sonraki;
            while(gecici_gorusme!=gecici->gorusmePtr)
            {
                if(aranan_tel_no_donusum(gecici_gorusme->aranan_no)==tel_no)
                {
                    kontrol=1;
                    break;
                }
                gecici_gorusme=gecici_gorusme->sonraki;
            }
            if(kontrol==1)
            {
                if(kontrol_2==0&&ada_gore_hash_tablosu[i].isme_gore_sonraki!=&ada_gore_hash_tablosu[i])
                {
                    printf("%-7s %-10s %-12s %-25s %-10s\n","Tel No","Adi","Soyadi","Adresi","Abone Tipi");
                    printf("------- ---------- ------------ ------------------------- ----------\n");
                    kontrol_2=1;
                }
                abone_tipi_donusturme(gecici->abone_tipi,abone_tip);
                printf("%-7d %-10s %-12s %-25s %-10s\n",gecici->tel_no,gecici->ad,gecici->soyad,gecici->adres,abone_tip);

            }
            gecici=gecici->isme_gore_sonraki;
            kontrol=0;
        }
        kontrol_3=1;
    }

    if(kontrol_3==0) printf("Kayitli abone yok.\n");
    else if(kontrol_2==0) printf("Bu numarayi arayan abone bulunamadi.\n");
}



int aranan_tel_no_donusum(char aranan[11])
{
    int aranan_tel_no_int=0,i;
    for(i=3; i<10; i++)
        aranan_tel_no_int+=(aranan[i]-48)*pow(10,9-i);

    return aranan_tel_no_int;
}


void bir_tarihteki_gorusmeler(abone *telefona_gore_hash_tablosu[900],char tarih[9])
{
    int gorusme_yili,gorusme_ayi,gorusme_gunu,i,kontrol=0,kontrol_2=0;
    int gecici_gorusme_yili,gecici_gorusme_ayi,gecici_gorusme_gunu;
    abone *gecici;
    gorusme *gecici_gorusme;
    for(i=0; i<900; i++)
    {
        gecici=telefona_gore_hash_tablosu[i];
        if(gecici!=NULL)
        {
            while (gecici!=NULL)
            {
                gecici_gorusme=gecici->gorusmePtr;
                gecici_gorusme=gecici_gorusme->onceki;
                gorusmenin_tarihinin_cozumlemesi(&gorusme_yili,&gorusme_ayi,&gorusme_gunu,tarih);
                while(gecici_gorusme!=gecici->gorusmePtr)
                {
                    gorusmenin_tarihinin_cozumlemesi(&gecici_gorusme_yili,&gecici_gorusme_ayi,&gecici_gorusme_gunu,gecici_gorusme->tarih);
                    if(gecici_gorusme_yili==gorusme_yili&&gecici_gorusme_ayi==gorusme_ayi&&gecici_gorusme_gunu==gorusme_gunu)
                    {
                        if(kontrol==0)
                        {
                            printf("%-13s %-10s %-13s %-11s %-4s\n","Arayan Tel No","Tarih","Aranan Tel No","Bas. Zamani","Sure");
                            printf("------------- ---------- ------------- ----------- ----\n");
                            kontrol=1;
                        }
                        printf("%-13d %.2d.%.2d.%.2d %-13s %c%c:%c%c       %d\n",gecici->tel_no,gorusme_gunu,gorusme_ayi,gorusme_yili,gecici_gorusme->aranan_no
                               ,gecici_gorusme->bas_zamani[0],gecici_gorusme->bas_zamani[1],gecici_gorusme->bas_zamani[2],gecici_gorusme->bas_zamani[3],gecici_gorusme->konusma_suresi);
                    }
                    gecici_gorusme=gecici_gorusme->onceki;
                }
                gecici=gecici->tele_gore_sonraki;
            }
            kontrol_2=1;
        }
    }

    if(kontrol_2==0) printf("Kayitli abone yok.\n");
    else if(kontrol==0) printf("Bu tarihte arama yapan abone bulunamadi.\n");
}
