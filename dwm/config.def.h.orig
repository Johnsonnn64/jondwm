/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const unsigned int gappih    = 15;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 15;       /* vert inner gap between windows */
static const unsigned int gappoh    = 15;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 15;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int user_bh            = 30;       /* user assigned bar height */
static const int vertpad                 = 0;        /* statusbar verical padding */
static const int sidepad                 = 15;       /* statusbar side padding */
static const unsigned int colorfultag    = 1;   /* 0 means use SchemeSel for selected tag */
static const char *fonts[]               = { "JetBrainsMono Nerd Font:style=Medium:size=9:antialias=true",
                                             "Material Design Icons-Regular:size=10:antialias=true", };

#include "catppuccin.h"

static const char *colors[][3]      = {
  /*                     fg       bg      border */
  [SchemeNorm]       = { gray3, black, gray2 },
  [SchemeSel]        = { gray4, blue,  blue },
  [SchemeTag]        = { gray3, black, black },
  [SchemeTag1]       = { yellow,  black, black },
  [SchemeTag2]       = { red,   black, black },
  [SchemeTag3]       = { orange, black,black },
  [SchemeTag4]       = { green, black, black },
  [SchemeTag5]       = { pink,  black, black },
  [SchemeTag6]       = { blue,   black, black },
  [SchemeTag7]       = { green, black,black },
  /*[SchemeTag8]       = { yellow, black, black },
  [SchemeTag9]       = { red,  black, black },
  [SchemeTag10]      = { pink,  black, black }, */
  [SchemeLayout]     = { yellow, black, black },
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
static const unsigned int ulinevoffset  = 0;    /* how far above the bottom of the bar the line should appear */
static const int ulineall               = 0;    /* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
  /*xprop(1):
    WM_CLASS(STRING) = instance, class
    WM_NAME(STRING) = title*/
  /* class            instance    title         tags mask     iscentered   isfloating   monitor */
  { "zoom",           NULL,        NULL,        1 << 6,       1,           1,           -1 },
  { "YouTube Music",  NULL,        NULL,        1 << 5,       0,           0,            1 },
  { "discord",        NULL,        NULL,        1 << 5,       0,           0,            0 },
  { NULL,             "fcen",      NULL,        0,            1,           1,           -1 },
  { NULL,             "fl",        NULL,        0,            0,           1,           -1 },
  /* scratchpads*/
  { NULL,		          "spterm",	 	 NULL,		    SPTAG(0),		  0,           1,			      -1 },
  { NULL,		          "spfm",	   	 NULL,		    SPTAG(1),		  0,           1,			      -1 },
  { NULL,		          "spcalcu",   NULL,		    SPTAG(2),		  0,           1,			      -1 },
  { NULL,		          "spvolume",  NULL,		    SPTAG(3),		  1,           1,			      -1 },
  { NULL,		          "spgotop",   NULL,		    SPTAG(4),		  1,           1,			      -1 },
  { NULL,		          "spcurse",   NULL,		    SPTAG(5),		  0,           1,			      -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "󰊠",      tile },    /* first entry is default */
  { "|M|",      centeredmaster },
  { "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
  { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
  { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask,              KEY,      tagnextmon,     {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask|ShiftMask,    KEY,      tagprevmon,     {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, /*"-l", "12",*/ "-x", "15", "-z", "1890", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *sscmd[] = { "scrot", "-F", "screenshot.png", "-o", "-s", "-f", "-e", "mv screenshot.png ~/personal/pictures/ && xclip -selection clipboard -target image/png -i ~/personal/pictures/screenshot.png", NULL};

typedef struct {
  const char *name;
  const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "lfrun", NULL };
const char *spcmd3[] = {"st", "-n", "spcalcu", "-g", "80x30", "-e", "bc", "-lq", NULL };
const char *spcmd4[] = {"st", "-n", "spvolume", "-g", "115x20", "-e", "pulsemixer", NULL };
const char *spcmd5[] = {"st", "-n", "spgotop", "-g", "115x35", "-e", "gotop", NULL };
const char *spcmd6[] = {"st", "-n", "spcurse", "-g", "100x30+1200+5", "-e", "calcurse", NULL };
static Sp scratchpads[] = {
  /* name          cmd  */
  {"spterm",       spcmd1},
  {"splf",         spcmd2},
  {"spcalcu",      spcmd3},
  {"spvolume",     spcmd4},
  {"spgotop",      spcmd5},
  {"spcurse",      spcmd6},
};

#include "movestack.c"
static Key keys[] = {
  /* modifier                     key        function        argument */

  { MODKEY,                       XK_grave,  view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_grave,  tag,            {.ui = ~0 } },
  TAGKEYS(                        XK_1,                      0)
  TAGKEYS(                        XK_2,                      1)
  TAGKEYS(                        XK_3,                      2)
  TAGKEYS(                        XK_4,                      3)
  TAGKEYS(                        XK_8,                      4)
  TAGKEYS(                        XK_9,                      5)
  TAGKEYS(                        XK_0,                      6)

  { MODKEY,                       XK_Tab,    spawn,          SHCMD("clipmenu -x 15 -z 1890 -l 12") },
  { MODKEY,                       XK_q,      killclient,     {0} },
  { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
  { MODKEY,                       XK_r,      spawn,          {.v = termcmd } },
  { MODKEY|ShiftMask,             XK_r,      quit,           {1} },
  { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
  { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
  { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },

  { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
  { MODKEY,                       XK_g,      togglescratch,  { .ui = 4 } },
  { MODKEY,                       XK_h,      setmfact,       {.f = -0.025 } },
  { MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = -0.25 } },
  { MODKEY|ShiftMask|ControlMask, XK_h,      moveresize,     {.v = "0x 0y -25w 0h" } },
  { ControlMask|ShiftMask,        XK_h,      moveresize,     {.v = "-25x 0y 0w 0h" } },
  { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
  { MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
  { MODKEY|ShiftMask|ControlMask, XK_j,      moveresize,     {.v = "0x 0y 0w 25h" } },
  { ControlMask|ShiftMask,        XK_j,      moveresize,     {.v = "0x 25y 0w 0h" } },
  { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
  { MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
  { MODKEY|ShiftMask|ControlMask, XK_k,      moveresize,     {.v = "0x 0y 0w -25h" } },
  { ControlMask|ShiftMask,        XK_k,      moveresize,     {.v = "0x -25y 0w 0h" } },
  { MODKEY,                       XK_l,      setmfact,       {.f = +0.025 } },
  { MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = +0.25 } },
  { MODKEY|ShiftMask|ControlMask, XK_l,      moveresize,     {.v = "0x 0y 25w 0h" } },
  { ControlMask|ShiftMask,        XK_l,      moveresize,     {.v = "25x 0y 0w 0h" } },
  { MODKEY,                       XK_Return, zoom,           {0} },
  { MODKEY|ShiftMask,             XK_Return, togglescratch,  {.ui = 0 } },

  { MODKEY|ControlMask,           XK_c,      togglescratch,  {.ui = 5} },
  { MODKEY|ControlMask,           XK_v,      togglescratch,  {.ui = 3} },
  { MODKEY,                       XK_b,      togglebar,      {0} },
  { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
  { MODKEY|ShiftMask,             XK_m,      movecenter,     {0} },
  { MODKEY,                       XK_n,      togglescratch,  {.ui = 1} },
  { MODKEY,                       XK_comma,  focusmonx,      {.i = 0 } },
  { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
  { MODKEY,                       XK_period, focusmonx,      {.i = 1 } },
  { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },

  { MODKEY,                       XK_F7,     spawn,          SHCMD("dmenuumount.sh") },
  { MODKEY,                       XK_F8,     spawn,          SHCMD("dmenumount.sh") },
  { MODKEY,                       XK_F9,     togglescratch,  {.ui = 2 } },
  { MODKEY,                       XK_F10,    spawn,          SHCMD("playerctl play-pause") },
  { MODKEY,                       XK_F11,    spawn,          SHCMD("playerctl previous") },
  { MODKEY,                       XK_F12,    spawn,          SHCMD("playerctl next") },
  { 0,                            XK_Print,  spawn,          {.v = sscmd } },
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
