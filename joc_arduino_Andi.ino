#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>

#define DIGIT4 4
#define DIGIT5 5
#define DIGIT6 6
#define DIGIT7 7
#define X 0                                                                                     //Pinul de pe care se citeste coordonata x de pe joystick
#define Y 1                                                                                     //Pinul de pe care se citeste coordonata y de pe joystick
#define SW 3                                                                                    //Pinul  corespunzator butonului joystick-ului
#define BUTTON1 13
#define BUTTON2 8
LedControl lc = LedControl (12,11,10,1);
LiquidCrystal LCD (1, 2, DIGIT4, DIGIT5, DIGIT6, DIGIT7);

int px = 6, py = 5, yj = 510;                                                                   //px, py - coordonatele initiale ale cursorului pe matrice
int matrix[8][12];                                                                              //matrix - matricea corespunzatoare pozitiilor de pe ecran 

long time;                                                                                      //time - numarul de secunde de la inceperea jocului                                              
int y1 = 63, y2 = 63;                                                                           //y1, y2 - variabile corespunzatoare reprezentarii zidurilor
int loops, OK = 0;                                                                              //loops - numarul de loop-uri efectuate pana la un moment dat
int lives;                                                                                      //lives - numarul de vieti
int address, vehicleType, bonusx = 8, bonusy;
long bonusTime = - 10000;                                                                       //address - adresa in memoria ROM; vehicleType - tipul vehiculului folosit
bool recordBonus;                                                                               //bonusx, bonusy - coordonatele punctului bonus pe matrice

class vehicle                                                                        
{
  protected:
     int ** a;                                                                                  //matricea a corespunde reprezentarii unui obiect vehicul pe matrice
     int cen1, cen2;                                                                            //cen1, cen2 - coordonatele centrului obiectului de tip vehicul
  public:
     vehicle();
     int shape (int, int);                                                            
     void erase (int, int);
     virtual bool bonus () = 0;
     int * operator[] (int);                                                         
     vehicle & operator = (vehicle &);
     int & row();
     int & col();
     virtual ~vehicle();
     
};

vehicle :: vehicle()                                                                             //constructorul clasei vehicul aloca memorie pentru matricea a,
{                                                                                                //elementele matricei vor primi valori prin apelul constructorilor claselor derivate
   a = new int * [3];
    for (int i = 0; i < 3; i ++)
          a[i] = new int[3];
}

int vehicle :: shape (int x, int y)                                                              //shape() contureaza un obiect vehicul pe matrice in functie de 
{                                                                                                //un centru dat. Metoda aprinde led-urile de pe pozitiile in care exista o componenta  
                                                                                                 //a vehiculului (pozitiile codificate cu 1 in matricea clasei). Daca un led pe care ar trebui
    for (int j = y - 1; j < y + 2; j ++)                                                         //conturat obiectul este deja aprins, vehiculul nu poate fi reprezentat, exceptand
         for (int i = x - 1; i < x + 2; i ++)                                                    //led-ul corespunzator unui element bonus, codificat cu cifra 2 pe tabloul matrix
             if (a[i - x + 1][j - y + 1] == 1)                                  
             {
                 if (matrix[j][i + 2] == 1)
                 {
                     if(recordBonus == 0)
                        return 0;
                 }
                          
                 if (matrix[j][i + 2] == 2)                                                    //jucatorul primeste bonus
                 {
                      recordBonus = bonus();
                      bonusx = 8;
                 }
                                  
                              
                 lc.setLed (0, j, i, true); 
                 matrix[j][i + 2] = 1;
                 if(recordBonus == 1)
                   if((time - bonusTime) > 5000)
                   {
                         recordBonus = 0;
                   }
                                             
              }
                
     cen1 = x;
     cen2 = y;
     return 1;  
}

