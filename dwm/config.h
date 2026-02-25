/* See LICENSE file for copyright and license details. */

/* appearance */
#include <X11/X.h>
#include <X11/XF86keysym.h>
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 40;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const int vertpad            = 20;       /* vertical padding of bar */
static const int sidepad            = 20;       /* horizontal padding of bar */
static const int focusonwheel       = 1;    /* click on focus with mousewheel */

#include "catppuccin.h"

static const char *fonts[]             = { "JetBrainsMono Nerd Font:style=Regular:size=11.5:antialias=true" };

static const char *colors[][3]      = {
	/*                     fg               bg         border   */
  [SchemeNorm]       = { peach,          base,     gray2 },
  [SchemeSel]        = { red,             base,     pink },
  [SchemeTag]        = { gray3,           base,     base },
  [SchemeTag1]       = { yellow,          base,     base },
  [SchemeTag2]       = { red,             base,     base },
  [SchemeTag3]       = { peach,          base,     base },
  [SchemeTag4]       = { green,           base,     base },
  [SchemeTag5]       = { pink,            base,     base },
  [SchemeTag6]       = { blue,            base,     base },
  [SchemeTag7]       = { green,           base,     base },
  // [SchemeTag8]       = { yellow,          base,     base },
  // [SchemeTag9]       = { red,             base,     base },
  [SchemeLayout]     = { yellow,          base,     base },
};                       
                         
/* tagging */
static char *tags[] = {"󰮯", "󰧞", "󰑊", "󰧞", "󰑊", "󰧞", "󰑊"};

static const int tagschemes[] = { SchemeTag1, SchemeTag2, SchemeTag3,
                                  SchemeTag4, SchemeTag5, SchemeTag6, SchemeTag7, 
                                  // SchemeTag8, SchemeTag9 
};

static const unsigned int ulinepad	= 5;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 2;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 4;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
  /*xprop(1):
    WM_CLASS(STRING) = instance, class
    WM_NAME(STRING) = title*/
  // class           instance   title           tags centered floating terminal noswal mon spnum
  { "st-256color",   NULL,      NULL,           0,   0,       0,       1,       0,     -1, -1 },
  { "Alacritty",     NULL,      NULL,           0,   0,       0,       1,       0,     -1, -1 },
  { "Dragon-drop",   NULL,      NULL,           ~0,  1,       1,       0,       1,     -1, -1 },
  { NULL,            NULL,      "Event Tester", 0,   0,       0,       0,       1,     -1, -1 },
  { NULL,            "fcen",    NULL,           0,   1,       1,       0,       1,     -1, -1 },
  { NULL,            "fl",      NULL,           0,   0,       1,       0,       1,     -1, -1 },

  /* scratchpads*/
  { NULL,		         "spterm",	NULL,	          0,	 1,       1,			 1,       0,     -1, 0  },
  { NULL,		         "spfm",	  NULL,	          0,	 1,       1,			 1,       0,     -1, 1  },
  { NULL,		         "spcalcu", NULL,	          0,	 1,       1,			 0,       0,     -1, 2  },
  { NULL,		         "spvol",   NULL,	          0,	 1,       1,			 0,       0,     -1, 3  },
  { NULL,		         "sptop",   NULL,	          0,	 1,       1,			 0,       0,     -1, 4  },
  { NULL,		         "spnote",  NULL,	          0,	 1,       1,			 0,       0,     -1, 5  },
  { "discord",       NULL,      NULL,           0,   1,       1,       0,       0,     -1, 6  },
  { "com.github.th_ch.youtube_music", NULL,      NULL,           0,   1,       1,       0,       0,     -1, 7  },
};

