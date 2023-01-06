# POS-Tanks


## Client
Klientská aplikácia zobrazuje grafické okno a stará sa o komunkáciu so serverom.<br />
Po spustení aplikácie sa zobrazí grafické okno kde sa hráč naviguje pomocou šipiek. Hráč si zvolí meno, IP adresu servera a port na ktorom tento server počúva.<br />
Pre naklonovanie repozitára musí hrač spraviť.
```
mkdir pos-tanks
git clone https://gitlab.com/LogiMou/pos-tanky.git
```
Následne sa hráč presunie do klientského adresára s názvom ***klient***. Nižšie su uvedené kroky potrebné na to aby bolo možné spustiť klientskú časť.
### MinGW-64
Klient beží na kompilátoru **MinGW-64**. Preto odporúčamé pred spustením hry mať tento kompílator nainštalovaný.
Sú rôzne cesty ako nainštalovať MinGW-64. Spomeniem 2 možnosti ako sme postupovali my. Ak budete postupovať pomocou druhého
spôsobu tak si zároveň aj nainštalujete ***make*** pomocou ktorého si budete môcť vyskúšat spustiť Clientov **MakeFile**.

```
1. Stiahnuť Clion kde natívne sa nainštaluje MinGW-64
2. Postupovať podľa tohto tutoriálu: https://www.youtube.com/watch?v=Zcy981HhGw0&ab_channel=SemicolonDotDev.
```

### SFML knižnica
V tejto časti ukážem ako nainštalovať **SMFL** knižnicu ktorá je potrebná pre beh hry. Z tejto knižnice využiváme grafické časti
a tak isto aj sieťové komponenty pomocou ktorý komunikujeme medzi serverom a klientom. Nižšie ukážem step-by-step postup ako si do projektu importovať **SFML** knižnicu.

```
1. Prejsť na link: https://www.sfml-dev.org/download/sfml/2.5.1/
2. Stiahnuť **MinGW** verziu knižnice podľa toho aku ste si nainštalovali. Ak ste postupovali podľa kroku vyššie tak zvoľte 64-bit verziu
3. Rozbaľte stiahnuty súbor.
4. V projekte vytvorte adresár s názvom **src**
5. Do adresára src prekopírujte stiahnute adresáre s názvami **lib** a **include**
6. Po piatom kroku by ste mali mať v adresári src 2 adresáre s názvami lib a src
7. Teraz už len stačí reloadnuť CmakeList a skompilovať projekt
8. Prajem príjemne hranie :)
```

## Server
Serverová časť sa stará komzúnikácie s klientmi. Čaká na pripojenia sa všetkých hráčov, posiela začiatočné nastavenia hráčom (ich ID, súradnice kde sa majú postaviť,...). Zapisuje skóre a po ukončení hrý rozpošle hráčom ich štatistiky.<br />
Po spustení servera je vyžiadaný vstup od administrátora servera pre počet hráčov koľko bude v danej hre. Následne keď do konzoly zadá počet hráčov tak sa čaká na ich pripojenie. Po ich pripojení sa rozpošlú inicializačné nastavenia a spustí sa hra.<br />

### Spustenie servera
Predpokladám že repozitar že je naklonovaný a že serverová aplikácia beží na ***frios2.fri.uniza.sk*** servery. Ak sú všetky tieto podmienky splnené tak už netreba nič nastavovať a len spustiť server.<br />
Dá sa to pomocou dvoch spôsobov:
```
1. Pomocou terminálu a Makefilu
    - Otvorím terminálove okno
    - Prejdem do adresára s názvom server
    - Napišem príkaz **make**
    - Následne len spustím vytvoreny súbor pomocou príkazu **./server**
    - Prajem príjemne hranie :)
2. Pomocou Clionu
    - Na Clione mame nastavený remote development
    - Otvorím adresár server pomocou Clionu
    - Počkám kým sa prenesú súbory na server
    - Reloadnem CmakeList
    - Spustím aplikáciu
    - Prajem príjemne hranie :)
```