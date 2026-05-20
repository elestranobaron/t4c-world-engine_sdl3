# t4c-world-engine_sdl3
Moteur graphique **TnC The Next Coming** (mestoph / Noth), porté **SDL3 natif** pour *The 4th Coming*.
Compilé dans le client Linux sibling [`t4c_client`](https://github.com/elestranobaron/t4c-168-vs2022) via `cmake/TncGraphical.cmake`.
> Ancien nom local : `client_graphical_sdl3_test` — ce repo est le **moteur de prod**, pas un labo de test.
## Contenu
| Dossier | Rôle |
|---------|------|
| `TnC_dev/MapInterface/` | Carte, tuiles, redraw |
| `TnC_dev/VSFInterface/` | Sprites `.dec`, palettes |
| `TnC_dev/NPCManager/` | PNJ animés |
| `TnC_dev/FontManager/` | Polices bitmap |
| `TnC_dev/TextManager/` | Texte à l'écran |
| `TnC_dev/include/tnc_sdl3.h` | Helpers SDL3 (remplace l'ancien shim SDL2) |
| `TnC_dev/render/` | `Sdl3FramePresenter` |
## Prérequis
- CMake ≥ 3.16
- SDL3, SDL3_image
- zlib
## Build (démo carte)
```bash
mkdir -p build && cd build
cmake ..
cmake --build . -j$(nproc)
# binaire : build/TnC_dev/mapi_sdl3
Usage :

./TnC_dev/mapi_sdl3 <x> <y> <zone>
# ex. Lighthaven : ./TnC_dev/mapi_sdl3 2880 1083 0
# zone : 0=world, 1=dungeon, 2=cavern, 3=underworld
Données runtime (T4C)
Non incluses dans ce repo (copyright T4C / Vircom).

Placer localement :

data/sprites/ — fichiers .dec
data/maps/ — fichiers .rmap
fonts/, NPCList.txt, modif_sprites.txt à côté du binaire ou sous TnC_dev/
Convertir depuis une installation T4C avec le labo client_graphical_path_to_follow (convert2).

Intégration client
Dans ../client/cmake/TncGraphical.cmake, ce repo est la priorité 1 :

../t4c-tnc-engine/TnC_dev   # ou chemin sibling actuel
Clone sibling recommandé :

finalstep/
├── client/
└── t4c-tnc-engine/    ← ce repo
Licence
Code mestoph (Noth, 2005) : GPL-2.0-or-later — voir en-têtes des sources et tnc.sourceforge.net.
Modifications SDL3 (2026) : même licence (œuvre dérivée).
Assets T4C (sprites, maps, sons) : non redistribuables ici.
Voir CREDITS.md.

Crédits
Tom — ElEsTaNoBaRoN — port et intégration
Noth (2005) — TnC original
Cursor (agent IA) — migration SDL3 native, fix palettes/couleurs
