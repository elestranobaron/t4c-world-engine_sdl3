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

#include "npcmanager.h" // class's header file

// class constructor
NPCManager::NPCManager(char *npc_list, VSFInterface *vsfi)
{
  //fm = new FontManager();
  //fm->load_font("./fonts/sans_bold_12");

  this->vsfi = vsfi;
  this->liste_npc = NULL;
  nb_npc_def = 0;

  //Lecture du fichier d'index des sprites.
  FILE *fid;
  fid = fopen(npc_list, "r");
  fscanf(fid, "%hu\n", &nb_npc_def);

  this->liste_npc_def = new struct _npc_def[nb_npc_def];
  int pos = 0;
  char tmp[255];
  char *sep;
  while ((!feof(fid))&&(pos < nb_npc_def))
  {
    fgets(tmp, 255, fid);
    liste_npc_def[pos].nom = new char[strlen(tmp)+1];
    tmp[strlen(tmp)-1] = '\0';
    strcpy(liste_npc_def[pos].nom, tmp);

    fgets(tmp, 255, fid);
    sep = strchr(tmp, '#');
    *sep = '\0';
    sep++;
    liste_npc_def[pos].deplacement = new char[strlen(tmp)+1];
    strcpy(liste_npc_def[pos].deplacement, tmp);
    liste_npc_def[pos].nb_deplacement = sep[0];

    fgets(tmp, 255, fid);
    sep = strchr(tmp, '#');
    *sep = '\0';
    sep++;
    liste_npc_def[pos].attaque = new char[strlen(tmp)+1];
    strcpy(liste_npc_def[pos].attaque, tmp);
    liste_npc_def[pos].nb_attaque = sep[0];    
    
    fgets(tmp, 255, fid);
    sep = strchr(tmp, '#');
    *sep = '\0';
    sep++;
    liste_npc_def[pos].mort = new char[strlen(tmp)+1];
    strcpy(liste_npc_def[pos].mort, tmp);
    liste_npc_def[pos].nb_mort = sep[0];  
    
    fgets(tmp, 255, fid);  

    
//printf("> [%02d] NPC : %20s  | Deplacement: %20s [a->%c] | Attaque: %20s [a->%c] | Mort: %20s [a->%c]\n",pos, liste_npc_def[pos].nom, liste_npc_def[pos].deplacement, liste_npc_def[pos].nb_deplacement, liste_npc_def[pos].attaque, liste_npc_def[pos].nb_attaque, liste_npc_def[pos].mort, liste_npc_def[pos].nb_mort);
    pos++;
  }
}

// class destructor
NPCManager::~NPCManager()
{
	// insert your code here
}



// No description
void NPCManager::dump_npc()
{
   struct _npc *npc = liste_npc;
   while(npc)
   {
     printf("[%03d] (%04d, %04d)  %s (%d)\n", npc->id, npc->x, npc->y, npc->type->nom, npc->direction);
     npc = npc->next;
   }  
}






// No description
bool
NPCManager::move_to(int id, unsigned int dest_x, unsigned int dest_y, unsigned short speed,
                    unsigned short steps_mul)
{
  struct _npc *npc = liste_npc;
  while (npc&&(npc->id != id))
    npc = npc->next;
  if(!npc) return false;

  if (steps_mul < 1) {
    steps_mul = 1;
  }
  const unsigned short denom = static_cast<unsigned short>(2 * speed * steps_mul);
  npc->depl_x = - (float)((npc->x*32 - dest_x*32)/denom);
  npc->depl_y = - (float)((npc->y*16 - dest_y*16)/denom);
  npc->duree_depl  = (short) (sqrt((npc->x - dest_x)*(npc->x - dest_x) + (npc->y-dest_y)*(npc->y-dest_y))*32*steps_mul/speed);
  npc->tps_depl  = 0;
    return true;
}

bool NPCManager::is_moving(const int id) {
  struct _npc *npc = liste_npc;
  while (npc && (npc->id != id)) {
    npc = npc->next;
  }
  if (!npc) {
    return false;
  }
  return npc->duree_depl > npc->tps_depl;
}


bool NPCManager::set_world_pos(int id, int x, int y)
{
  struct _npc *npc = liste_npc;
  while (npc && (npc->id != id))
    npc = npc->next;
  if (!npc)
    return false;
  npc->x = x;
  npc->y = y;
  npc->depl_x = 0.f;
  npc->depl_y = 0.f;
  npc->duree_depl = 0;
  npc->tps_depl = 0;
  return true;
}

bool NPCManager::set_npc_type(int id, const char *nom)
{
  if (!nom)
    return false;

  struct _npc_def *def = NULL;
  for (int i = 0; i < nb_npc_def; i++) {
    if (strchr(liste_npc_def[i].nom, '\r'))
      liste_npc_def[i].nom[strlen(liste_npc_def[i].nom) - 1] = 0;
    if (!strcmp(liste_npc_def[i].nom, nom)) {
      def = &liste_npc_def[i];
      break;
    }
  }
  if (!def)
    return false;

  struct _npc *npc = liste_npc;
  while (npc && (npc->id != id))
    npc = npc->next;
  if (!npc)
    return false;

  if (npc->type == def)
    return true;

  npc->type = def;
  npc->frame = 'a';
  npc->action = 'S';
  return true;
}

bool NPCManager::remove_npc(const int id)
{
  if (id == 0) {
    return false;
  }

  struct _npc **prev = &liste_npc;
  struct _npc *npc = liste_npc;
  while (npc) {
    if (npc->id == id) {
      *prev = npc->next;
      free(npc);
      return true;
    }
    prev = &npc->next;
    npc = npc->next;
  }
  return false;
}

