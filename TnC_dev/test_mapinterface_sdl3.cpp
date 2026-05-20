/**
 * Démo carte TnC — variante SDL3 (Phase 1).
 * Usage: mapi_sdl3 <x> <y> <zone>
 *   zone: 0=world, 1=dungeon, 2=cavern, 3=underworld
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "MapInterface/mapinterface.h"
#include "NPCManager/npcmanager.h"
#include "TextManager/textmanager.h"
#include "VSFInterface/vsfinterface.h"
#include "render/Sdl3FramePresenter.h"
#include "tnc_sdl3.h"

#define PATH_SPRITES "data/sprites/"
#define PATH_MAPS "data/maps/"

static SDL_Surface *screen = nullptr;
static SDL_Surface *sol = nullptr;
static SDL_Surface *decor = nullptr;
static SDL_Surface *env = nullptr;

static MAPInterface *mapi = nullptr;
static VSFInterface *vsfi = nullptr;
static FontManager *fm = nullptr;
static FontManager *fm2 = nullptr;
static TextManager *txtm = nullptr;
static NPCManager *npcm = nullptr;

static bool map_flag = true;
static bool disp_infos = false;
static char str_infos[1024];
static float fps = 0.f;

static Sdl3FramePresenter presenter;
static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;

static char *dup_cstr(const char *s) {
    const size_t n = strlen(s);
    char *out = static_cast<char *>(malloc(n + 1));
    if (out) {
        memcpy(out, s, n + 1);
    }
    return out;
}

static void redraw(unsigned short zone, unsigned int loc_x, unsigned int loc_y) {
    const Uint32 tmptick = TnC_GetTicksMs();

    if (map_flag) {
        mapi->get_map(loc_x, loc_y, zone, sol, decor);
        map_flag = false;
    }

    TnC_FillArgb(screen, nullptr, 0xFF000000);
    SDL_BlitSurface(sol, nullptr, screen, nullptr);
    SDL_BlitSurface(decor, nullptr, screen, nullptr);
    npcm->draw_npc(screen, static_cast<int>(loc_x), static_cast<int>(loc_y));
    SDL_BlitSurface(env, nullptr, screen, nullptr);
    txtm->draw_texts(screen, static_cast<int>(loc_x), static_cast<int>(loc_y));

    char charloc[64];
    snprintf(charloc, sizeof(charloc), "Loc : %u,%u Zone %u FPS : %.0f", loc_x, loc_y, zone, fps);
    SDL_Surface *txt_loc = fm->get_text(charloc, 0x00FFFFFF);
    SDL_BlitSurface(txt_loc, nullptr, screen, nullptr);
    SDL_DestroySurface(txt_loc);

    if (disp_infos) {
        SDL_Surface *srf_infos = fm->get_text(str_infos);
        SDL_Rect txtpos{60, 20, static_cast<int>(srf_infos->w), static_cast<int>(srf_infos->h)};
        TnC_FillArgb(screen, &txtpos, 0x758F75AA);
        SDL_BlitSurface(srf_infos, nullptr, screen, &txtpos);
        SDL_DestroySurface(srf_infos);
    }

    presenter.present(screen, SCREEN_W, SCREEN_H);
    fps = 1000.f / static_cast<float>(TnC_GetTicksMs() - tmptick + 1);
}

static SDL_Surface *make_layer(int w, int h, bool /*with_alpha*/) {
    return TnC_CreateRgbaSurface(w, h);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <x> <y> <zone>\n", argv[0]);
        fprintf(stderr, "  Ex: %s 2880 1083 0  (Lighthaven, worldmap)\n", argv[0]);
        return 1;
    }

    unsigned int loc_x = static_cast<unsigned int>(atoi(argv[1]));
    unsigned int loc_y = static_cast<unsigned int>(atoi(argv[2]));
    const unsigned short zone = static_cast<unsigned short>(atoi(argv[3]));

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("TnC Map (SDL3)", SCREEN_W, SCREEN_H, SDL_WINDOW_RESIZABLE);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer || !presenter.init(window, renderer)) {
        fprintf(stderr, "Renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderLogicalPresentation(renderer, SCREEN_W, SCREEN_H, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    screen = make_layer(SCREEN_W, SCREEN_H, false);
    sol = make_layer(SCREEN_W, SCREEN_H, false);
    decor = make_layer(SCREEN_W, SCREEN_H, true);
    env = make_layer(SCREEN_W, SCREEN_H, false);

    fm = new FontManager();
    if (!fm->load_font(dup_cstr("./fonts/font_trebuchet_12"))) {
        fprintf(stderr, "FontManager: police introuvable (./fonts/)\n");
        return 1;
    }

    fm2 = new FontManager();
    fm2->load_font(dup_cstr("./fonts/sans_bold_12"));

    vsfi = new VSFInterface(dup_cstr(PATH_SPRITES));
    mapi = new MAPInterface(dup_cstr(PATH_MAPS), vsfi);

    txtm = new TextManager(fm2);
    txtm->add_color_text_at_pos(dup_cstr("[ Zone SDL3 ]"), 380, 300, 5, 0xBBCC6699);

    npcm = new NPCManager(dup_cstr("NPCList.txt"), vsfi);
    npcm->ajout_npc(0, dup_cstr("CentaurWarrior"), loc_x, loc_y, 135);
    npcm->set_action(0, 'D');
    npcm->move_to(0, loc_x - 5, loc_y - 5, 15);

    SDL_Surface *torche = IMG_Load("torche.png");
    if (torche) {
        SDL_BlitSurface(torche, nullptr, env, nullptr);
        SDL_DestroySurface(torche);
    }
    TnC_SetSurfaceAlpha(env, 0);

    snprintf(str_infos, sizeof(str_infos), "TnC SDL3 test — F1 debug | F12 screenshot | Esc quit");

    map_flag = true;
    disp_infos = false;

    bool running = true;
    while (running) {
        SDL_Delay(5);
        redraw(zone, loc_x, loc_y);

        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_QUIT) {
                running = false;
                break;
            }
            if (ev.type != SDL_EVENT_KEY_DOWN || !ev.key.down) {
                continue;
            }
            switch (ev.key.key) {
            case SDLK_ESCAPE:
                running = false;
                break;
            case SDLK_F1:
                mapi->set_debug(true);
                map_flag = true;
                break;
            case SDLK_F2:
                mapi->set_debug(false);
                map_flag = true;
                break;
            case SDLK_F3:
                disp_infos = !disp_infos;
                break;
            case SDLK_F12:
                SDL_SaveBMP(screen, "screen_sdl3.bmp");
                break;
            case SDLK_LEFT:
            case SDLK_KP_4:
                if (loc_x > 0) {
                    loc_x--;
                }
                map_flag = true;
                break;
            case SDLK_RIGHT:
            case SDLK_KP_6:
                loc_x++;
                map_flag = true;
                break;
            case SDLK_UP:
            case SDLK_KP_8:
                if (loc_y > 0) {
                    loc_y--;
                }
                map_flag = true;
                break;
            case SDLK_DOWN:
            case SDLK_KP_2:
                loc_y++;
                map_flag = true;
                break;
            default:
                break;
            }
        }
    }

    presenter.shutdown();
    SDL_DestroySurface(screen);
    SDL_DestroySurface(sol);
    SDL_DestroySurface(decor);
    SDL_DestroySurface(env);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
