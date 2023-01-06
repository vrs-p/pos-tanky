# POS-Tanks


## Client
Klientská aplikácia zobrazuje grafické okno a stará sa o komunkáciu so serverom.<br />
Po spustení aplikácie sa zobrazí grafické okno kde sa hráč naviguje pomocou šipiek. Hráč si zvolí meno, IP adresu servera a port na ktorom tento server počúva.<br />
Pre naklonovanie repozitára musí hrač spraviť:
```
mkdir pos-tanks
git clone https://gitlab.com/LogiMou/pos-tanky.git
```
Následne sa hráč presunie do klientského adresára s názvom ***klient***. Nižšie su uvedené kroky potrebné na to aby bolo možné spustiť klientskú časť.
### MinGW-64
Klient beží na kompilátore **MinGW-64**. Preto odporúčamé pred spustením hry mať tento kompílátor nainštalovaný.
Sú rôzne cesty ako nainštalovať MinGW-64. Spomenieme 2 možnosti, ktoré sme využili my. Ak budete postupovať pomocou druhého
spôsobu, tak si zároveň aj nainštalujete ***make*** pomocou ktorého si budete môcť vyskúšat spustiť Clientov **MakeFile**.

```
1. Stiahnuť Clion, kde sa natívne nainštaluje MinGW-64
2. Postupovať podľa tohto tutoriálu: https://www.youtube.com/watch?v=Zcy981HhGw0&ab_channel=SemicolonDotDev.
```

### SFML knižnica
V tejto časti ukážeme ako nainštalovať **SMFL** knižnicu, ktorá je potrebná pre beh hry. Z tejto knižnice využiváme grafické časti
a tak isto aj sieťové komponenty, pomocou ktorých prebieha komunikácia medzi serverom a klientom. Nižšie ukážeme step-by-step postup ako si do projektu importovať **SFML** knižnicu.

```
1. Prejsť na link: https://www.sfml-dev.org/download/sfml/2.5.1/
2. Stiahnuť **MinGW** verziu knižnice podľa toho, akú ste si nainštalovali. Ak ste postupovali podľa kroku vyššie, tak zvoľte 64-bit verziu
3. Rozbaľte stiahnutý súbor
4. V projekte vytvorte adresár s názvom **src**
5. Do adresára src prekopírujte stiahnuté adresáre s názvami **lib** a **include**
6. Po piatom kroku by ste mali mať v adresári src 2 adresáre s názvami lib a src
7. Teraz už len stačí reloadnuť CmakeList a skompilovať projekt
8. Prajeme príjemné hranie :)
```

## Server
Serverová časť sa stará komzúnikáciu s klientmi. Čaká na pripojenie sa všetkých hráčov, posiela začiatočné nastavenia hráčom (ich ID, súradnice, kde sa majú postaviť, ...). Zapisuje skóre a po ukončení hrý rozpošle hráčom ich štatistiky.<br />
Po spustení servera je vyžiadaný vstup od administrátora servera pre počet hráčov (koľko hráčov bude v danej hre). Následne, keď do konzoly zadá počet hráčov, tak sa čaká na ich pripojenie. Po ich pripojení sa rozpošlú inicializačné nastavenia a spustí sa hra.<br />

### Spustenie servera
Predpokladáme, že repozitár je naklonovaný a že serverová aplikácia beží na ***frios2.fri.uniza.sk*** servery. Ak sú všetky tieto podmienky splnené, tak už netreba nič nastavovať a len spustiť server.<br />
Dá sa to pomocou dvoch spôsobov:
```
1. Pomocou terminálu a Makefilu
    - Otvoríme terminálove okno
    - Prejdeme do adresára s názvom server
    - Napišeme príkaz **make**
    - Následne len spustíme vytvorený súbor pomocou príkazu **./server**
    - Prajeme príjemne hranie :)
2. Pomocou Clionu
    - Na Clione mame nastavený remote development
    - Otvoríme adresár server pomocou Clionu
    - Počkáme, kým sa prenesú súbory na server
    - Reloadneme CmakeList
    - Spustíme aplikáciu
    - Prajeme príjemné hranie :)
```