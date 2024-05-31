## SnakeArena Online
A játék a mindenki által ismert snake játék egy többjátékos online változata. Ez a játék a sima játékon felül újabb dolgokat is tartalmaz, mint például új pályelemek,játéklogikák, lehetőségek.

### Gyümölcsök
A gyümölcsöket aktiválni kell miután megettük, hogy az effectet megkapd. Az utolsó gyümölcs amit a játékos megevett az a használatáig el van tárolva.

- **Felgyorsító gyümölcs**: Felgyorsítja azt a játékost, aki megette.
- **Támadó gyümölcs**: Egy adott időre lehetőséget ad a játékosnak más játékosok kígyójának megevésére. Ha ennek a gyümölcsnek a hatása alatt a játékos átmegy egy másik játékos kígyójának egy részén, akkor azt a játékos elveszti.
- **Védő gyümölcs**: Megvéd a támadó gyümölcstől.(oldalra fordítja a támadót)
- **Bemenet megfordító gyümölcs**: Egy véletlenszerű játékosnak (kivétel az, aki megette) adott időre megfordítja a bementeit.
- **Lassító gyümölcs**: Lassítja a legközlebbi játékost (a megevőn kívül).
- **Több pontot adó gyümölcs**: Több pontot ad, mint az alap gyümölcs.
- **Teleportkapu**: Átteleportálja azt a játékost, aki belement.

### Játékosszám
Ebben a játékban a maximális játékosok száma 4.

### Pálya
A pálya rácsokból áll. A pálya mérete a játékosszámhoz  alkalmazkodik (például 4 játékos esetén nagyobb a pálya, mint 3 játékos esetén). A játék során a pálya szűkül.

- Vannak a pályán obsticale-ök amiknek ha nekimegy a játékos akkor meghal (ha támadő gyümölcsel megy neki akkor viszont lerombolja és nem hal meg)

### Különleges esetek 
- Két játékos szembe megy egymással és ütköznek: a kevesebb pontal rendelkező (kissebb) játékos kiesik, a nagyobb pedig elveszít annyi pontot mint amennyi a kissebbnek volt.
- Ha az utolsó két játékos megy szembe egymással, ütköznek és ugyanannyi pontjuk van, akkor döntetlen.
- Ha egy játékos kilép vagy elveszti a kapcsolatot a szerverrel, akkor  a játék megáll és adott ideig vár, ha nem csatlakozik vissza a játékos akkor kiesik és a játék folytatódik
- Ha egy játékos kigyójának egy része kilóg a pálya szűkülése során, akkor azt elveszti, kivétel, ha ez a kigyó feje mert akkor kiesik.
- Ha két játékos egyszerre megy a teleportkapuba, akkor ugyanaz játszódik le mint az ütközésnél (a kevesebb pontal rendelkező játékos kiesik)
- Ha két játékos maradt és az egyik lecsatlakozik és nem csatlakozik vissza a játékba akkor az a játékos nyert aki bent maradt a játékban.
- Ha egy játékos hosszába támad a támadó gyümölcsel akkor addig támad ameddig hat a gyömülcs (ha nem fordul el időben akkor kiesik)
- Ha egy játékos szembe támad (a másik játékos fejét eltalálja) a támadó gyümölcsel akkor insta kill,de ha a másik játékos kivédi a védő gyümölcsel akkor midkét játékos elfordul, ha pedig a másik játékos is használja a támadó gyümölcsöt akkor az történik mint az ütközésnél (a kissebb kiesik).

### Játék Vége
- A játék véget ér, ha egy játékos marad, ekkor ő nyert.
- A játéknak vége ha az utolsó kettő játékos egyszerre esik ki, ekkor döntetlen.