void vehicle :: erase (int x, int y)                                                              //erase() stinge led-urile in care se afla componentele vehiculului pe ecran
{                                                                                                 //si notifica modificarile in tabloul matrix
      
   for (int j = y - 1; j < y + 2; j ++)
        for (int i = x - 1; i < x + 2; i ++)
            if (a[i - x + 1][j - y + 1] == 1 && matrix[j][i + 2] == 1)
            {
                lc.setLed (0, j, i, false);
                matrix[j][i + 2] = 0;
            }
    
}

int * vehicle :: operator [] (int i)
{
    return a[i];  
}


int & vehicle :: row ( )                                                                          //functia row() returneaza coordonata x a centrului unui obiect vehicul
{  
    return cen1;
}

int & vehicle :: col ( )                                                                          //functia col() returneaza coordonata y a centrului unui obiect vehicul
{
    return cen2;
}

vehicle & vehicle :: operator = (vehicle & t)
{
    for (int i = 0; i < 3; i ++)
        for (int j = 0; j < 3; j ++)
            a[i][j] = t[i][j];
    cen1 = t.row();
    cen2 = t.col();
    return * this; 
}

vehicle :: ~ vehicle()
{
    for (int i = 0; i < 3; i ++)
         delete a[i];
    delete [] a;
}

class car : public vehicle                                                               
{
   public:
      car();
      bool bonus ();
      car & operator = (car &);
};

car :: car()
{
    a[0][0] = 0;
    a[0][1] = 1;
    a[0][2] = 0;       
    for (int i = 0; i < 3; i ++)  
           a[1][i] = 1;
    a[2][0] = 1;
    a[2][1] = 0;
    a[2][2] = 1;
    cen1 = - 2;                                                     
    if (random (0, 2) == 0)
        cen2 = 2;
    else cen2 = 5;
}

bool car :: bonus ()
{   
    
    if (lives < 3)                                
       lives ++;
               
    return 0;
}

car & car :: operator = (car & t)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            a[i][j] = t.a[i][j];
    cen1 = t.row();
    cen2 = t.col();
    return * this; 
}

class spaceship : public vehicle
{
       int invTime;
    public:
       spaceship();
       bool bonus ();
       spaceship & operator = (spaceship &);
};

spaceship :: spaceship()
{             
    a[0][0] = 1;
    a[0][1] = 0;
    a[0][2] = 1;
    a[1][0] = 0;
    a[1][1] = 1;
    a[1][2] = 0;
    a[2][0] = 0;
    a[2][1] = 1;
    a[2][2] = 0;
    cen1 = -2;
    if (random (0, 2) == 0)
        cen2 = 2;
       else cen2 = 5;
}

spaceship & spaceship :: operator = (spaceship & t)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            a[i][j] = t.a[i][j];
    cen1 = t.row();
    cen2 = t.col();
    return * this; 
}

bool spaceship :: bonus ()
{ 
     bonusTime = millis();
     return 1;
}

void setup() 
{
  LCD.begin (16, 2);
  for (int i = 4; i <= 7; i++)
      pinMode (i, OUTPUT);
  pinMode (BUTTON1, OUTPUT);
  pinMode (BUTTON2, OUTPUT);
  lc.shutdown (0,false);
  lc.setIntensity (0,8);
  lc.clearDisplay (0);
  pinMode (SW, INPUT);
  digitalWrite (SW, HIGH);
  analogWrite (9, 100);
}

void cleanMatrix()                                                                               //functia marcheaza stingerea tuturor becurilor de pe ecran
{
      for (int i = 0; i < 8; i++)
          for (int j = 0; j < 12; j++)
             matrix[i][j] = 0; 
}

