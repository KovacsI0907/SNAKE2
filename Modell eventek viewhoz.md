## A view frissítéséhez szükséges modellbeli eventek és a hozzájuk tartozó csomag osztályok
`<Event name>` : `<Packet name>`

### Létrehozás
- `GameItemCreatedEvent` : `GameItemPacket`
- `PlayerSnakeCreatedEvent` : `PlayerSnakePacket`
- `TileCreatedEvent` : `TilePacket`

### Frissítés
- `SnakeStepEvent` : `SnakeUpdatePacket`
A SnakeStepEvent a kígyó lépése után kell hogy meghívódjon és a SnakeUpdatePacket-et kell elküldenie. Ez tartalmazza a kígyó új hosszát és a fej új pozícióját.
- `SnakeDeathEvent` : `SnakeDeathPacket`c
- `TileStateChangedEvent` : `TileStatePacket`
A Tile állapotának változásakor (a kör szűkülésekor)
- `GameItemDeletedEvent` : `ItemDeletePacket`
Bármilyen törlés esetén (pl.: evéskor vagy obstacle megsemmisítésekor is)
- `PowerUsedEvent` : `EffectsChangedPacket`
- `EffectExpiredEvent` : `EffectsChangedPacket`

### Általános
- `GameStartEvent` : `GameStartPacket`
A meccs elindítása után rögtön kell küldeni, előtte a játékot fel kell építeni (lásd. Létrehozás)
- `GameEndEvent` : `GameEndPacket`
- `PlayerDisconnectedEvent` : `NetworkErrorPacket`
- `GamePausedEvent` : `NetworkErrorPacket`
