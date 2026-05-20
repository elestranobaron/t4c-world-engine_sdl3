# Crédits — moteur TnC SDL3 (`client_graphical_sdl3_test`)

Moteur carte / sprites compilé dans le client Linux sibling [`../client/`](../client/) via `cmake/TncGraphical.cmake` (`TNC_GRAPHICAL_ROOT`).

> **Renommage prévu :** `t4c-tnc-engine` ou `tnc_sdl3` — ce dossier n’est plus un « test », c’est le moteur de prod.

---

## Contributeurs (port SDL3, 2026-05)

- **Tom — ElEsTaNoBaRoN** — direction, tests en jeu, validation visuelle, intégration client.
- **Cursor (agent IA / Composer)** — migration SDL3 native : `tnc_sdl3.h`, MapInterface, palettes/bake RGBA, `Sdl3FramePresenter`.

## Moteur d’origine (TnC / mestoph)

- **Noth (2005)** — *TnC The Next Coming* (`TnC_dev/` : MapInterface, VSFInterface, NPCManager, FontManager, TextManager) — [tnc.sourceforge.net](http://tnc.sourceforge.net/) — **GPL v2**.

## Référence jeu

- **Vircom / The 4th Coming** — assets et client d’origine (non redistribués ici).

---

## VSF — propriétaire ou pas ?

| Élément | Statut |
|---------|--------|
| **Format VSF + fichiers** (`.vsf`, `.dec`, maps, sons) | Contenu **T4C** — **non libre** ; ne pas committer dans un repo public |
| **Code `VSFInterface/`** (lecteur) | **GPL v2** (Noth) |

---

## Arborescence

| Chemin | Rôle |
|--------|------|
| **`TnC_dev/`** (ici) | Sources compilées par `t4c_client` et binaire démo `mapi_sdl3` |
| `TnC_dev/include/tnc_sdl3.h` | Helpers SDL3 natifs (remplace l’ancien shim) |
| `TnC_dev/render/` | `Sdl3FramePresenter` |
| `../client/` | Exécutable, réseau, GUI |
| `../client_graphical_path_to_follow/` | Labo offline (convert2) — **pas** la racine de build client |

---

## Licence — GPL vs Apache

- Code Noth + modifications SDL3 → **GPL-2.0-or-later** (ajouter un fichier `LICENSE` à la racine de ce repo quand il sera versionné).
- **Apache seul** sur ce moteur : non (œuvre dérivée GPL).
- Linké dans `t4c_client` → le client reste GPL aussi.

---

## Assets dans Git

- Ne pas versionner sprites/maps convertis (`.dec`, `.rmap`) dans un repo **public**.
- Runtime : `T4C_DATA` ou `data/` en local ; voir aussi `../client/CREDITS.md`.