void drawBonus()                                                                                 //functia modifica pozitia unui element bonus pe matrice
{
    if (bonusx < 7)
    {
      lc.setLed (0, bonusy, bonusx, false);
      matrix[bonusy][bonusx + 2] = 0;
      bonusx++;
      lc.setLed (0, bonusy, bonusx, true);
      matrix[bonusy][bonusx + 2] = 2;
    }
    else 
    if (bonusx == 7)
    {
        lc.setLed (0, bonusy, bonusx, false);
        matrix[bonusy][bonusx + 2] = 0;
        bonusx++; 
    }
}

 
void eraseEnemies (vehicle ** & b, int n)                                                       //functia apeleaza metoda erase() pentru adversarii prezenti
{                                                                                               //pe matrice la un moment dat
  for (int i = 0; i < n; i ++)
  {
     int p1 = b[i] -> row(), p2 = b[i] -> col();
     b[i] -> erase (p1, p2);
  }
}

int shapeEnemies (vehicle ** & b, int n)                                                       //functia apeleaza metoda shape() pentru adversarii prezenti
{                                                                                              //pe matrice la un moment dat
   for (int i = 0; i < n; i ++)
   {
      int p1 = b[i] -> row(), p2 = b[i] -> col();
      b[i] -> shape (p1, p2);                 
   }
}

void modifyEnemies (vehicle ** & b, int n)                                                     //functia determina modificarea pozitiei adversarilor prezenti
{                                                                                              //la un moment dat pe ecran in raport cu pozitia curenta a vehiculului
   for (int i = 0; i < n; i ++)                                                                //cursor
   {
      int m = b[i] -> row();
      m++;
      b[i] -> row() = m;
   }
}

void pushBackEnemy (vehicle ** & b, int & n, bool type)                                        //functia determina adaugarea unui adversar in sirul de adversari 
{                                                                                              //prezenti la un moment dat pe ecran
    if (n > 0)
    {
       vehicle ** c = new vehicle * [n];
       for (int i = 0; i < n; i ++)
           if (type)
              c[i] = new car;
         else c[i] = new spaceship;
       
      for (int i = 0; i < n; i ++)
          * (c[i]) = * (b[i]);

       for (int i = 0; i < n; i ++)
            delete b[i];
       delete [] b;
       b = new vehicle * [n + 1];
       for (int i = 0; i < n + 1; i ++)
            if (type)
                b[i] = new car;
            else b[i] = new spaceship;
       for (int i = 0; i < n; i ++)
            * (b[i]) = * (c[i]);

       b[n] -> row() = -2;
       for (int i = 0; i < n; i ++)
           delete c[i];
       delete [] c; 
    }
    else
    {
      b = new vehicle *[1];
      if (type)
         b[0] = new car;
      else b[0] = new spaceship;
      b[0] -> row() = -2;
    }
    
    n++;
}

void popFrontEnemy (vehicle ** & b, int & n, bool type)                                         //functia determina stergerea unui adversar din sirul de adversari 
{                                                                                               //prezenti la un moment dat pe ecran
    if (n > 0)
    {
        vehicle ** c = new vehicle *[n];

        for (int i = 0; i < n; i ++)
             if (type)
                 c[i] = new car;
             else c[i] = new spaceship;
        for (int i = 0; i < n; i ++)
             * (c[i]) = * (b[i]);
      
        for (int i = 0; i < n; i ++)
             delete b[i];   
        delete [] b;
        
        if (n > 1)
        {
           b = new vehicle * [n - 1];
           for (int i = 0; i < n - 1; i++)
               if (type)
                  b[i] = new car;
               else b[i] = new spaceship;
                  
            for (int i = 1; i < n; i++)
                * (b[i - 1]) = * (c[i]);
         } 
         else b = NULL;
           
        for (int i = 0; i < n; i++)
                 delete c[i]; 
        delete [] c;
        n --;  
    }
}

void wall (int x, int & y)                                                                    //functia contureaza o margine a pistei de curse
{
   lc.setRow (0, x, y);
   switch (y)
   {
      case 126 : y = 63; break;
      case 252: y = 126; break;
      default : y = (y >> 1)|128;
   }
}

