## SnakeArena Online
A játék a mindenki által ismert snake játék egy többjátékos online változata. Ez a játék a sima játékon felül újabb dolgokat is tartalmaz, mint például új pályelemek,játéklogikák, lehetőségek.

### Gyümölcsök
- **Felgyorsító gyümölcs**: Felgyorsítja azt a játékost, aki megette.
- **Támadó gyümölcs**: Egy adott időre lehetőséget ad a játékosnak más játékosok kígyójának megevésére. Ha ennek a gyümölcsnek a hatása alatt a játékos átmegy egy másik játékos kígyójának egy részén, akkor azt a játékos elveszti.
- **Bemenet megfordító gyümölcs**: Egy véletlenszerű játékosnak (kivétel az, aki megette) adott időre megfordítja a bementeit.
- **Lassító gyümölcs**: Lassítja azt a játékost, aki megette.
- **Több pontot adó gyümölcs**: Több pontot ad, mint az alap gyümölcs.
- **Teleportkapu**: Átteleportálja azt a játékost, aki belement.

### Játékosszám
Ebben a játékban a maximális játékosok száma 4.

### Pálya
A pálya rácsokból áll. A pálya mérete a játékosszámhoz  alkalmazkodik (például 4 játékos esetén nagyobb a pálya, mint 3 játékos esetén). A játék során, ha egy játékos kiesik vagy sokáig nem esik ki játékos, a pálya alkalmazkodik ehhez és összeszűkül.

### Különleges esetek 
- Két játékos szembe megy egymással és ütköznek: mindkettő játékos kiesik
- Ha az utolsó két játékos megy szembe egymással és ütköznek, akkor döntetlen.
- Ha egy játékos kilép vagy elveszti a kapcsolatot a szerverrel, akkor  a játék megáll és adott ideig vár, ha nem csatlakozik vissza a játékos akkor kiesik és a játék folytatódik
- Ha egy játékos kigyójának egy része kilóg a pálya szűkülése során, akkor azt elveszti, kivétel, ha ez a kigyó feje mert akkor kiesik.
- Ha két játékos egyszerre megy a teleportkapuba, akkor mindkettő kiesik.(Ha ez a két játékos az utolsó kettő játékos akkor döntetlen).
- Ha két játékos maradt és az egyik lecsatlakozik és nem csatlakozik vissza a játékba akkor az a játékos nyert aki bent maradt a játékban.

### Játék Vége
- A játék véget ér, ha egy játékos marad, ekkor ő nyert.
- A játéknak vége ha az utolsó kettő játékos egyszerre esik ki, ekkor döntetlen.