/* layout(s) */
static const float mfact     = 0.45; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
  /* first entry is default */
	{ "|M|",      centeredmaster },
	{ "󰊠",        tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

typedef struct {
  char *rule;
  const void *cmd;
} Sp;

/* st cmds */
/* const char *spterm[] = {"st", "-n", "spterm", "-g", "125x34", NULL };
const char *spfm[] = {"st", "-n", "spfm", "-g", "110x33", "-e", "lfrun", NULL };
const char *spbc[] = {"st", "-n", "spcalcu", "-g", "80x30", "-e", "bc", "-lq", NULL };
const char *sppm[] = {"st", "-n", "spvol", "-g", "100x15", "-e", "pulsemixer", NULL };
const char *sptop[] = {"st", "-n", "sptop", "-g", "115x35", "-e", "btop", NULL };
const char *sporg[] = {"st", "-n", "spnote", "-g", "125x34", "-e", "nvim", "-c :call cursor(4,4)", "-c cd ~/personal/org", "/home/john/personal/org/index.org", NULL }; */

/* alacritty cmds */
const char *spterm[] = {"alacritty", "--class=spterm", "-o", "window.dimensions.columns=125", "window.dimensions.lines=34", NULL };
const char *spfm[] = {"alacritty", "--class=spfm", "-o", "window.dimensions.columns=110", "window.dimensions.lines=33", "-e", "lfrun", NULL };
const char *spbc[] = {"alacritty", "--class=spcalcu", "-o", "window.dimensions.columns=80", "window.dimensions.lines=30", "-e", "bc", "-lq", NULL };
const char *sppm[] = {"alacritty", "--class=spvol", "-o", "window.dimensions.columns=100", "window.dimensions.lines=15", "-e", "pulsemixer", NULL };
const char *sptop[] = {"alacritty", "--class=sptop", "-o", "window.dimensions.columns=115", "window.dimensions.lines=35", "-e", "btop", NULL };
const char *sporg[] = {"alacritty", "--class=spnote", "-o", "window.dimensions.columns=84", "window.dimensions.lines=34", "-e", "nvim", "+3", "-c", "cd ~/personal/org", "/home/john/personal/org/index.org", NULL };

const char *dcmd[] = {"discord", NULL };
const char *ytmcmd[] = {"youtube-music", NULL };

static Sp scratchpads[] = {
  /* name          cmd  */
  {"spterm",       spterm},
  {"splf",         spfm},
  {"spcalcu",      spbc},
  {"spvol",        sppm},
  {"sptop",        sptop},
  {"spnote",       sporg},
  {"dcmd",         dcmd},
  {"ytmcmd",       ytmcmd},
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(CS,CHAIN,KEY,TAG) \
	{ MODKEY,                       CS, CHAIN,    KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           CS, CHAIN,    KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             CS, CHAIN,    KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, CS, CHAIN,    KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* dmenu */
static char ds[3] = "0";
static char dv[3] = "0";
static char ms[12] = "0";
static char dh[2] = "0";
#define DMENUS(NAME,CMD) \
  static const char *NAME[] = { CMD, "-x", ds, "-y", dv, "-z", ms, "-h", dh, /* "-m", dmenumon, */ NULL };

//     cmd name    executable 
DMENUS(dmenucmd,   "dmenu_run")
DMENUS(clipcmd,    "clipmenu")
DMENUS(mountcmd,   "dmenumount")
DMENUS(umountcmd,  "dmenuumount")
DMENUS(wikicmd,    "wikimenu")
DMENUS(scrcmd,     "scrmenu")
DMENUS(powermenu,  "power.sh")
DMENUS(btmenu,     "bluetoothdmenu.sh")

/* commands */
static const char *termcmd[]  = { "alacritty", NULL };

static const ClientSpecific cs[] = {
  /*xprop(1):
    WM_CLASS(STRING) = instance, class
    WM_NAME(STRING) = title*/
  // class                instance 
  { NULL,                 NULL },
  { "Weston Compositor",  NULL },
};

#include "movestack.c"

static const Key keys[] = {
	/* modifier                           client keychain    key        function        argument */
  { MODKEY|ShiftMask,                   0,     -1,         XK_Return, togglesp,       {.v = "spterm" } },
  { MODKEY,                             0,     XK_s,       XK_f,      togglesp,       {.v = "splf" } },
  { MODKEY,                             0,     XK_s,       XK_F9,     togglesp,       {.v = "spcalcu" } },
  { MODKEY|ControlMask,                 0,     -1,         XK_v,      togglesp,       {.v = "spvol" } },
  { MODKEY,                             0,     -1,         XK_g,      togglesp,       {.v = "sptop" } },
  { MODKEY,                             0,     XK_s,       XK_n,      togglesp,       {.v = "spnote" } },
  { MODKEY,                             0,     XK_s,       XK_d,      togglesp,       {.v = "dcmd" } },
  { MODKEY,                             0,     XK_s,       XK_y,      togglesp,       {.v = "ytmcmd" } },

  { MODKEY,                             0,     XK_s,       XK_1,      togglesp,       {.ui = 1} }, 
  { MODKEY,                             0,     XK_s,       XK_2,      togglesp,       {.ui = 2} }, 
  { MODKEY,                             0,     XK_s,       XK_3,      togglesp,       {.ui = 3} }, 
  { MODKEY,                             0,     XK_s,       XK_4,      togglesp,       {.ui = 4} }, 
  { MODKEY,                             0,     XK_s,       XK_5,      togglesp,       {.ui = 5} }, 
  { MODKEY,                             0,     XK_s,       XK_6,      togglesp,       {.ui = 6} }, 
  { MODKEY,                             0,     XK_s,       XK_7,      togglesp,       {.ui = 7} }, 
  { MODKEY,                             0,     XK_s,       XK_8,      togglesp,       {.ui = 8} }, 
  { MODKEY,                             0,     XK_s,       XK_9,      togglesp,       {.ui = 9} }, 
  { MODKEY,                             0,     XK_s,       XK_0,      togglesp,       {.ui = 10} }, 

	{ MODKEY,                             0,     -1,  XK_p,      spawn,          {.v = dmenucmd } },
  { MODKEY,                             0,     -1,  XK_Tab,    spawn,          {.v = clipcmd } },
  { MODKEY|ShiftMask,                   0,     -1,  XK_w,      spawn,          {.v = wikicmd } },
  { MODKEY,                             0,     -1,  XK_r,      spawn,          {.v = termcmd } },
  { MODKEY|ControlMask,                 0,     -1,  XK_s, 	    spawn,          {.v = scrcmd } },
  { MODKEY|ControlMask,                 0,     -1,  XK_p,      spawn,          {.v = powermenu } },
  { MODKEY|ControlMask,                 0,     -1,  XK_b,      spawn,          {.v = btmenu } },
  { MODKEY,                             0,     -1,  XK_n,      spawn,          SHCMD("alacritty -e nvim") },
	{ MODKEY,                             0,     -1,  XK_b,      togglebar,      {0} },
	{ MODKEY,                             0,     -1,  XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                             0,     -1,  XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,                   0,     -1,  XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,                   0,     -1,  XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                             0,     -1,  XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                             0,     -1,  XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                             0,     -1,  XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                             0,     -1,  XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                             0,     -1,  XK_u,      setlayout,      {.v = &layouts[0]} },
  { Mod4Mask,                           0,     -1,  XK_m,      movecenter,     {0} },
	{ MODKEY,                             0,     -1,  XK_Return, zoom,           {0} },
	{ MODKEY,                             0,     -1,  XK_q,      killclient,     {0} },
	{ MODKEY,                             0,     -1,  XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,                   0,     -1,  XK_t,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                             0,     -1,  XK_f,      togglefullscr,  {0} },
	{ MODKEY,                             0,     -1,  XK_m,      setlayout,      {.v = &layouts[3]} },
  { MODKEY,                             0,     -1,  XK_space,  spawn,          SHCMD("dunstctl close-all") },
	{ MODKEY|ShiftMask,                   0,     -1,  XK_space,  togglefloating, {0} },
	// { MODKEY,                          0,     -1,     XK_0,      view,           {.ui = ~0 } },
	// { MODKEY|ShiftMask,                0,     -1,     XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                             0,     -1,  XK_comma,  focusnthmon,    {.i = 0 } },
	{ MODKEY,                             0,     -1,  XK_period, focusnthmon,    {.i = 1 } },
	{ MODKEY|ShiftMask,                   0,     -1,  XK_comma,  tagnthmon,      {.i = 0 } },
	{ MODKEY|ShiftMask,                   0,     -1,  XK_period, tagnthmon,      {.i = 1 } },
	TAGKEYS(                              0,     -1,  XK_1,                      0)
	TAGKEYS(                              0,     -1,  XK_2,                      1)
	TAGKEYS(                              0,     -1,  XK_3,                      2)
	TAGKEYS(                              0,     -1,  XK_4,                      3)
	TAGKEYS(                              0,     -1,  XK_8,                      4)
	TAGKEYS(                              0,     -1,  XK_9,                      5)
	TAGKEYS(                              0,     -1,  XK_0,                      6)
	{ MODKEY|ShiftMask,                   0,     -1,  XK_q,      quit,           {0} },
	{ MODKEY|ShiftMask,                   0,     -1,  XK_r,      quit,           {1} }, 

  { Mod4Mask|ShiftMask,                 0,     -1,  XK_h,      moveresize,     {.v = "0x 0y -25w 0h" } },
  { Mod4Mask,                           0,     -1,  XK_h,      moveresize,     {.v = "-25x 0y 0w 0h" } },
  { Mod4Mask|ShiftMask,                 0,     -1,  XK_j,      moveresize,     {.v = "0x 0y 0w 25h" } },
  { Mod4Mask,                           0,     -1,  XK_j,      moveresize,     {.v = "0x 25y 0w 0h" } },
  { Mod4Mask|ShiftMask,                 0,     -1,  XK_k,      moveresize,     {.v = "0x 0y 0w -25h" } },
  { Mod4Mask,                           0,     -1,  XK_k,      moveresize,     {.v = "0x -25y 0w 0h" } },
  { Mod4Mask|ShiftMask,                 0,     -1,  XK_l,      moveresize,     {.v = "0x 0y 25w 0h" } },
  { Mod4Mask,                           0,     -1,  XK_l,      moveresize,     {.v = "25x 0y 0w 0h" } },
	{ Mod4Mask|ControlMask,               0,     -1,  XK_h,     moveresizeedge,  {.v = "l"} },
	{ Mod4Mask|ControlMask,               0,     -1,  XK_j,      moveresizeedge, {.v = "b"} },
	{ Mod4Mask|ControlMask,               0,     -1,  XK_k,      moveresizeedge, {.v = "t"} },
	{ Mod4Mask|ControlMask,               0,     -1,  XK_l,      moveresizeedge, {.v = "r"} },
	{ Mod4Mask|ControlMask|ShiftMask,     0,     -1,  XK_h,     moveresizeedge, {.v = "L"} },
	{ Mod4Mask|ControlMask|ShiftMask,     0,     -1,  XK_j,     moveresizeedge, {.v = "B"} },
	{ Mod4Mask|ControlMask|ShiftMask,     0,     -1,  XK_k,     moveresizeedge, {.v = "T"} },
	{ Mod4Mask|ControlMask|ShiftMask,     0,     -1,  XK_l,     moveresizeedge, {.v = "R"} },

  { MODKEY,                             0,     -1,  XK_F5,     spawn,          SHCMD("bright_down") },
  { MODKEY,                             0,     -1,  XK_F6,     spawn,          SHCMD("bright_up") },
  { MODKEY,                             0,     -1,  XK_F7,     spawn,          {.v = umountcmd } },
  { MODKEY,                             0,     -1,  XK_F8,     spawn,          {.v = mountcmd } },
  { MODKEY,                             0,     -1,  XK_F10,    spawn,          SHCMD("playerctl play-pause") },
  { MODKEY,                             0,     -1,  XK_F11,    spawn,          SHCMD("playerctl previous") },
  { MODKEY,                             0,     -1,  XK_F12,    spawn,          SHCMD("playerctl next") },
  { 0,                                  0,     -1,  XK_Print,  spawn,          SHCMD("screenshot") },
  { ControlMask,                        0,		 -1, 	XF86XK_AudioMute,   spawn,          SHCMD("playerctl play-pause") },

  { 0,                                  1,     -1,  XK_1,      spawn,          SHCMD("bluearchive one")},
  { 0,                                  1,     -1,  XK_2,      spawn,          SHCMD("bluearchive two")},
  { 0,                                  1,     -1,  XK_3,      spawn,          SHCMD("bluearchive three")},
  { 0,                                  1,     -1,  XK_s,      spawn,          SHCMD("bluearchive speed")},
  { 0,                                  1,     -1,  XK_a,      spawn,          SHCMD("bluearchive auto")},

  // { 0, 	0,		 -1, 	     XF86XK_AudioLowerVolume, spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -2%") },
  // { 0, 	0,		 -1, 	     XF86XK_AudioRaiseVolume, spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +2%") },
  // { 0, 	0,		 -1, 	     XF86XK_AudioMute,        spawn,          SHCMD("pactl set-sink-mute  @DEFAULT_SINK@ toggle") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