void direct (int & y, int iny)                                                                //functia compara pozitia anterioara a vehiculului cursor
{                                                                                             //cu pozitia citita de pe joystick
      if (iny > 520)
      {
         if (y + 1 <= 5)
            y ++;
      }
      else if (iny < 500)
           {
               if (y - 1 >= 2)
               y --;
           }
}

void destroy (vehicle & a, int x, int y)                                                     //functia este apelata la accidentarea vehiculului cursor
{                                                                                            //si genereaza un efect vizual asupra obiectului pentru instiintea 
  for (int i = x - 1; i < x + 2; i ++)                                                       //jucatorului de pierderea unei vieti
       for (int j = y - 1; j < y + 2; j ++)
              if ( a[i - x + 1][j - y + 1])
              {
                 lc.setLed (0, j, i, false);
                 matrix[j][i + 2] = 0;
              } 

  delay (400);
  for (int i = x - 1; i < x + 2; i ++)
       for (int j = y - 1; j < y + 2; j ++)
              if (a[i - x + 1][j - y + 1])
                 {
                    lc.setLed (0, j, i, true);
                    matrix[j][i + 2] = 1;
                 }  
  delay (400);
   for (int i = x - 1; i < x + 2; i++)
       for (int j = y - 1; j < y + 2; j++)
             if (a[i - x + 1][j - y + 1])
              {
                 lc.setLed (0, j, i, false);
                 matrix[j][i + 2] = 0;
              }
  delay (400);
               
}

int aleator()
{
   return random (0, 3);
}

void reload (vehicle & a, vehicle ** & b, int & n)                                               //reload() reseteaza valorile implicite ale variabilelor globale la pierderea unei vieti
{                                                                                                //sau la sfarsitul jocului                                                                
   eraseEnemies (b, n);         
   int s = a.row(), ss = a.col();
   a.erase (s, ss);
   delete [] b;
   b = NULL;
   n = 0;
   bonusx = 8;
   bonusTime = time - 10000;
   px = 6;
   py = 5;
   y1 = y2 = 63;
   yj = 510;
}

vehicle * a, ** b = NULL;                                                                        //a este adresa vehiculului cursor, iar b este un vector de adrese ale unor obiecte vehicul,                  
int score = 0, ncars = 0;                                                                        //care reprezinta adversarii lui a in cursa
long raceTime;                                                                                    

int speed (int i)                                                                                //functia returneaza viteza cu care concureaza adversarii in functie de nivel
{
  switch (i)
  {
    case 2 : return 200;
    case 3 : return 150;
    case 4 : return 100;
    case 5 : return 90;
    case 6 : return 80;
    case 7 : return 70;
    case 8 : return 60;
    case 9 : return 50;
    default : return 250;
    
  }
}

int calcLevel (int points)                                                                       //functia decide nivelul la care ajunge jucatorul in functie de punctajul
{                                                                                                //obtinut pana la un moment dat    
     if (points >= 0 && points < 40)
        return 1;
     if (points >= 40 && points < 100)
        return 2;
     if (points >= 100 && points < 180)
        return 3;
     if (points >= 180 && points < 200)
        return 4;
     if (points >= 200 && points < 320)
        return 5;
     if (points >= 320 && points < 450)
        return 6;
     if (points >= 450 && points < 550)
        return 7;
     if (points >= 550 && points < 620)
        return 8;
     if (points >= 620)
        return 9;
}

