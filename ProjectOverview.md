# Super Mario Bros Remake - Przegląd Projektu

Wierne odtworzenie gry Super Mario Bros napisane w C++ z wykorzystaniem biblioteki SFML (Simple and Fast Multimedia Library). Projekt został zbudowany od podstaw bez silnika gry — wszystkie kluczowe systemy (fizyka, wykrywanie kolizji, animacje, dźwięk, zarządzanie zasobami) są zaimplementowane ręcznie. Gra zawiera trzy autorskie poziomy, walkę z bossem, oryginalne AI Bowsera oraz edytor poziomów.

## Struktura katalogów

```
C++ Files/
├── Main.cpp                          # Punkt wejścia aplikacji
├── Classes/
│   ├── MarioGame.h/cpp               # Singleton: główna pętla gry, zarządzanie scenami
│   ├── Managers.h/cpp                # Szablony menedżerów zasobów (tekstury, czcionki, muzyka, dźwięki)
│   ├── EventHandler.h/cpp            # Mapowanie wejścia i obsługa zdarzeń
│   │
│   ├── Scens/                        # System zarządzania scenami
│   │   ├── Scen.h/cpp                # Abstrakcyjna klasa bazowa sceny
│   │   ├── GameScen.h/cpp            # Główna scena rozgrywki
│   │   ├── MenuScen.h/cpp            # Scena menu
│   │   └── LoadingGameScen.h/cpp     # Parser i loader plików poziomów
│   │
│   ├── GameClasses/                  # Klasy obiektów gry
│   │   ├── GameObject.h/cpp          # Abstrakcyjna baza dla wszystkich obiektów gry
│   │   ├── PhysicalObject.h/cpp      # Baza dla obiektów z fizyką (grawitacja, prędkość)
│   │   ├── Mario.h/cpp               # Postać gracza (ruch, poziomy, umiejętności)
│   │   ├── Landscape.h/cpp           # Statyczne elementy dekoracyjne tła
│   │   ├── Shield.h/cpp              # System tarczy/zdrowia (używany przez Mario i Bowsera)
│   │   ├── Princess.cpp              # NPC na końcu poziomu
│   │   │
│   │   ├── Blocks/                   # Typy bloków
│   │   │   ├── Block.h/cpp           # Blok bazowy (DynamicBlock, StaticBlock)
│   │   │   ├── Blocks.h              # Definicje typów bloków
│   │   │   ├── Barse.cpp             # Ruchome platformy (poziom Bowsera)
│   │   │   ├── MysterBox.h/cpp       # Bloki z pytajnikiem (monety, grzyby, kwiaty)
│   │   │   ├── MoveableBlock.h/cpp   # Ruchome platformy
│   │   │   ├── Pipe.h/cpp            # Rury z Kwiatkami Piranha
│   │   │   ├── SpecialPipe.h/cpp     # Przejścia do podziemnych poziomów
│   │   │   └── FieryBlock.h/cpp      # Bloki z ognistymi pociskami
│   │   │
│   │   ├── Enemies/                  # Typy wrogów
│   │   │   ├── Enemies.h             # Wspólne definicje i klasy bazowe wrogów
│   │   │   ├── Bowser.h/cpp          # Boss ze złożonym AI i wieloma atakami
│   │   │   ├── Goombas.cpp           # Goomby
│   │   │   ├── Turtle.cpp            # Podstawowy Koopa Troopa
│   │   │   ├── RedTurtle.cpp         # Czerwony wariant Koopy
│   │   │   ├── FlyingTurtle.cpp      # Paratroopa (latający)
│   │   │   ├── ArmedTurtle.h/cpp     # Żółw z bronią
│   │   │   ├── ArmoredTurtle.cpp     # Opancerzony wariant żółwia
│   │   │   ├── Flower.cpp            # Kwiatek Piranha w rurach
│   │   │   └── CreatorRedTurtles.cpp # Spawner wrogów
│   │   │
│   │   └── Items/                    # Przedmioty i pociski
│   │       ├── Items.h               # Definicje typów przedmiotów
│   │       ├── Coin.h/cpp            # Moneta do zebrania
│   │       ├── Mushroom.cpp          # Grzyb - power-up do wzrostu
│   │       ├── SpecialFlower.cpp     # Kwiat ognia - power-up
│   │       ├── Shell.h/cpp           # Skorupa żółwia (pocisk)
│   │       ├── Trampoline.h/cpp      # Trampolina
│   │       ├── LineWithFlag.h/cpp    # Flaga na końcu poziomu
│   │       ├── Gun.h/cpp             # Wieżyczka strzelająca
│   │       ├── GunBullet.cpp         # Pocisk wieżyczki
│   │       ├── MarioBullet.cpp       # Kula ognia gracza
│   │       ├── Hammer.cpp            # Młotek (pocisk)
│   │       ├── FireballBullets.h/cpp # Ognisty atak Bowsera
│   │       ├── LavaBullet.h/cpp      # Pocisk lawy
│   │       ├── LavaWaves.h/cpp       # Przeszkoda lawowa
│   │       └── WaterWaves.h/cpp      # Przeszkoda wodna
│   │
│   └── GUIClasses/                   # System interfejsu użytkownika
│       ├── GUI.h/cpp                 # Statyczny menedżer GUI (etykiety, menu, efekty)
│       ├── GUIObject.h               # Abstrakcyjna baza elementów GUI
│       ├── Animator.h/cpp            # System sprite'ów i animacji
│       ├── Menu.h/cpp                # Menu z etykietami i nawigacją
│       ├── Label.cpp                 # Pojedyncza opcja menu
│       ├── PointerToLabel.cpp        # Kursor menu
│       ├── FlowText.h/cpp            # Tymczasowy unoszący się tekst (np. "+100")
│       └── SpecialEffects.h/cpp      # Efekty wizualne (eksplozje, animacje śmierci)
```

