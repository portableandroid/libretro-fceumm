#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../fceu.h"
#include "../../fceu-types.h"
#include "../../vsuni.h"
#include "../../git.h"
#include "../../driver.h"
#include "../../cart.h"

#include "libretro_dipswitch.h"

#define MAX_CORE_OPTIONS 8
#define MAX_VALUES 10

extern CartInfo iNESCart;

typedef struct {
   const char  *name;
   uint8       value;
} SETTING;

typedef struct {
   const char  *option_name;
   SETTING     settings[MAX_VALUES];
} DIPSWITCH;

typedef struct {
   unsigned    game_id;
   const char  *game_name;
   DIPSWITCH   core_options[MAX_CORE_OPTIONS];
} VSUNIGAME;

static VSUNIGAME dipswitch_battlecity = {
   VS_BATTLECITY, "battlecity",
   {
      {  "Credits for 2 Players",
         {
            { "2", 0x01 },
            { "1", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Lives",
         {
            { "5", 0x02 },
            { "3", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Demo Sounds",
         {
            { "disabled", 0x04 },
            { "enabled",  0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   },
};

static VSUNIGAME dipswitch_castlevania = {
   VS_CASTLEVANIA, "castlevania",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives",
         {
            { "3", 0x00 },
            { "2", 0x08 },
            { NULL, 0 },
         }
      },
      {  "Bonus",
         {
            { "100k", 0x00 },
            { "200k", 0x20 },
            { "300k", 0x10 },
            { "400k", 0x30 },
            { NULL, 0 },
         },
      },
      {  "Difficulty",
         {
            { "Normal", 0x00 },
            { "Hard",   0x40 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_clucluland = {
   VS_CLUCLULAND, "clucluland",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives",
         {
            { "3", 0x00 },
            { "4", 0x40 },
            { "5", 0x20 },
            { "2", 0x60 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_drmario = {
   VS_DRMARIO, "drmario",
   {
      {  "Drop Rate Increase After",
         {
            { "7 Pills",  0x00 },
            { "8 Pills",  0x01 },
            { "9 Pills",  0x02 },
            { "10 Pills", 0x03 },
            { NULL, 0 },
         },
      },
      {  "Virus Level",
         {
            { "1", 0x00 },
            { "3", 0x04 },
            { "5", 0x08 },
            { "7", 0x0c },
            { NULL, 0 },
         },
      },
      {  "Drop Speed Up",
         {
            { "Slow",    0x00 },
            { "Medium",  0x10 },
            { "Fast",    0x20 },
            { "Fastest", 0x30 },
            { NULL, 0 },
         },
      },
      {  "Free Play",
         {
            { "disabled", 0x00 },
            { "enabled",  0x40 },
            { NULL, 0 },
         },
      },
      {  "Demo Sounds",
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_duckhunt = {
   VS_DUCKHUNT, "duckhunt",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Difficulty",
         {
            { "Normal", 0x08 },
            { "Medium", 0x10 },
            { "Hard",   0x18 },
            { "Easy",   0x00 },
            { NULL, 0 },
         },
      },
      {  "Miss per game",
         {
            { "5", 0x20 },
            { "3", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Bonus Life",
         {
            { "30000",  0x00 },
            { "50000",  0x40 },
            { "80000",  0x80 },
            { "100000", 0xc0 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_excitebike = {
   VS_EXITEBIKE, "excitebike",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Bonus Bike",
         {
            { "100k and Every 50k", 0x00 },
            { "100k Only",          0x08 },
            { "Every 100k",         0x10 },
            { "None",               0x18 },
            { NULL, 0 },
         },
      },
      {  "1st Qualifying Time",
         {
            { "Easy", 0x00 },
            { "Hard", 0x20 },
            { NULL, 0 },
         },
      },
      {  "2nd Qualifying Time",
         {
            { "Easy", 0x00 },
            { "Hard", 0x40 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_freedomforce = {
   VS_FREEDOMFORCE, "freedomforce",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Health Awarded At",
         {
            { "10k 50k",           0x10 },
            { "20k 60k",           0x00 },
            { "20k 60k Every 60k", 0x08 },
            { "10k 50k Every 50k", 0x18 },
            { NULL, 0 },
         },
      },
      {  "Difficulty (Damage)",
         {
            { "Normal",  0x00 },
            { "Easy",    0x40 },
            { "Hard",    0x20 },
            { "Hardest", 0x60 },
            { NULL, 0 },
         },
      },
      {  "Difficulty (Enemy)",
         {
            { "Normal", 0x80 },
            { "Hard",   0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_goonies = {
   VS_GOONIES, "goonies",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives",
         {
            { "3", 0x08 },
            { "4", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Bonus",
         {
            { "40000", 0x00 },
            { "50000", 0x20 },
            { "60000", 0x10 },
            { "70000", 0x30 },
            { NULL, 0 },
         },
      },
      {  "Timer",
         {
            { "Slow", 0x00 },
            { "Fast", 0x40 },
            { NULL, 0 },
         },
      },
      { "Demo Sounds",
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_gradius = {
   VS_GRADIUS, "gradius",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives",
         {
            { "3", 0x08 },
            { "4", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Bonus",
         {
            { "100k", 0x00 },
            { "200k", 0x20 },
            { "300k", 0x10 },
            { "400k", 0x30 },
            { NULL, 0 },
         },
      },
      {  "Difficulty",
         {
            { "Normal", 0x00 },
            { "Hard",   0x40 },
            { NULL, 0 },
         },
      },
      { "Demo Sounds",
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_gumshoe = {
   VS_GUMSHOE, "gumshoe",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Difficulty",
         {
            { "Normal",  0x08 },
            { "Easy",    0x00 },
            { "Hard",    0x10 },
            { "Hardest", 0x18 },
            { NULL, 0 },
         },
      },
      {  "Lives",
         {
            { "3", 0x20 },
            { "5", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Bullets per Balloon",
         {
            { "3", 0x00 },
            { "2", 0x40 },
            { NULL, 0 },
         },
      },
      {  "Bonus Man Awarded",
         {
            { "At 50000", 0x00 },
            { "None",     0x80 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_hogansalley = {
   VS_HOGANSALLEY, "hogansalley",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Difficulty",
         {
            { "Normal",  0x08 },
            { "Easy",    0x00 },
            { "Hard",    0x10 },
            { "Hardest", 0x18 },
            { NULL, 0 },
         },
      },
      {  "Misses Per Game",
         {
            { "5", 0x20 },
            { "3", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Bonus Life",
         {
            { "30000",  0x00 },
            { "50000",  0x40 },
            { "80000",  0x80 },
            { "100000", 0xc0 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_iceclimber = {
   VS_ICECLIMBER, "iceclimber",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives",
         {
            { "3", 0x00 },
            { "4", 0x10 },
            { "5", 0x08 },
            { "7", 0x18 },
            { NULL, 0 },
         },
      },
      {  "Difficulty",
         {
            { "Easy", 0x00 },
            { "Hard", 0x20 },
            { NULL, 0 },
         },
      },
      {  "Time before bear appears",
         {
            { "Long",  0x00 },
            { "Short", 0x40 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_jajamaru = {
   VS_JAJAMARU, "jajamaru",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives",
         {
            { "3", 0x00 },
            { "4", 0x10 },
            { "5", 0x08 },
            { NULL, 0 },
         },
      },
      {  "Demo Sounds",
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_machrider = {
   VS_MACHRIDER, "machrider",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Time",
         {
            { "280 (Easy)", 0x00 },
            { "250",        0x10 },
            { "220",        0x08 },
            { "200 (Hard)", 0x18 },
            { NULL, 0 },
         },
      },
      {  "Enemies",
         {
            { "Less", 0x00 },
            { "More", 0x40 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_mightybj = {
   VS_MIGHTYBOMBJACK, "mightybj",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "5 Coins 1 Credit", 0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives",
         {
            { "3", 0x00 },
            { "4", 0x08 },
            { "5", 0x18 },
            { "2", 0x10 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_pinball = {
   VS_PINBALL, "pinball",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x01 },
            { "1 Coin 2 Credits", 0x06 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x04 },
            { "2 Coins 1 Credit", 0x05 },
            { "3 Coins 1 Credit", 0x03 },
            { "4 Coins 1 Credit", 0x07 },
            { "Free Play",        0x00 },
            { NULL, 0 },
         },
      },
      {  "Side Drain Walls",
         {
            { "High", 0x00 },
            { "Low",  0x08 },
            { NULL, 0 },
         },
      },
      {  "Bonus Life",
         {
            { "50000", 0x00 },
            { "70000", 0x10 },
            { NULL, 0 },
         },
      },
      {  "Balls",
         {
            { "3", 0x00 },
            { "4", 0x40 },
            { "5", 0x20 },
            { "2", 0x60 },
            { NULL, 0 },
         }
      },
      {  "Ball Speed",
         {
            { "Slow", 0x00 },
            { "Fast", 0x80 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_platoon = {
   VS_PLATOON, "platoon",
   {
      {  "Demo Sounds",
         {
            { "enabled",  0x00 },
            { "disabled", 0x04 },
            { NULL, 0 },
         },
      },
      {  "Difficulty",
         {
            { "Normal", 0x08 },
            { "Medium", 0x10 },
            { "Hard",   0x18 },
            { "Easy",   0x00 },
            { NULL, 0 },
         }
      },
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x20 },
            { "1 Coin 3 Credits", 0x40 },
            { "2 Coins 1 Credit", 0x60 },
            { "3 Coins 1 Credit", 0x80 },
            { "4 Coins 1 Credit", 0xa0 },
            { "5 Coins 1 Credit", 0xc0 },
            { "Free Play",  0xe0 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_rbibaseball = {
   VS_RBIBASEBALL, "atari_rbibb",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x01 },
            { "2 Coins 1 Credit", 0x02 },
            { "3 Coins 1 Credit", 0x03 },
            { NULL, 0 },
         },
      },
      {  "1p-inn 2p-inn time-min",
         {
            { "2 2 4", 0x0c },
            { "3 2 6", 0x00 },
            { "4 3 7", 0x08 },
            { "2 1 3", 0x04 },
            { NULL, 0 },
         }
      },
      {  "Demo Sounds",
         {
            { "enabled",  0x00 },
            { "disabled", 0x10 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_slalom = {
   VS_SLALOM, "slalom",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Freestyle Points",
         {
            { "Hold Time",  0x08 },
            { "Left Right", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Difficulty",
         {
            { "Normal", 0x10 },
            { "Medium", 0x20 },
            { "Hard",   0x30 },
            { "Easy",   0x00 },
            { NULL, 0 },
         },
      },
      {  "Allow Continue",
         {
            { "enabled",  0x00 },
            { "disabled", 0x40 },
            { NULL, 0 },
         },
      },
      {  "Inverted Input",
         {
            { "disabled", 0x00 },
            { "enabled",  0x80 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_smb = {
   VS_SMB, "smb",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x06 },
            { "1 Coin 3 Credits", 0x01 },
            { "1 Coin 4 Credits", 0x05 },
            { "1 Coin 5 Credits", 0x03 },
            { "2 Coins 1 Credit", 0x04 },
            { "3 Coins 1 Credit", 0x02 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives",
         {
            { "3", 0x00 },
            { "2", 0x08 },
            { NULL, 0 },
         },
      },
      {  "Bonus Life",
         {
            { "100 coins", 0x00 },
            { "150 coins", 0x20 },
            { "200 coins", 0x10 },
            { "250 coins", 0x30 },
            { NULL, 0 },
         },
      },
      {  "Timer",
         {
            { "Slow", 0x00 },
            { "Fast", 0x40 },
            { NULL, 0 },
         },
      },
      {  "Continue Lives",
         {
            { "3", 0x80 },
            { "4", 0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_soccer = {
   VS_SOCCER, "soccer",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Points Timer",
         {
            { "1000 Pts", 0x08 },
            { "1200 Pts", 0x18 },
            { "600 Pts",  0x00 },
            { "800 Pts",  0x10 },
            { NULL, 0 },
         },
      },
      { "Difficulty",
         {
            { "Normal", 0x40 },
            { "Medium", 0x20 },
            { "Hard",   0x60 },
            { "Easy",   0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_starluster = {
   VS_STARLUSTER, "starluster",
   {
      { "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x03 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_superskykid = {
   VS_SUPERSKYKID, "superskykid",
   {
      {  "Difficulty",
         {

            { "Normal", 0x01 },
            { "Medium", 0x02 },
            { "Hard",   0x03 },
            { "Easy",   0x00 },
            { NULL, 0 },
         }
      },
      {  "Lives",
         {
            { "3", 0x04 },
            { "2", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x08 },
            { "2 Coins 1 Credit", 0x10 },
            { "3 Coins 1 Credit", 0x18 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_superxevious = {
   VS_SUPERXEVIOUS, "superxevious",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x10 },
            { "2 Coins 1 Credit", 0x20 },
            { "3 Coins 1 Credit", 0x30 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_tetris = {
   VS_TETRIS, "tetris",
   {
      {  "Difficulty",
         {
            { "Normal", 0x04 },
            { "Medium", 0x08 },
            { "Hard",   0x0c },
            { "Easy",   0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_tkoboxing = {
   VS_TKOBOXING, "tkoboxing",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x01 },
            { "2 Coins 1 Credit", 0x02 },
            { "3 Coins 1 Credit", 0x03 },
            { NULL, 0 },
         },
      },
      {  "Difficulty",
         {
            { "Normal",    0x04 },
            { "Hard",      0x08 },
            { "Very Hard", 0x0c },
            { "Easy",      0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

static VSUNIGAME dipswitch_topgun = {
   VS_TOPGUN, "topgun",
   {
      {  "Coinage",
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x06 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",  0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives per Coin",
         {
            { "3-12 Max", 0x00 },
            { "2-9 Max",  0x08 },
            { NULL, 0 },
         },
      },
      {  "Bonus Life",
         {
            { "30k and Every 50k",   0x00 },
            { "50k and Every 100k",  0x20 },
            { "100k and Every 150k", 0x10 },
            { "200k and Every 200k", 0x30 },
            { NULL, 0 },
         },
      },
      {  "Difficulty",
         {
            { "Easy", 0x00 },
            { "Hard", 0x40 },
            { NULL, 0 },
         },
      },
      {  "Demo Sounds",
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, { {0} } },
   }
};

enum {
   DPSW_NONE = 0,
   DPSW_VSUNI,
   DPSW_NWC,
};

static struct retro_core_option_definition option_defs_empty = {
   NULL, NULL, NULL, { {0} }, NULL
};

static struct retro_core_option_definition vscoreopt[MAX_CORE_OPTIONS];
static VSUNIGAME *vsgame = NULL;

static char *core_key[MAX_CORE_OPTIONS];
static unsigned dipswitch_type = DPSW_NONE;
static unsigned numCoreOptions = 0;
static unsigned numValues[MAX_VALUES] = {0};
static uint8 dipswitchPreset = 0;

static const char *str_to_corekey(char *s)
{
   char *str = s;
   unsigned i = 0;

   while (str[i] != '\0')
   {
      if (str[i] == ' ')
         str[i] = '_';
      i++;
   }
   str[i] = '\0';

   return str;
}

static void make_core_options(struct retro_core_option_definition *vs_core_options)
{
   unsigned i, j;

   for (i = 0; i < numCoreOptions; i++)
   {
      struct retro_core_option_value vars_empty = { NULL, NULL };
      const char *game_name = vsgame->game_name;
      const char *option_name = vsgame->core_options[i].option_name;
      char key[100] = {0};

      /* Set core key and sanitize string */
      sprintf(key, "fceumm_dipswitch_%s-%s", game_name, option_name);
      core_key[i] = calloc(strlen(key) + 1, sizeof(char));
      strcpy(core_key[i], key);
      vs_core_options[i].key = str_to_corekey(core_key[i]);

      /* Set desc */
      vs_core_options[i].desc = option_name;

      /* Set info */
      vs_core_options[i].info = NULL;

      /* Set core values */
      for (j = 0; j < numValues[i]; j++) {
         const char *var_value = vsgame->core_options[i].settings[j].name;
         vs_core_options[i].values[j].value = var_value;
      }

      vs_core_options[i].values[j] = vars_empty;

      /* Set default value. Top entry used as default */
      vs_core_options[i].default_value = vsgame->core_options[i].settings[0].name;
   }
}

static VSUNIGAME *get_vsuni_dipswitch(unsigned id)
{
   switch (id) {
   case VS_BATTLECITY:
      return (&dipswitch_battlecity);
   case VS_CASTLEVANIA:
      return (&dipswitch_castlevania);
   case VS_CLUCLULAND:
      return (&dipswitch_clucluland);
   case VS_DRMARIO:
      return (&dipswitch_drmario);
   case VS_DUCKHUNT:
      return (&dipswitch_duckhunt);
   case VS_EXITEBIKE:
      return (&dipswitch_excitebike);
   case VS_FREEDOMFORCE:
      return (&dipswitch_freedomforce);
   case VS_GOONIES:
      return (&dipswitch_goonies);
   case VS_GRADIUS:
      return (&dipswitch_gradius);
   case VS_GUMSHOE:
      return (&dipswitch_gumshoe);
   case VS_HOGANSALLEY:
      return (&dipswitch_hogansalley);
   case VS_ICECLIMBER:
      return (&dipswitch_iceclimber);
   case VS_JAJAMARU:
      return (&dipswitch_jajamaru);
   /* case VS_LADIESGOLF: */
   /* case VS_STROKEANDMATCHGOLF: */
   case VS_MACHRIDER:
      return (&dipswitch_machrider);
   case VS_MIGHTYBOMBJACK:
      return (&dipswitch_mightybj);
   case VS_PINBALL:
      return (&dipswitch_pinball);
   case VS_PLATOON:
      return (&dipswitch_platoon);
   case VS_RBIBASEBALL:
      return (&dipswitch_rbibaseball);
   case VS_SLALOM:
      return (&dipswitch_slalom);
   case VS_SMB:
      return (&dipswitch_smb);
   case VS_SOCCER:
      return (&dipswitch_soccer);
   case VS_STARLUSTER:
      return (&dipswitch_starluster);
   case VS_SUPERSKYKID:
      return (&dipswitch_superskykid);
   case VS_SUPERXEVIOUS:
      return (&dipswitch_superxevious);
   case VS_TETRIS:
      return (&dipswitch_tetris);
   case VS_TKOBOXING:
      return (&dipswitch_tkoboxing);
   case VS_TOPGUN:
      return (&dipswitch_topgun);
   }

   FCEU_printf("No dipsettings found. %d\n", id);
   return (NULL);
}

static void update_dipswitch_vsuni(void)
{
   unsigned index_key;
   uint8 vsdip_new = 0;
   uint8 last_vsdip = FCEUI_VSUniGetDIPs();

   for (index_key = 0; index_key < numCoreOptions; index_key++)
   {
      struct retro_variable var = { NULL, NULL };
      const char *key = vscoreopt[index_key].key;
      unsigned index_value;

      var.key = key;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) == 0)
         continue;

      for (index_value = 0; index_value < numValues[index_key]; index_value++)
      {
         const char *var_value = vscoreopt[index_key].values[index_value].value;
         uint8 value = 0;

         if (strcmp(var.value, var_value) != 0)
            continue;

         value = vsgame->core_options[index_key].settings[index_value].value;
         vsdip_new |= value;
      }
   }

   vsdip_new |= dipswitchPreset; /* Append any default preset to new value */
   if (last_vsdip != vsdip_new)
   {
      vsdip = vsdip_new;
#ifdef DEBUG
      FCEU_printf("Dipswitch changed %d%d%d%d%d%d%d%d\n",
         (vsdip >> 0) & 1, (vsdip >> 1) & 1, (vsdip >> 2) & 1, (vsdip >> 3) & 1,
         (vsdip >> 4) & 1, (vsdip >> 5) & 1, (vsdip >> 6) & 1, (vsdip >> 7) & 1);
#endif
   }
}

/* Nintendo World Championship */
static struct retro_core_option_definition dipswitch_nwc[] = {
   {
      "fceumm_dipswitch_nwc_swa",
      "Dipswitch SW1 (+18.7secs)",
      "Adds 18.7 seconds to total time. Total time = 5 mins + enabled dipswitches.",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL},
      },
      "disabled",
   },
   {
      "fceumm_dipswitch_nwc_swb",
      "Dipswitch SW1 (+37.5s)",
      "Adds 37.5 seconds to total time. Total time = 5 mins + enabled dipswitches.",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL},
      },
      "disabled",
   },
   {
      "fceumm_dipswitch_nwc_swc",
      "Dipswitch SW1 (+1m 15s)",
      "Adds 1 minute and 15 seconds to total time. Total time = 5 mins + enabled dipswitches.",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL},
      },
      "enabled",
   },
   {
      "fceumm_dipswitch_nwc_swd",
      "Dipswitch SW1 (+2m 30s)",
      "Adds 2 minutes and 30 seconds to total time. Total time = 5 mins + time of enabled dipswitches.",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL},
      },
      "disabled",
   },

   { NULL, NULL, NULL, { {0} }, NULL },
};

static void update_dipswitch_nwc(void)
{
   unsigned i, dips = 0x00;

   for (i = 0; i < 4; i++)
   {
      struct retro_variable var = { NULL, NULL };
      const char *key = dipswitch_nwc[i].key;

      var.key = key;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) == 0)
         continue;
      if (strcmp(var.value, "enabled") != 0)
         continue;
      dips |= (1 << i);
   }

   if (GameInfo->cspecial != dips)
   {
      GameInfo->cspecial = dips;
#ifdef DEBUG
      FCEU_printf("Dipswitch changed = %d%d%d%d\n", (dips >> 0) & 1,
         (dips >> 1) & 1, (dips >> 2) & 1, (dips >> 3) & 1);
#endif
   }
}

size_t set_dipswitch_variables(unsigned current_index, struct retro_core_option_definition *vars)
{
   unsigned index = current_index;
   int dipsw_size = 0;

   /* VSUNI Dipswitch */
   if (GameInfo->type == GIT_VSUNI)
   {
      dipswitchPreset = FCEUI_VSUniGetDIPs();
      vsgame = get_vsuni_dipswitch(GameInfo->gameid);
      if (vsgame)
      {
         unsigned i, j;

         numCoreOptions = 0;
         dipswitch_type = DPSW_VSUNI;

         /* Initialize dipswitch struct with empty values */
         for (i = 0; i < MAX_CORE_OPTIONS; i++)
            vscoreopt[i] = option_defs_empty;

         /* Count number of core options and the number of values for each */
         for (i = 0; i < MAX_CORE_OPTIONS; i++) {
            if (vsgame->core_options[i].option_name) {
               for (j = 0; j < MAX_VALUES; j++) {
                  if (vsgame->core_options[i].settings[j].name)
                     numValues[i]++;
               }
               numCoreOptions++;
            }
         }

         make_core_options(vscoreopt);

         /* Append dipswitch struct to main core options struct */
         while (vscoreopt[dipsw_size].key) {
            vars[index] = vscoreopt[dipsw_size];
            index++;
            dipsw_size++;
         }
      }
      return (dipsw_size);
   }

   /* Nintendo World Championship cart (Mapper 105)*/
   if (iNESCart.mapper == 105)
   {
      dipswitch_type = DPSW_NWC;

      while (dipswitch_nwc[dipsw_size].key) {
         vars[index] = dipswitch_nwc[dipsw_size];
         index++;
         dipsw_size++;
      }
      return (dipsw_size);
   }

   dipswitch_type = DPSW_NONE;
   return (0);
}

void update_dipswitch(void)
{
   switch (dipswitch_type) {
   case DPSW_VSUNI:
      update_dipswitch_vsuni();
      break;
   case DPSW_NWC:
      update_dipswitch_nwc();
      break;
   }
}

void DPSW_Cleanup(void)
{
   unsigned i;

   for (i = 0; i < MAX_CORE_OPTIONS; i++) {
      if (core_key[i]) {
         free(core_key[i]);
         core_key[i] = NULL;
      }
   }
}
