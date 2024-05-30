## UI
### Kezdőképernyő
Induláskor illetve meccs vége után jelenik meg
- Név szövegdoboz
- Csatlakozás gomb
- Új játék (lobby) létrehozása gomb
- Kilépés asztalra gomb
### Csatlakozás popup/képernyő
- Szövegdoboz IP-cím és/vagy lobby ID beírására
- Csatlakozás gomb
### Lobby képernyő
A játék indulásáig ezt látja a csatlakozó játékos
- Kilépés gomb
- Játékosok listája
### Lobby képernyő (a lobby létrehozójának)
- Szerver IP szöveg
- Lobby ID szöveg
- Csatlakozott játékosok listája
- Indítás gomb
- Lobby törlése gomb
### Játék vége képernyő
Kiírja, hogy ki nyert
- OK gomb
### In-game menü
- Kilépés asztalra gomb
- Kilépés kezdőképernyőre gomb

## Játék
A pályát 3d-ben látjuk magas oldalnézetből. A pálya rácsából kiemelkednek "szigetek", ahol a kígyók nem mozoghatnak. A rácsvonalak jelölve lesznek.

## Technikai
A grafikus program nyelve C++. Megjelenítéshez az OpenGL-t használjuk.
A program feladatai:
- A kígyók, a pálya és a pályaelemek megjelenítése és animálása.
- A UI-hoz szükséges komponensek implementálása.
- Hangok lejátszása
- Intefészt ad a kígyók készítéséhez és a pozíció információk átvételéhez, ugyanez a pályára és a többi pályaelemre.
- Interfészt ad a UI komponensek használatához (pl.: gomb klikk callback függvények)
- Interfészt ad hanglejátszáshoz.
- Kezeli az alkalmazásablakot.