void load (int c)                                                                                //functia load se apeleaza doar la inceputul jocului 
{                                                                                                //(cand parametrul c este 0)
    if (c == 0)
    {
      LCD.setCursor (0, 0);
      LCD.print ("Incarcare...");
      LCD.setCursor (0, 1);
      for (int i = 0; i < random (2, 5); i ++)                                                   //se afiseaza de mai multe ori o imagine care sugereaza incarcarea jocului 
      {                                                           
          for (int j = 2; j <= 5; j ++)
          {
              lc.setLed (0, j, 3, true);
              lc.setLed (0, j, 4, true);
              delay (200);
          }

          for (int j = 2; j <= 5; j ++)
          {
               lc.setLed (0, j, 3, false);
               lc.setLed (0, j, 4, false);
          }
     }
     lc.clearDisplay (0);                                                                       //se sterge ecranul si se contureaza o alta imagine prin care se sugereaza
     lc.setLed (0, 2, 5, true);                                                                 //incheierea procesului de incarcare
     delay (50);
     for (int i = 6; i >= 3; i --)
     {
         lc.setLed (0, 3 - i + 6, i, true);
         delay (50);
     }
     LCD.clear();
     LCD.setCursor (0, 0);
     LCD.print ("* Cel mai mare");                                                              //se afiseaza pe ecran cel mai mare scor, care este memorat in obiectul EEPROM
     LCD.setCursor (0, 1);                                                                      //pe patru octeti
     LCD.print ("scor : ");
     int highestScore = 0;
     address = 0;
     while (address < 4)
     {
        highestScore += EEPROM.read (address);
        address++;
     }
     LCD.print (highestScore);
     delay (5000);
     lc.clearDisplay (0);
     LCD.clear();                                                                               //se afiseaza pe ecran un obiect car si un obiect spaceship
     car aux1;                                                                                  //pentru a arata jucatorului tipurile de vehicule pe care le poate 
     spaceship aux2;                                                                            //utiliza pentru cursa
     aux1.shape (4, 6);
     aux2.shape (4, 1);
     LCD.setCursor (0, 0);
     LCD.print ("Pentru start, ");
     LCD.setCursor (0, 1);
     LCD.print ("alegeti o masina");
    }
}

void status()                                                                                
{                                                                                               //functia afiseaza pe LCD progresul in joc: scorul, nivelul, 
  LCD.clear();                                                                                  //numarul de vieti ramase
  LCD.setCursor (0, 0); 
  LCD.print ("Scor:");
  LCD.setCursor (7, 0);
  LCD.print (score);
  LCD.setCursor (13, 0);
  LCD.print ("N.");
  LCD.setCursor (15, 0);
  LCD.print (calcLevel (score));
  LCD.setCursor (0, 1);
  LCD.setCursor (0, 1);
  LCD.print ("Vieti: ");
  LCD.setCursor (8, 1);
  for (int i = 0; i < lives; i ++)
  {
     LCD.setCursor (8 + i, 1);
     LCD.print ("*");
  } 
   
}

void saveScore(int points)                                                                    //la apelul functiei saveScore, se salveaza in memoria EEPROM scorul
{                                                                                             //actual in cazul in care depaseste scorul prezent in EEPROM, suprascriindu - l 
  int max = 0;                                                                                //pe acesta.
  address = 0;
  while (address < 4)
  {
       max = max + EEPROM.read(address);
       address ++;
  }
  if (points > max)
   {
      address = 0;
      while (address < 4)
      {
         if (points > 255)
         {
            EEPROM.write (address, 255);
            points = points - 255;
         }
         else
         {
            EEPROM.write (address, points);
            points = 0;
         }
         address++;
      }  
   }
}