## Hierarchia klas

```
GameObject (abstrakcyjna)
├── PhysicalObject (grawitacja, prędkość, kolizje)
│   ├── Mario (postać gracza + Shield)
│   ├── Wrogowie (Bowser, Turtle, Goombas, Flower, itp.)
│   ├── Przedmioty (Shell, Mushroom, MarioBullet, Hammer, itp.)
│   └── Barse (ruchome platformy)
├── Block (abstrakcyjna)
│   ├── DynamicBlock (reaguje na uderzenia)
│   └── StaticBlock (niereaktywny)
│       ├── MysterBox (generuje przedmioty)
│       ├── MoveableBlock (ruchome platformy)
│       ├── Pipe / SpecialPipe (strefy teleportacji)
│       └── FieryBlock (ogniste zagrożenia)
├── Coin (moneta)
├── Trampoline (trampolina)
├── LineWithFlag (cel poziomu)
├── Gun (wieżyczka)
├── LavaWaves / WaterWaves (przeszkody)
└── Landscape (dekoracje)

Scene (abstrakcyjna)
├── GameScene (główna rozgrywka)
├── MenuScene (menu)
└── LoadingGameScene (parser poziomów)

GuiObject (abstrakcyjna)
├── Animator → SpriteAnimator, Animations
├── Menu (z Label, PointerToLabel)
├── FlowText
└── SpecialEffects
```

## Główna pętla gry

Punkt wejścia: `Main.cpp` → `MarioGame::Instance().Run()`

Gra działa z limitem **110 FPS** (ustawionym przez SFML `setFramerateLimit`). Każda klatka wykonuje następujące kroki:

