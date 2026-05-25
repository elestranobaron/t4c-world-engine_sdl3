/***********************************************************************
* This file is a part of TnC The Next Coming (The 4th Coming rebuild with SDL)
* Copyright (C) 2005  Noth
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* aint with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "npcmanager.h"
#include "../MapInterface/mapinterface.h"
#include "tnc_sdl3.h"
// No description
bool NPCManager::draw_npc(SDL_Surface *surf, int x, int y)
{
  struct _npc *npc = liste_npc;
  short angle = 0;
  bool m_flag;

  while (npc)
  {
     angle = npc->direction;
     m_flag = false;
     if (angle == 225) {m_flag = true; angle=135;}
     if (angle == 270) {m_flag = true; angle=90;}
     if (angle == 315) {m_flag = true; angle=45;}
     
     
     
    //génération du nom du sprite
    char nom[255];
    if       (npc->action == 'D') //Deplacement
    {
      if (TnC_GetTicksMs() - npc->timer > 1000/ANIM_FPS)
      {
         npc->timer = TnC_GetTicksMs();
         npc->frame ++;
      }  
      if (npc->frame > npc->type->nb_deplacement) npc->frame = 'a';
      sprintf(nom, "%s%03d-%c", npc->type->deplacement, angle, npc->frame);
    }  
    else if  (npc->action == 'M') //mort
    {
      if (TnC_GetTicksMs() - npc->timer > 1000/ANIM_FPS)
      {
         npc->timer = TnC_GetTicksMs();
         if (npc->frame < npc->type->nb_mort) npc->frame ++;
      }  

      sprintf(nom, "%s-%c", npc->type->mort, npc->frame);
    }  
    else if  (npc->action == 'A') //attaque
    {
      if (TnC_GetTicksMs() - npc->timer > 1000/ANIM_FPS)
      {
         npc->timer = TnC_GetTicksMs();
         npc->frame ++;
      }  
      if (npc->frame > npc->type->nb_attaque) npc->frame = 'a';
      sprintf(nom, "%s%03d-%c", npc->type->attaque, angle, npc->frame);
    }
    else
    {
      npc->frame = 'a';
      sprintf(nom, "%s%03d-%c", npc->type->deplacement, angle, npc->frame);
    }  
    
    
    //recherche du sprite
    struct _sprite *spr;
    if (!m_flag)
        spr = vsfi->get_sprite_by_name(nom);
    else
        spr = vsfi->get_mirrored_sprite_by_name(nom);

    //gestion du déplacement
    int bonus_x = 0;
    int bonus_y = 0;
    if (npc->duree_depl > npc->tps_depl)
    {
      npc->tps_depl ++;
      bonus_x = (int) npc->depl_x * npc->tps_depl;
      bonus_y = (int) npc->depl_y * npc->tps_depl;
      
      if (npc->duree_depl <= npc->tps_depl)
      {
       npc->x += (int) (bonus_x/32);
       npc->y += (int) (bonus_y/16);
      }  
    }

    //affichage du sprite
    if (spr&&(!m_flag))
    {    
      SDL_Rect pos;
      pos.x = (npc->x - x) * 32 + SCREEN_W / 2 + spr->offset_x + bonus_x;
      pos.y = (npc->y - y) * 16 + SCREEN_H / 2 + spr->offset_y + bonus_y;
      SDL_BlitSurface(spr->sdl_sprite, NULL, surf, &pos);
/*      
      SDL_Rect txtpos;
      txtpos.x = (npc->x - x)*32+800/2;
      txtpos.y = (npc->y - y)*16 + 600/2- spr->sdl_sprite->h;
      SDL_Surface *txt = fm->get_text(nom, 0xFB9F0066);
      SDL_BlitSurface(txt, NULL, surf, &txtpos);
      SDL_FreeSurface(txt);*/
    }  
    else if (spr&&m_flag)
    {    
      SDL_Rect pos;
      pos.x = (npc->x - x) * 32 + SCREEN_W / 2 + spr->offset_mx + bonus_x;
      pos.y = (npc->y - y) * 16 + SCREEN_H / 2 + spr->offset_my + bonus_y;
      SDL_BlitSurface(spr->sdl_sprite_miroir, NULL, surf, &pos);
      /*
      SDL_Rect txtpos;
      txtpos.x = (npc->x - x)*32+800/2;
      txtpos.y = (npc->y - y)*16 + 600/2 - spr->sdl_sprite->h;
      SDL_Surface *txt = fm->get_text(nom, 0xFB9F0066);
      SDL_BlitSurface(txt, NULL, surf, &txtpos);
      SDL_FreeSurface(txt);*/
    }  
    else
      printf("NPCManager > Echec d'affichage pour %s\n", nom);

    

    npc = npc->next;
  } 
  return true; 
}
