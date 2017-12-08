# Introducere în robotică cu Arduino - Tema 1

Pârjoleanu Andi - Olivian, Grupa 251

Joc ales : Curse (Racing game)

# Descriere : 

Pe matricea cu LED-uri, va apărea inițial un vehicul cu care un pilot (jucătorul) va participa la o cursă. În timpul jocului, vor apărea pe matrice și alte vehicule care trebuie depășite de pilot pentru a câștiga, însă el trebuie să evite să le lovească pentru a nu fi descalificat din cursă. Juriul este indulgent și îi permite concurentului două abateri. De fiecare dată când lovește un vehicul, pilotul reia cursa de unde a rămas, însă juriul reține evenimentul drept o abatere. Dacă pilotul reușește să depășească un anumit număr de vehicule într-un singur tur, se califică în etapa următoare a competiției, care va fi mai dificilă deoarece se impune participanților să concureze cu o viteză mai mare. La fiecare etapă, limita de viteză crește, implicit și dificultatea.
Jocul are două versiuni. 

În funcție de preferințe, jucătorul poate participa fie la o cursă de mașini, fie la o cursă de navete spațiale. Regulile de joc sunt aceleași pentru ambele versiuni. Cursele sunt organizate pe o pistă cu două benzi, mărginită de două ziduri laterale luminoase. Pe drumul spre linia de sosire, pilotul poate găsi oportunități de bonus oferite de echipa de mentenanță.

# Materiale folosite pentru realizarea jocului:

Componenta hardware a jocului cuprinde: placă Arduino Uno, matrice de LED-uri 8x8, 3 breadboard-uri, ecran LCD, un joystick, condensator electrolitic, 2 butoane, driver MAX 7219, 4 rezistențe de 10 kΩ,  fire de legătură, carcasă de carton.

# Metodele de control:

Jucătorul poate controla vehiculul cu care participă la cursă cu ajutorul unui joystick care citește poziții de pe axa absciselor. El trebuie să evite coliziunea cu adversarii prin schimbarea poziției inițiale a joystick-ului.
Butoanele din circuit sunt folosite pentru pornirea jocului. Prin apăsarea unui buton, jucătorul își alege tipul de cursă la care participă. Butonul stâng corespunde cursei de navete spațiale, butonul drept corespunde cursei de mașini. După apăsarea unuia dintre butoane, jucătorul intră direct în cursă.

# Progresul jucătorului:

Jocul are 10 niveluri. Dacă pilotul reușește să obțină un anumit punctaj, se califică într-o etapă ulterioară, echivalentă unui nivel. Viteza minimă cu care pilotul trebuie să concureze diferă pentru fiecare nivel în parte. Viteza se măsoară în unități de distanță (1 linie pe matrice) pe milisecunde.

Nivel 1 (0 - 40 de puncte): viteza minimă 1/250

Nivel 2 (40 - 100 de puncte): viteza minimă 1/200

Nivel 3 (100 - 180 de puncte) : viteza minimă 1/150

Nivel 4 (180 - 200 de puncte) : viteza minimă 1/100

Nivel 5 (200 - 320 de puncte) : viteza minimă 1/90

Nivel 6 (320 - 450 de puncte) : viteza minimă 1/80

Nivel 7 (450 - 550 de puncte) : viteza minimă 1/70

Nivel 8 (550 - 620 de puncte) : viteza minimă 1/60

Nivel 9 (începând de la 620 de puncte) : viteza minimă 1/50

Dacă jucătorul reușește să strângă 700 de puncte fără să-și epuizeze numărul de abateri permise, primește un loc pe podium, iar arbitrul
afișează pe ecranul LCD mesajul ”YOU WON!!”. Un punct se obține după depășirea unei mașini.

În timpul cursei, sunt afișate pe ecran: scorul, nivelul (calculat în funcție de scor) și numărul de vieți rămase (numărul de abateri permise). Dacă jocul se termină, programul comunică jucătorului dacă scorul obținut depășește scorul maxim de până atunci.

Jucătorul poate primi bonusuri de la susținători. Acestea apar aleatoriu în program și sunt definite diferit pentru cele două tipuri de vehicule. Dacă jucătorul concurează cu o mașină, prin obținerea unui bonus, numărul de abateri permise crește. Dacă concurează cu o naetă spațială, bonusul oferă invizibilitate vehiculului timp de 5 secunde.

# Stilul abordat în implementare:

Pentru reprezentarea mașinilor și navetelor spațiale în memorie, am creat o clasă abstractă vehicle cu metoda virtuală pură bonus. Clasa vehicle este moștenită de clasele car și spaceship care implementează diferit funcția bonus. În clasa de bază există un câmp de tip matrice de întregi care este completat cu valori la apelul constructorilor claselor car și spaceship. Câmpul matrice este folosit pentru reprezentarea formei conceptuale a unui vehicul. Elementele cu valoarea 1 indică prezența unei componente a vehiculului pe o anumită poziție de pe matricea de LED-uri. În clasa de bază se mai regăsesc coordonatele centrului unui obiect în raport cu poziția acestuia pe matrice. Coordonata coloană primește valoare aleatoare (banda 1 sau 2 pe pista de curse), iar coordonata linie primește valoarea -1. Coordonatele vehiculului controlat de jucător vor primi alte valori inițiale, iar acestea vor fi modificate în funcție de poziția relativă a joystick-ului față de starea de echilibru. 

În program, este declarat global un vector de pointeri la clasa vehicul care reprezintă mulțimea de adversari prezenți la un moment dat pe ecran. În funcție de tipul de vehicul ales de jucător, se alocă dinamic memorie pentru tablou, pentru elementele sale și pentru obiectul controlat de jucător. De fiecare dată când primul adversar din vector este depășit și ajunge pe poziția 10, este șters din memorie. Dacă ultimul element adăugat se află pe o poziție egală cu un număr ales aleator din mulțimea {5, 8}, se poate adăuga un nou obiect în vector deoarece între cele două vehicule ar exista suficient spațiu astfel încât vehiculul controlat de jucător să evite ambii adversari. 

# Demo video
https://drive.google.com/file/d/1mhiqyCTOYBcjjmrwvEglge1n_Zf3vYt-/view?usp=sharing
https://drive.google.com/file/d/10PKWDFvOr7zn2-7P7YR4Sfuk0YnzeiaW/view?usp=sharing