```
1. Pomiar czasu klatki          clock.restart() → delta time
2. Obsługa zdarzeń              EventHandler::HandleEvents(window)
3. Rysowanie                    window->clear()
                                  Scene::Draw(window)
                                  Gui::Draw(window)
4. Aktualizacja logiki          (pomijana gdy menu jest aktywne)
   (jeśli brak menu)              Scene::UpdateFrameTime(delta)
                                  Scene::Update()
                                  Gui::Update()
5. Wyświetlenie                 window->display()
```

Czas klatki (`sFrameTime`) i czas trwania sceny (`sDurationScene`) to zmienne statyczne klasy `Scene`, aktualizowane przed logiką gry. Gdy menu pauzy jest aktywne, rysowanie nadal działa, ale logika jest wstrzymana.

## Aktualizacja rozgrywki (GameUpdate)

Metoda `GameScene::GameUpdate()` to serce logiki gry. Wykonuje się w następującej kolejności:

```
1. Aktualizacja statycznych animacji
   ├── MysterBox::UpdateStaticAnimation()
   ├── Coin::UpdateStaticAnimation()
   ├── WaterWaves::UpdateStaticAnimation()
   └── LavaWaves::UpdateStaticAnimation()

2. Aktualizacja obiektów (tylko widocznych)
   ├── Mario::Update()
   ├── Kamera podąża za Mario (MoveCamera)
   ├── Block::Update()           — wszystkie widoczne bloki
   ├── PhysicalObject::Update()  — wszystkie widoczne obiekty fizyczne
   └── GameObject::Update()      — wszystkie widoczne obiekty niefizyczne

3. Kolizje z blokami
   ├── MarioBlockCollision()     — Mario vs wszystkie bloki
   └── PhysicalObjBlockCollision() — obiekty fizyczne vs bloki

4. Kolizje obiekt-obiekt
   ├── Mario vs przedmioty i wrogowie
   ├── Obiekty fizyczne vs obiekty fizyczne
   └── Obiekty fizyczne vs obiekty niefizyczne (dwukierunkowo)

5. Odroczone operacje
   ├── UpdateRemovedGameObjects() — usunięcie martwych obiektów z kontenerów
   └── UpdateNewGameObjects()     — dodanie nowych obiektów do kontenerów
```

## System fizyki

Klasa `PhysicalObject` implementuje fizykę opartą na delta time:

| Stała              | Wartość    | Opis                              |
|---------------------|-----------|-----------------------------------|
| `sGravitation`      | `2600.0f` | Przyspieszenie grawitacyjne       |
| `mJumpForce`        | `-900.0f` | Impuls skoku (ujemne = w górę)    |
| `mFallingForce`     | `250.0f`  | Początkowa prędkość spadania      |

Cykl aktualizacji fizyki per klatka:

```
1. Zapisz bieżącą pozycję jako poprzednią   mPreviousPosition = mCurrentPosition
2. Ruch poziomy (Move)                       na podstawie mKindMovement i mValueAcceleration
3. Skok/spadanie (MakeJump)                  mForce += sGravitation * deltaTime
                                             mCurrentPosition.y += mForce * deltaTime
4. Sprawdzenie czy obiekt spadł pod mapę     CheckUnderMap()
5. Aktualizacja animacji                     mAnimator->Update(mCurrentPosition)
```

Odpowiedź na kolizje z blokami (`UpdateForCollisionWithBlock`):
- **Dół** (`Bottom`) — obiekt ląduje na bloku, skok się kończy
- **Góra** (`Top`) — uderzenie głową, siła skoku zerowana
- **Boki** (`LeftSide`/`RightSide`) — odwrócenie kierunku ruchu

## Zarządzanie obiektami

Obiekty nie są dodawane ani usuwane natychmiast — używane są **odroczone kolejki**:

- `sNewObjects` — obiekty utworzone w trakcie klatki (np. grzyb wyskakujący z bloku)
- `sRemovedObjects` — obiekty oznaczone do usunięcia (`mIsDead = true`)

