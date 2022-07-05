/* See LICENSE file for copyright and license details. */

#include <X11/X.h>
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx     = 1;    /* border pixel of windows */
static const unsigned int snap         = 32;   /* snap pixel */
static const int showbar               = 1;    /* 0 means no bar */
static const int topbar                = 1;    /* 0 means bottom bar */
static const int swallowfloating       = 0;    /* 1 means swallow floating windows by default */
static const unsigned int gappih       = 15;   /* horiz inner gap between windows */
static const unsigned int gappiv       = 15;   /* vert inner gap between windows */
static const unsigned int gappoh       = 15;   /* horiz outer gap between windows and screen edge */
static const unsigned int gappov       = 15;   /* vert outer gap between windows and screen edge */
static const int smartgaps             = 0;    /* 1 means no outer gap when there is only one window */
static const int user_bh               = 28;   /* user assigned bar height */
static const int vertpad               = 15;   /* statusbar verical padding */
static const int sidepad               = 15;   /* statusbar side padding */
static const int focusonwheel          = 0;    /* click on focus with mousewheel */
static const unsigned int colorfultag  = 1;    /* 0 means use SchemeSel for selected tag */
static const char *fonts[]             = { "JetBrainsMono Nerd Font:style=Medium:size=9:antialias=true",
// static const char *fonts[]             = { "Iosevka Nerd Font:style=Medium:size=10:antialias=true",
                                           "Material Design Icons-Regular:size=9:antialias=true", };

#include "catppuccin.h"

static const char *colors[][3] = {
  /*                  fg      bg     border */
  [SchemeNorm]    = { gray3,  black, gray2 },
  [SchemeSel]     = { gray4,  blue,  blue },
  [SchemeTag]     = { gray3,  black, black },
  [SchemeTag1]    = { yellow, black, black },
  [SchemeTag2]    = { red,    black, black },
  [SchemeTag3]    = { orange, black, black },
  [SchemeTag4]    = { green,  black, black },
  [SchemeTag5]    = { pink,   black, black },
  [SchemeTag6]    = { blue,   black, black },
  [SchemeTag7]    = { green,  black, black },
  /*[SchemeTag8]  = { yellow, black, black },
  [SchemeTag9]    = { red,    black, black },
  [SchemeTag10]   = { pink,   black, black }, */
  [SchemeLayout]  = { yellow, black, black },
};

static const int tagschemes[] = { 
  SchemeTag1, SchemeTag2, SchemeTag3, SchemeTag4,
  SchemeTag5, SchemeTag6, SchemeTag7,
  // SchemeTag8, SchemeTag9
};

/* tagging */
static char *tags[] = {"󰮯", "󰧞", "󰑊", "󰧞", "󰑊", "󰧞", "󰑊"};

static const unsigned int ulinepad      = 5;    /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke   = 2;    /* thickness / height of the underline */
static const unsigned int ulinevoffset  = 4;    /* how far above the bottom of the bar the line should appear */
static const int ulineall               = 0;    /* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
  /*xprop(1):
    WM_CLASS(STRING) = instance, class
    WM_NAME(STRING) = title*/
  /* class            instance    title            tags  centered  floating  terminal  noswal  mon  spnum*/
  { "st-256color",    NULL,       NULL,            0,    0,        0,        1,        0,      -1,  -1 },
  { "Alacritty",      NULL,       NULL,            0,    0,        0,        1,        0,      -1,  -1 },
  { "Dragon-drop",    NULL,       NULL,            ~0,   1,        1,        0,        1,      -1,  -1 },
  { NULL,             NULL,       "Event Tester",  0,    0,        0,        0,        1,      -1,  -1 },
  { NULL,             "fcen",     NULL,            0,    1,        1,        0,        1,      -1,  -1 },
  { NULL,             "fl",       NULL,            0,    0,        1,        0,        1,      -1,  -1 },
  /* scratchpads*/
  { NULL,		          "spterm",	 	NULL,	           0,		 1,        1,			   1,        0,      -1,  0  },
  { NULL,		          "spfm",	   	NULL,	           0,		 1,        1,			   1,        0,      -1,  1  },
  { NULL,		          "spcalcu",  NULL,	           0,		 1,        1,			   0,        0,      -1,  2  },
  { NULL,		          "spvol",    NULL,	           0,		 1,        1,			   0,        0,      -1,  3  },
  { NULL,		          "sptop",    NULL,	           0,		 1,        1,			   0,        0,      -1,  4  },
  { NULL,		          "spnote",   NULL,	           0,		 1,        1,			   0,        0,      -1,  5  },
  { "discord",        NULL,       NULL,            0,    1,        1,        0,        0,      -1,  6  },
  { "YouTube Music",  NULL,       NULL,            0,    1,        1,        0,        0,      -1,  7  },
};

static const LayoutMonitorRule lm_rules[] = {
 	/* >=w, >=h,   req'd layout, new nmaster, new mfact */
  {  0,   1500,  0,            0,           0.50 },
};


