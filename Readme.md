# T4C — bac à sable graphique SDL3 (Phase 1)

Dossier jumeau de [`client_graphical_path_to_follow`](../client_graphical_path_to_follow) pour :

1. **Reproduire le pipeline offline** (`convert2` : VSF/VSB/maps → `data/`)
2. **Tester la carte isométrique directement en SDL3** (`mapi_sdl3`)

Aucune dépendance SDL2 embarquée : SDL3 système + shim de compatibilité pour le code TnC legacy.

## Arborescence

```
client_graphical_sdl3_test/
├── decode/          # convert2 (zlib uniquement)
├── TnC_dev/         # démo carte SDL3 + sources TnC (symlinks + MapInterface patché)
├── data/            # sortie convertisseur (symlink ou généré)
├── scripts/
│   ├── run_convert.sh
│   └── run_mapi_sdl3.sh
└── build/           # généré par CMake
```

## Prérequis

- CMake ≥ 3.16, compilateur C++17
- zlib
- SDL3, SDL3_image (paquets distro ou build local)
- Copie des **Game Files** T4C (voir référence TnC) : lien `decode/Game Files` ou chemin passé au script

## Étape 1 — Convertir les données

```bash
./scripts/run_convert.sh "/chemin/vers/T4C/Game Files" ./data
```

Équivalent historique :

```bash
cd decode && make   # référence SDL2 — non requis ici
./convert2 "Game Files" ../data/
```

Produit notamment :

- `data/sprites/t4cgamefile*.dec`
- `data/maps/*.rmap`
- `data/sons/*.wav`

L’index `decode/id_list.txt` (1525 entrées, incomplet) est copié en `id_list.txt.txt` pour la conversion des cartes.

## Étape 2 — Démo carte SDL3

```bash
./scripts/run_mapi_sdl3.sh 2880 1083 0   # Lighthaven, worldmap
```

Contrôles : flèches / pavé numérique, `F1` debug tuiles, `F12` capture `screen_sdl3.bmp`, `Esc` quitter.

## Détail technique SDL3

- `TnC_dev/include/SDL/SDL.h` : shim SDL2 → SDL3 (`tnc_sdl2_compat.h`)
- `TnC_dev/render/Sdl3FramePresenter.cpp` : surfaces offscreen → `SDL_RenderTexture`
- `MapInterface/mapi_full_redraw.cpp` : copie locale (`SDL_MapSurfaceRGB` pour les masques de fusion)

## Prochaines étapes (client principal)

- Porter `VSFInterface` vers textures GPU sans shim global
- Brancher après login dans `client/` (`LINUX_PORT`)
- Enrichir `id_list.txt` pour réduire les tuiles « ERR! »

Référence : [tnc.sourceforge.net](http://tnc.sourceforge.net/)