Oba kontenery przetwarzane są na **końcu klatki**, co zapobiega invalidacji iteratorów podczas pętli kolizji. Nowe obiekty trafiają do odpowiedniego kontenera na podstawie typu:

```
Nowy obiekt
├── Block?           → mBlocks (na początek listy)
├── PhysicalObject?  → mPhysicalObjs (na koniec)
└── Inny?            → mNotPhysicalObjs (na koniec)
```

## Maszyna stanów

`GameScene` zarządza dwupoziomową maszyną stanów:

```
GameStates (poziom zewnętrzny)
├── Gameplay → aktywna rozgrywka, używa GamePlayStates
├── GameOver → gracz stracił wszystkie życia (8s → menu)
└── WinGame  → gracz ukończył ostatni poziom (8s → menu)

GamePlayStates (poziom wewnętrzny, tylko w Gameplay)
├── MainGame                      — normalna gra
├── Warning                       — ostanie 1/4 czasu poziomu
├── MarioDead                     — Mario zginął (5s → restart lub GameOver)
├── ChangingLevelMario            — animacja transformacji power-up
├── LevelCompleted                — liczenie punktów za czas i życia
├── EnteringToPipe                — animacja wchodzenia do rury
├── LeavingUndergroundWorldByPipe — wychodzenie z podziemi
├── ReturnToBeforeWorld           — powrót po podziemnym poziomie
├── FightWithBowser               — walka z bossem
└── BowserDefeat                  — boss pokonany, czyszczenie
```

## Kolejność rysowania

```
1. Tło (background sprite)
2. Bloki (widoczne)
3. Krajobraz (Landscape)
4. Obiekty niefizyczne (widoczne)
5. Obiekty fizyczne (widoczne)
6. Mario (zawsze na wierzchu)
```

Widoczność obiektów ustalana jest na podstawie granic kamery (`mBoundsView`). Obiekty poza ekranem nie są ani rysowane, ani aktualizowane.

## Kamera

Kamera podąża za Mario w osi poziomej. Gdy pozycja Mario przekracza połowę szerokości okna, widok przesuwa się o tę samą odległość co Mario. Tło i etykiety GUI przesuwają się razem z kamerą.

## Zarządzanie pamięcią (RAII i smart pointery)

Projekt **częściowo** stosuje RAII i smart pointery — w warstwie zarządzania zasobami i scenami są używane konsekwentnie, ale w warstwie GUI i obiektów gry występuje ręczne zarządzanie pamięcią (`new`/`delete`).

**Gdzie stosowane są smart pointery:**

| Typ                  | Zastosowanie                                                     |
|----------------------|------------------------------------------------------------------|
| `unique_ptr`         | `ResourceManager::mResources`, `MarioGame::mWindow`,            |
|                      | `MarioGame::mCurrentScene`, `MarioGame::mGui`,                  |
|                      | `MarioGame::mEventHandler`, `GameScene::mMario`,                |
|                      | `Menu::mPointerToLabel`, `Shield::mShieldPointer`                |
| `shared_ptr`         | Kontenery obiektów gry w `GameScene` (`mBlocks`, `mPhysicalObjs`,|
|                      | `mNotPhysicalObjs`, `mLandscapes`) — współdzielone z `SpecialPipe`|
|                      | podczas przejść między poziomami                                 |
| `unique_ptr` w kontenerach | `list<unique_ptr<Block>>`, `list<unique_ptr<PhysicalObject>>`,|
|                      | `list<unique_ptr<GameObject>>` — automatyczne zwalnianie         |

**Gdzie używane jest ręczne `new`/`delete`:**

