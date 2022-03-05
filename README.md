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