/* layout(s) */
static const float mfact = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "󰊠",        tile },    /* first entry is default */
  { "|M|",      centeredmaster },
  { "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(CHAIN,KEY,TAG) \
  { MODKEY,                        CHAIN,  KEY,  view,        {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask,            CHAIN,  KEY,  toggleview,  {.ui = 1 << TAG} }, \
  { MODKEY|ShiftMask,              CHAIN,  KEY,  tag,         {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask,  CHAIN,  KEY,  toggletag,   {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask,               CHAIN,  KEY,  tagnextmon,  {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask|ShiftMask,     CHAIN,  KEY,  tagprevmon,  {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static char monsize[1920];
/* dmenu */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-x", "15", "-y", "15", "-z", monsize, NULL };
static const char *clipcmd[] = { "clipmenu", "-m", dmenumon, "-x", "15", "-y", "15", "-z", monsize, NULL };
static const char *passcmd[] = { "passmenu", "-m", dmenumon, "-x", "15", "-y", "15", "-z", monsize, NULL };
static const char *mountcmd[] = { "dmenumount", "-m", dmenumon, "-x", "15", "-y", "15", "-z", monsize, NULL };
static const char *umountcmd[] = { "dmenuumount", "-m", dmenumon, "-x", "15", "-y", "15", "-z", monsize, NULL };
static const char *wikicmd[] = { "wikimenu", "-m", dmenumon, "-x", "15", "-y", "15", "-z", monsize, NULL };

static const char *termcmd[]  = { "st", NULL };

typedef struct {
  const char *name;
  const void *cmd;
} Sp;

/* st cmds */
const char *spterm[] = {"st", "-n", "spterm", "-g", "125x34", NULL };
const char *spfm[] = {"st", "-n", "spfm", "-g", "110x33", "-e", "lfrun", NULL };
const char *spbc[] = {"st", "-n", "spcalcu", "-g", "80x30", "-e", "bc", "-lq", NULL };
const char *sppm[] = {"st", "-n", "spvol", "-g", "100x15", "-e", "pulsemixer", NULL };
const char *sptop[] = {"st", "-n", "sptop", "-g", "115x35", "-e", "btop", NULL };
const char *sporg[] = {"st", "-n", "spnote", "-g", "125x34", "-e", "nvim", "-c :call cursor(3,3)", "-c cd ~/personal/org", "/home/john/personal/org/index.org", NULL };

/* alacritty cmd */
/* const char *spcmd1[] = {"alacritty", "--class=spterm", "-o", "window.dimensions.columns=125", "window.dimensions.lines=34", NULL };
const char *spcmd2[] = {"alacritty", "--class=spfm", "-o", "window.dimensions.columns=110", "window.dimensions.lines=33", "-e", "lfrun", NULL };
const char *spcmd3[] = {"alacritty", "--class=spcalcu", "-o", "window.dimensions.columns=80", "window.dimensions.lines=30", "-e", "bc", "-lq", NULL };
const char *spcmd4[] = {"alacritty", "--class=spvol", "-o", "window.dimensions.columns=100", "window.dimensions.lines=15", "-e", "pulsemixer", NULL };
const char *spcmd5[] = {"alacritty", "--class=sptop", "-o", "window.dimensions.columns=115", "window.dimensions.lines=35", "-e", "btop", NULL };
const char *spcmd6[] = {"alacritty", "--class=spcurse" "-o", "window.dimensions.columns=100", "window.dimensions.lines=30", "-e", "calcurse", NULL };
const char *spcmd7[] = {"alacritty", "--class=spnote", "-o", "window.dimensions.columns=125", "window.dimensions.lines=34", "-e", "nvim", "+3", "-c", "cd ~/personal/org", "/home/john/personal/org/index.org", NULL }; */

const char *dcmd[] = {"discocss", NULL };
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

/* key bindings */
#include "movestack.c"
static Key keys[] = {
  /* modifier                     chain key      key        function        argument */
  { MODKEY|ShiftMask,             -1,            XK_Return, togglesp,       {.ui = 0 } },
  { MODKEY,                       XK_s,          XK_f,      togglesp,       {.ui = 1 } },
  { MODKEY,                       XK_s,          XK_F9,     togglesp,       {.ui = 2 } },
  { MODKEY|ControlMask,           -1,            XK_v,      togglesp,       {.ui = 3 } },
  { MODKEY,                       -1,          XK_g,      togglesp,       {.ui = 4 } },
  { MODKEY,                       XK_s,          XK_n,      togglesp,       {.ui = 5 } },
  { MODKEY,                       XK_s,          XK_d,      togglesp,       {.ui = 6 } },
  { MODKEY,                       XK_s,          XK_y,      togglesp,       {.ui = 7 } },

  { MODKEY,                       -1,            XK_grave,  view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             -1,            XK_grave,  tag,            {.ui = ~0 } },
  TAGKEYS(                        -1,            XK_1,                      0)
  TAGKEYS(                        -1,            XK_2,                      1)
  TAGKEYS(                        -1,            XK_3,                      2)
  TAGKEYS(                        -1,            XK_4,                      3)
  TAGKEYS(                        -1,            XK_8,                      4)
  TAGKEYS(                        -1,            XK_9,                      5)
  TAGKEYS(                        -1,            XK_0,                      6)

  { MODKEY,                       -1,            XK_Tab,    spawn,          {.v = clipcmd } },
  { MODKEY,                       -1,            XK_q,      killclient,     {0} },
  { MODKEY|ShiftMask,             -1,            XK_q,      quit,           {0} },
  { MODKEY,                       -1,            XK_w,      spawn,          {.v = wikicmd } },
  { MODKEY,                       -1,            XK_r,      spawn,          {.v = termcmd } },
  { MODKEY|ShiftMask,             -1,            XK_r,      quit,           {1} },
  { MODKEY,                       -1,            XK_t,      setlayout,      {.v = &layouts[0]} },
  { MODKEY,                       -1,            XK_i,      incnmaster,     {.i = +1 } },
  { MODKEY,                       -1,            XK_p,      spawn,          {.v = dmenucmd } },
  { MODKEY|ControlMask,           -1,            XK_p,      spawn,          {.v = passcmd } },

  { MODKEY,                       XK_c,          XK_s, 	    restorewin,     {0}},
  { MODKEY,                       -1,            XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       -1,            XK_f,      togglefullscr,  {0} },
  { MODKEY,                       -1,            XK_h,      setmfact,       {.f = -0.025 } },
  { MODKEY,                       XK_c,          XK_h, 	    hidewin,        {0}},
  { MODKEY|ShiftMask,             -1,            XK_h,      setcfact,       {.f = -0.20 } },
  { Mod4Mask|ShiftMask,           -1,            XK_h,      moveresize,     {.v = "0x 0y -25w 0h" } },
  { Mod4Mask,                     -1,            XK_h,      moveresize,     {.v = "-25x 0y 0w 0h" } },
  { MODKEY,                       -1,            XK_j,      focusstack,     {.i = +1 } },
  { MODKEY|ShiftMask,             -1,            XK_j,      movestack,      {.i = +1 } },
  { Mod4Mask|ShiftMask,           -1,            XK_j,      moveresize,     {.v = "0x 0y 0w 25h" } },
  { Mod4Mask,                     -1,            XK_j,      moveresize,     {.v = "0x 25y 0w 0h" } },
  { MODKEY,                       -1,            XK_k,      focusstack,     {.i = -1 } },
  { MODKEY|ShiftMask,             -1,            XK_k,      movestack,      {.i = -1 } },
  { Mod4Mask|ShiftMask,           -1,            XK_k,      moveresize,     {.v = "0x 0y 0w -25h" } },
  { Mod4Mask,                     -1,            XK_k,      moveresize,     {.v = "0x -25y 0w 0h" } },
  { MODKEY,                       -1,            XK_l,      setmfact,       {.f = +0.025 } },
  { MODKEY|ShiftMask,             -1,            XK_l,      setcfact,       {.f = +0.20 } },
  { Mod4Mask|ShiftMask,           -1,            XK_l,      moveresize,     {.v = "0x 0y 25w 0h" } },
  { Mod4Mask,                     -1,            XK_l,      moveresize,     {.v = "25x 0y 0w 0h" } },
  { MODKEY,                       -1,            XK_Return, zoom,           {0} },

  { MODKEY,                       -1,            XK_b,      togglebar,      {0} },
  { MODKEY|ControlMask,           -1,            XK_b,      spawn,          SHCMD("bluetoothdmenu.sh") },
  // { MODKEY,                       -1,            XK_m,      setlayout,      {.v = &layouts[1]} },
  { Mod4Mask,                     -1,            XK_m,      movecenter,     {0} },
  { MODKEY,                       -1,            XK_comma,  focusmonx,      {.i = 0 } },
  { MODKEY|ShiftMask,             -1,            XK_comma,  tagmon,         {.i = -1 } },
  { MODKEY,                       -1,            XK_period, focusmonx,      {.i = 1 } },
  { MODKEY|ShiftMask,             -1,            XK_period, tagmon,         {.i = +1 } },
  { MODKEY,                       -1,            XK_space,  spawn,          SHCMD("dunstctl close-all") },
  { MODKEY|ShiftMask,             -1,            XK_space,  togglefloating, {0} },

  { MODKEY,                       -1,            XK_F5,     spawn,          SHCMD("sudo xbacklight -dec 5") },
  { MODKEY,                       -1,            XK_F6,     spawn,          SHCMD("sudo xbacklight -inc 5") },
  { MODKEY,                       -1,            XK_F7,     spawn,          {.v = umountcmd } },
  { MODKEY,                       -1,            XK_F8,     spawn,          {.v = mountcmd } },
  { MODKEY,                       -1,            XK_F10,    spawn,          SHCMD("playerctl play-pause") },
  { 0,                            -1,            XK_Print,  spawn,          SHCMD("screenshot") },
  { 0,                            -1,            XF86XK_AudioPlay, spawn,   SHCMD("playerctl play-pause") },
  { 0,                            -1,            XF86XK_AudioNext, spawn,   SHCMD("playerctl next") },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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