- **GUI** — `Label`, `Menu`, `FlowText`, `SpecialEffects` tworzone przez `new` i ręcznie usuwane w `Gui::Update()`. Komentarz w kodzie wyjaśnia: GUI potrzebuje kontroli nad kolejnością usuwania, bo destruktory `GuiObject` mogą dodawać nowe obiekty GUI
- **Sprite'y** — `sf::Sprite*` tworzone w `Gui::CreateSprite()` i `Gui::CreateText()`, przekazywane do `SpriteAnimator` który je usuwa w destruktorze
- **Singleton** — `MarioGame` alokowany przez `new` w `Instance()`, zwolniony przez `delete this` w `Run()` po zamknięciu okna
- **Obiekty gry (wyjątki)** — `SpecialPipe` ręcznie zarządza `EnteringPipe` (`delete pActivatePipe`), `Coin` ręcznie usuwa statyczną animację

**Blokowanie kopiowania:**

Klasy z zasobami blokują kopiowanie przez `= delete`:
- `ResourceManager` — `ResourceManager(const ResourceManager&) = delete`
- `Menu` — `Menu(const Label&) = delete`
- `PointerToLabel` — `PointerToLabel(const PointerToLabel&) = delete`

## Wzorce architektoniczne

- **Singleton** — `MarioGame::Instance()` zarządza pętlą gry, scenami i wszystkimi menedżerami zasobów
- **Zarządzanie scenami** — Abstrakcyjna baza `Scene` z implementacjami `GameScene`, `MenuScene`, `LoadingGameScene`; przejścia przez `MarioGame::ChangeScene()`
- **Szablonowy menedżer zasobów** — `ResourceManager<T>` generycznie ładuje tekstury, czcionki, muzykę i dźwięki ze specjalizacjami dla SFML
- **Maszyna stanów** — `GameScene` śledzi `GamePlayStates` (MainGame, Warning, MarioDead, FightWithBowser, EnteringToPipe, itp.) oraz `GameStates` (Gameplay, GameOver, WinGame)
- **Kompozycja** — Obiekty posiadają wskaźniki `Animator` do renderowania; Mario i Bowser współdzielą komponent `Shield` do zarządzania zdrowiem
- **Wzorzec callback** — `EventHandler` i etykiety `Menu` używają `std::function` do obsługi akcji
- **Pula obiektów** — `SoundManager` utrzymuje pulę 25 obiektów `sf::Sound` do ponownego użycia

## Konwencje nazewnictwa

| Element                | Konwencja        | Przykład                         |
|------------------------|------------------|----------------------------------|
| Klasy                  | PascalCase       | `MarioGame`, `PhysicalObject`    |
| Metody                 | PascalCase       | `Update()`, `Draw()`, `ActOnMe()`|
| Zmienne instancji      | mCamelCase       | `mCurrentPosition`, `mIsJump`    |
| Zmienne statyczne      | sCamelCase       | `sInstance`, `sFrameTime`        |
| Stałe                  | kCamelCase       | `kScaleToTile`                   |
| Parametry              | pCamelCase       | `pPos`, `pWindow`, `pValue`      |
| Klasy enum             | PascalCase       | `GamePlayStates`, `KindCollision`|
| Wartości enum          | PascalCase       | `MainGame`, `LeftSide`, `Bottom` |

## Zależności

- **SFML/Graphics** — Renderowanie, sprite'y, kształty, tekst, widoki
- **SFML/Audio** — Odtwarzanie muzyki i efektów dźwiękowych
- **Biblioteka standardowa C++** — `<memory>`, `<vector>`, `<map>`, `<list>`, `<string>`, `<functional>`, `<fstream>`, `<cassert>`, `<stdexcept>`

## AI Bowsera

Bowser posiada złożone, wielofazowe AI z sześcioma wzorcami ataku:
- **BasicFireBallAttack** — Standardowe ogniste pociski
- **SpecialCircleFireBallAttack** — Okrężny wzorzec ognia
- **SpecialSinFireBallAttack** — Trajektoria ognia w kształcie sinusoidy
- **LeapOnMario** — Atak skokiem w kierunku Mario
- **Charge** — Atak szarżą
- **EnemiesRaid** — Dynamiczne generowanie wrogów podczas walki
