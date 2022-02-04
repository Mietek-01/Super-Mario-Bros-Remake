# Super-Mario-Bros-Remake

![Browser Screenshot](https://github.com/Mietek-01/Super-Mario-Bros-Remake/blob/master/Screens/Start%20Menu.png)

Remake-a tej jakże kultowej gry napisałem przy użyciu języka C++ z wykorzystaniem biblioteki SFML. 
Tworzyłem go przez okres drugiej klasy liceum i był to mój pierwszy tak rozbudowany projekt.

Zaczynałem z bardzo małym doświadczeniem w programowaniu obiektowym, także projekt ten pozwolił mi na lepsze zrozumienie tego paradygmatu. 
Przełożyło się to znacznie na moją późniejszą naukę języka C#, a to na stworzenie mojej własnej gry pt. [Depther.](https://github.com/Mietek-01/Depther) 

Gra jest przeznaczona na Windows-a a assety tj. textury i dźwięki pobrałem z [tego projektu.](https://github.com/PfAndrey/supermariohd)
 Dzięki nim mogłem w pełni skupić się na pisaniu kodu i implementowaniu własnych rozwiązań, za co jestem autorowi bardzo wdzięczny.

## Opis gry

Gra jest remake-iem gry „Super Mario Bros” i zawiera jej oryginalną mechanikę. Składa się z trzech poziomów
, które sam zaprojektowałem przy użyciu kreatora, którego również własnoręcznie napisałem.

![Browser Screenshot](https://github.com/Mietek-01/Super-Mario-Bros-Remake/blob/master/Screens/1.png)

![Browser Screenshot](https://github.com/Mietek-01/Super-Mario-Bros-Remake/blob/master/Screens/2.png)

![Browser Screenshot](https://github.com/Mietek-01/Super-Mario-Bros-Remake/blob/master/Screens/4.png)

![Browser Screenshot](https://github.com/Mietek-01/Super-Mario-Bros-Remake/blob/master/Screens/3.png)

Dodatkowo gra zawiera poziom walki z bossem, który jest już moim autorskim dodatkiem stworzonym na wzór gier typu „Mortal Kombat”. 
Tworzenie AI dla Bowsera było jednym z fajniejszych etapów podczas tworzenia tej gry. 

![Browser Screenshot](https://github.com/Mietek-01/Super-Mario-Bros-Remake/blob/master/Screens/5.png)

## Odnośniki

- Jeśli chcesz zagrać możesz pobrać folder Mario.zip z sekcji [Releases](https://github.com/Mietek-01/Super-Mario-Bros-Remake/releases).

- W folderze [Gameplay](https://github.com/Mietek-01/Super-Mario-Bros-Remake/tree/master/Gameplay) znajduje się filmik prezętujący grę.

- W folderze [VSCodeCounter](https://github.com/Mietek-01/Super-Mario-Bros-Remake/blob/master/.VSCodeCounter/2022-01-31_23-07-07/results.md) znajduje się spis wszystkich
plików jakie owa gra zawiera.

- W folderze [C++ Files](https://github.com/Mietek-01/Super-Mario-Bros-Remake/tree/master/C%2B%2B%20Files) znajdują się wszystkie pliki gry.

## Opis projektu

Gra NIE jest stworzona na silniku, a z wykorzystaniem biblioteki SFML. Wszystko musiałem pisać ręcznie jak np. system kolizji, podstawową fizykę
, mechanizm wczytywania poziomu czy system usuwania i dynamicznego tworzenia obiektów. 

W efekcie stworzyłem ok. 90 plików które w rezultacie dają 11 000 linijek kodu. 

Niestety z faktu, iż ową grę pisałem trzy lata temu i dopiero stawiałem pierwsze kroki w programowaniu obiektowym, czytelność kodu nie jest na zbyt dobrym poziomie.

Z doświadczeniem jakie teraz posiadam wiele rzeczy zrobił bym inaczej, a zwłaszcza lepiej nazywał poszczególne zmiennie czy metody.
Komentarze również mogą nie być na za dobrym poziomie, bo prawdopodobnie nie przykładałem wtedy do tego zbyt dużej wagi.

Na całkowity refactoring projektu nie mam po prostu czasu ani jakiś szczególnych chęci zwłaszcza, że z C++ już raczej nie chce mieć do czynienia
, ale jak to mówią nigdy nie mów nigdy. 

Jak patrzę teraz na ten projekt z perspektywy czasu, jestem w szoku ze skali determinacji jaką posiadałem te trzy lata temu.
Zrobienie takiej samej gry na silniku mogło by wiązać się z kilkukrotnie mniejszym nakładem pracy, jednak uważam że koniec końców wyszło mi to na dobre.
Wymyślenie i zaimplementowanie tych wszystkich mechanizmów pozwoliło mi wejść na zupełnie inny programistyczny poziom i znacznie lepiej zrozumieć proces tworzenia gier.