void race (vehicle & a, vehicle ** & b, bool vtype, int & n, int level)                            //functia primeste ca parametri vehiculul cursor,
{                                                                                                  //vectorul de adrese la adversari, tipul vehiculului
      long currentTime = millis();                                                                  //controlat, numarul de adversari si nivelul la care se afla
      if (currentTime - time > 50)                                                                 //jucatorul la un moment dat
      {                                                                                            //la fiecare 50 de milisecunde,
        a.erase (px, py);                                                                          //se reactualizeaza imaginea zidurilor in pozitia curenta
        wall (0, y1);                                                                                    
        wall (7, y2); 
        status();                                                                                  //se afiseaza progresul jucatorului
        direct (py, yj);                                                                           //Daca exista adversari pe ecran,
        if (n != 0)                                                                                //se mai adauga un obiect in vectorul de adversari doar daca
        {                                                                                        //pozitia ultimului element din vector respecta conditia din instructiunea if
            int m = b[n - 1] -> row();
            if (m == aleator() + 5)
            {
                pushBackEnemy (b, n, vtype);
                if (bonusx == 8 && score % 8 == random (0, 8))                               //se genereaza bonus in functie de o regula "pseudoaleatoare"
                  {
                      bonusx = random(-2, 0);
                      int t = b[n - 1] -> col();
                      bonusy = (t == 5)? 2 : 5;
                  }
            }
                 
            if (b[0] -> row() == 10)
            {
               score++;
               popFrontEnemy (b, n, vtype);                                                     //daca primul element din vectorul de adversari are coordonata linie
            }                                                                                  //egala cu 10, inseamna ca adversarul nu se mai afla pe ecran, adica vehiculul controlat
        }                                                                                        //l - a depasit, iar jucatorul primeste un punct
        else pushBackEnemy (b, n, vtype);
      
   
        if (currentTime - raceTime > speed (level))                                                //se modifica pozitia adversarilor pe matrice in functie de viteza stabilita pe nivel
        {       
           eraseEnemies (b, n);
           modifyEnemies (b, n);          
           shapeEnemies (b, n);
           drawBonus();
           raceTime = currentTime;
        }

                     
        if (a.shape (px, py) == 0)                                                                 //daca pe pozitia curenta a cursorului se gaseste un adversar,      
        {                                                                                    //jucatorul pierde viata si se restabilesc conditiile initiale de joc
            destroy (a, px, py); 
            reload (a, b, n);
            cleanMatrix();
            lives --;
            if (lives == 0)                                                                    //daca jucatorul nu mai are vieti, se reia jocul de la meniul de intrare
            {
                 OK = 0;
                 saveScore(score);
                 score = 0;
                 loops = 0;
                 LCD.clear();
             }
             lc.clearDisplay (0);
         }
         time = currentTime;
      }
}


void playAgain(int points)                                                                          //daca jucatorul obtine scorul maxim posibil, jocul se termina, se afiseaza pe LCD 
{                                                                                                   //mesajul "YOU WON" si se restabilesc conditiile initiale de joc
    OK = 0;
    saveScore(10);
    lives = 0;
    score = 0;
    loops = 0;
    LCD.clear();
    lc.clearDisplay(0);
    LCD.print("   YOU WON!!");
    cleanMatrix();
    bonusx = 8;
    bonusTime = - 10000;
    delay(2000);
}

int end_game()                                                                                     //functia reseteaza variabilele pentru o noua cursa
{                                                                                                  //Jocul incepe dupa apasarea unui buton
    int signal;                                                                                    //obiectele vehicul vor fi create in functie de butonul apasat.
    if (loops == 0)
    {
           
       lives = 3;
       bonusx = 8;
       load (OK);
       int s1 = digitalRead (BUTTON1);
       int s2 = digitalRead (BUTTON2);
       if (s1 == 0)
          vehicleType = 1;
       else if (s2 == 0)
               vehicleType = 0;
       signal = ! (s1 | s2);
       cleanMatrix();
       raceTime = millis();
       time = millis();
       bonusTime = - 10000;
       OK = 1;
     }
     else signal = 0;

     return signal;
}

void loop()
{    
    if (! end_game())
    {
      if (loops == 0)                                                                               //la primul lup, se aloca memorie pentru vehiculul controlat
       {
         lc.clearDisplay (0);
         if (vehicleType)
              a = new car;
             else a = new spaceship;
         loops++;
       }
       if(score < 700)
          race (* a, b, vehicleType, ncars, calcLevel (score));                                     //daca jucatorul nu a atins scorul maxim de 700 de puncte, participa in continuae la cursa
       else if(score == 700)
              playAgain(700);
       yj = analogRead (Y);
    }
}

