# Grafika Beadandók



## Feladatok amiket teljesít az első beadandó:

-  A program létrehoz egy pontosan 600 pixel oldalhosszúságú négyzetes ablakot sárga háttérrel. 
- A képernyő közepén jelenítsünk meg egy r = 50 sugarú beszínezett kört, zöld határvonallal, piros centrummal és a kettő között lineárisan interpolált árnyalatokkal. 
- A kör induljon középről és legyen képes az x-tengely mentén vízszintesen mozogni oda-vissza olyan módon, hogy a képernyő szélét pontosan érintve a kör visszapattan.
-  A képernyő közepén kék színnel rajzoljunk harmad oldalhosszúságú, 3 pixel vastagságú vízszintes vonalat.

## Plusz funkciók amire képes az első beadandó:
- A program legyen képes a felhasználói interakció alapvető kezelésére. A fel és le billentyűkkel lehessen a szimmetriatengelyt mozgatni a megfelelő irányba.
- Amikor a körlap mozgás közben már nincs metszésben a szakasszal, a határvonal zöld és a centrum piros árnyalata cserélődjön fel. Ez hatással lesz az interpolált belső pontokra.
  - Innen is köszönöm mégegyszer a VBO-s tippet ezt használtam fel amit az órán említett nekem.
- S billentyű lenyomását követően a labda elkezd pattogni, hogy akció is legyen a történetben ezt gyorsítottam. 



## Feladatok amiket teljesít a második beadandó:

-  A program működése során felhasználja a 4 kontrollponttal megadott harmadfokú Bézier-görbe kirajzolását törött vonallal bemutató példaprogram függvényeit.
- A program képes 4 darab kerek kontrollpont kirajzolására is képes.
- A kontrollpontok a drag-and-drop technikával mozgathatóak. A kontrollpontok mozgásával egy időben a görbe is újrarajzolódik.

## Plusz funkciók amire képes a második beadandó:

- A görbét alkotó pontok mellett kirajzolható a kontrollpoligon is.
- A görbe, a kontrollpontok és a kontrollpoligon három eltérő tetszőleges színnel jelenik meg.
- A bal gombbal üres helyre kattintva hozzunk létre egy új kontrollpontot a kattintás helyén. Jobb gombbal meglévő kontrollpontra kattintva távolítsuk el azt. A fokszám módosítás után rajzoljuk újra a görbét az új kontrollpontok alapján. 


## Feladatok amiket teljesít a harmadik beadandó:

- A kockák körbejárhatóak egy olyan kamera segítségével, amely a térben egy hengerfelületen mozgatható, részleteiben pedig megfelel az alábbiaknak:
	- A kamera kezdőpozíciója legyen (r; 0; 0), ahol (8 ≤ r ≤ 10). 
	- Az UP vektorunk számára vegyük fel a (0; 0; 1) értéket.
	- A kamera mindig az origóba nézzen
	- A kamerát lehessen mozgatni a z-tengely körül, az r sugarú körön a jobb-bal nyilakkal.
	- A kamerát a z-tengely mentén fel-le tudjuk mozgatni a fel-le nyíl billentyűkkel.
	- A vetítés legyen perspective 55 értékkel.
- A jelenet tartalmazzon 3 darab egységkockát úgy, hogy az első legyen origó középpontú. A másik
kettő kockát helyezzük el úgy, hogy a fent definiált kamera képén a középső kocka felett és alatt
látszódjanak. A három darab kocka azonos méretű és egymástól olyan messze legyen, hogy közéjük
még éppen beférjen egy-egy kocka. A kockák színe legyen fehér.

## Plusz funkciók amire képes a második beadandó:

- Nem lett plusz funkció létrehozva a projekt készítése során.