/* See LICENSE file for copyright and license details. */

/* Windows allowed above a custom fullscreen window.
 * Kept separate from Rule so this patch can be used without scratchpads. */
/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int usealtbar          = 1;        /* 1 means use non-dwm status bar */
static const char *altbarclass      = "Polybar"; /* Alternate bar class name */
static const char *altbarcmd        = "$HOME/bar.sh"; /* Alternate bar launch command */
static const char *fonts[]          = { "JetBrainsMono Nerd Font:style=Regular:size=11.5:antialias=true" };
static const char base[]       = "#1E1E2E";
static const char gray2[]       = "#313244"; // unfocused window border
static const char peach[]      = "#fab387";
static const char *colors[][3]      = {
  /*               fg         bg         border   */
  [SchemeNorm]       = { peach,          base,     gray2 },
  [SchemeSel]        = { peach,          base,     peach },
};

/* SpNone must remain zero; IDs are shared by rules, commands and bindings. */
enum { SpNone, SpTerm, SpFm, SpCalcu, SpVol, SpTop, SpDiscord, YtMsc};

typedef struct {
  char *rule;
  const void *cmd;
} Sp;

const char *spterm[] = {"alacritty", "--class=spterm", "-o", "window.dimensions.columns=125", "window.dimensions.lines=34", NULL };
const char *spfm[] = {"alacritty", "--class=spfm", "-o", "window.dimensions.columns=110", "window.dimensions.lines=33", "-e", "lfrun", NULL };
const char *spbc[] = {"alacritty", "--class=spcalcu", "-o", "window.dimensions.columns=80", "window.dimensions.lines=30", "-e", "bc", "-lq", NULL };
const char *sppm[] = {"alacritty", "--class=spvol", "-o", "window.dimensions.columns=100", "window.dimensions.lines=15", "-e", "pulsemixer", NULL };
const char *sptop[] = {"alacritty", "--class=sptop", "-o", "window.dimensions.columns=115", "window.dimensions.lines=35", "-e", "btop", NULL };
const char *ytmsc[] = {"alacritty", "--class=ytmsc", "-o", "window.dimensions.columns=125", "window.dimensions.lines=35", "-e", "ytmusic-tui", NULL };

const char *dcmd[] = {"discord", NULL };

static Sp scratchpads[] = {
  /* Designated indices keep IDs aligned even when entries are reordered. */
  /* ID             name          cmd  */
  [SpTerm]    = {"spterm",       spterm},
  [SpFm]      = {"splf",         spfm},
  [SpCalcu]   = {"spcalcu",      spbc},
  [SpVol]     = {"spvol",        sppm},
  [SpTop]     = {"sptop",        sptop},
  [YtMsc]     = {"ytmsc",        ytmsc},
  [SpDiscord] = {"dcmd",         dcmd},
};

/* tagging */
static char *tags[] = {"󰮯", "󰧞", "󰑊", "󰧞", "󰑊", "󰧞", "󰑊", "󰧞"};

static const FullscreenRule fullscreenrules[] = {
  /* class         instance    title    ignorefs */
  { NULL,          "spterm",   NULL,    1 },
  { NULL,          "spcalc",   NULL,    1 },
  { "Dragon-drop", NULL,       NULL,    1 },
};

static const Rule rules[] = {
  /* xprop(1):
   *	WM_CLASS(STRING) = instance, class
   *	WM_NAME(STRING) = title
   */
  /* class           instance   title           tags centered floating terminal noswal monitor spnum */
  { "st-256color",   NULL,      NULL,           0,   0,       0,       1,       0,     -1, SpNone },
  { "Alacritty",     NULL,      NULL,           0,   0,       0,       1,       0,     -1, SpNone },
  { "Dragon-drop",   NULL,      NULL,           ~0,  1,       1,       0,       1,     -1, SpNone },
  { NULL,            NULL,      "Event Tester", 0,   0,       0,       0,       1,     -1, SpNone },
  { NULL,            "fcen",    NULL,           0,   1,       1,       0,       1,     -1, SpNone },
  { NULL,            "fl",      NULL,           0,   0,       1,       0,       1,     -1, SpNone },

  /* scratchpads: use the same enum ID here and in scratchpads[] */
  { NULL,		         "spterm",	NULL,	          0,	 1,       1,			 1,       0,     -1, SpTerm },
  { NULL,		         "spfm",	  NULL,	          0,	 1,       1,			 1,       0,     -1, SpFm },
  { NULL,		         "spcalcu", NULL,	          0,	 1,       1,			 0,       0,     -1, SpCalcu },
  { NULL,		         "spvol",   NULL,	          0,	 1,       1,			 0,       0,     -1, SpVol },
  { NULL,		         "sptop",   NULL,	          0,	 1,       1,			 0,       0,     -1, SpTop },
  { NULL,		         "ytmsc",   NULL,	          0,	 1,       1,			 0,       0,     -1, YtMsc },
  { "discord",       NULL,      NULL,           0,   1,       1,       0,       0,     -1, SpDiscord },
};

/* layout(s) */
static const float mfact     = 0.45; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

static const LayoutMonitorRule lm_rules[] = {
  /* >=w, >=h, req'd layout, new nmaster, new mfact */
  { 0, 2000,   0,            0,           0.50 },
};

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "|M|",      centeredmaster },
  { "[]=",      tile },    /* first entry is default */
  { "><>",      NULL },    /* no layout function means floating behavior */
  { "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
  &((Keychord){1, {{ MODKEY,                       KEY}},      view,           {.ui = 1 << TAG} }), \
  &((Keychord){1, {{ MODKEY|ControlMask,           KEY}},      toggleview,     {.ui = 1 << TAG} }), \
  &((Keychord){1, {{ MODKEY|ShiftMask,             KEY}},      tag,            {.ui = 1 << TAG} }), \
  &((Keychord){1, {{ MODKEY|ControlMask|ShiftMask, KEY}},      toggletag,      {.ui = 1 << TAG} }), \

// dmenu
static const char dmenuln[] = "10";
static const char dmenulh[] = "40";
static const char dmenuwidth[] = "700";
static char dmenux[12];
static char dmenuy[12];
static char dmenumon[12];
#define DMENUS(NAME,CMD) \
  static const char *NAME[] = { CMD, "-m", dmenumon, "-x", dmenux, "-y", dmenuy, "-z", dmenuwidth, "-h", dmenulh, "-l", dmenuln,  NULL };

DMENUS(dmenucmd,   "dmenu_run")
DMENUS(clipcmd,    "clipmenu")
DMENUS(powermenu,  "power.sh")
DMENUS(btmenu,     "bluetoothdmenu.sh")
DMENUS(screenshot, "screenshot")

/* commands */
static const char *termcmd[]  = { "alacritty", NULL };

static const Keychord *keychords[] = {
  /* modifier                     key        function        argument */
  &((Keychord){1, {{ MODKEY|ShiftMask, XK_Return}},   togglesp,       {.i = SpTerm } }),
  &((Keychord){2, {{ MODKEY, XK_s}, {0, XK_f}},       togglesp,       {.i = SpFm } }),
  &((Keychord){2, {{ MODKEY, XK_s}, {0, XK_F9}},      togglesp,       {.i = SpCalcu } }),
  &((Keychord){1, {{ MODKEY|ControlMask, XK_v}},      togglesp,       {.i = SpVol } }),
  &((Keychord){1, {{ MODKEY, XK_g}},                  togglesp,       {.i = SpTop} }),
  &((Keychord){2, {{ MODKEY, XK_s}, {0, XK_d}},       togglesp,       {.i = SpDiscord} }),
  &((Keychord){2, {{ MODKEY, XK_s}, {0, XK_y}},       togglesp,       {.i = YtMsc } }),


  &((Keychord){1, {{MODKEY, XK_p}},      spawn,          {.v = dmenucmd } }),
  &((Keychord){1, {{MODKEY, XK_Print}},  spawn,          {.v = screenshot } }),
  &((Keychord){1, {{MODKEY, XK_r}},      spawn,          {.v = termcmd } }),
  &((Keychord){1, {{MODKEY, XK_b}},      togglebar,      {0} }),
  &((Keychord){1, {{MODKEY, XK_j}},      focusstack,     {.i = +1 } }),
  &((Keychord){1, {{MODKEY, XK_k}},      focusstack,     {.i = -1 } }),
  &((Keychord){1, {{MODKEY, XK_i}},      incnmaster,     {.i = +1 } }),
  &((Keychord){1, {{MODKEY, XK_d}},      incnmaster,     {.i = -1 } }),
  &((Keychord){1, {{MODKEY, XK_Down}},   moveresize,     {.v = "0x 25y 0w 0h" } }),
  &((Keychord){1, {{MODKEY, XK_Up}},     moveresize,     {.v = "0x -25y 0w 0h" } }),
  &((Keychord){1, {{MODKEY, XK_Right}},  moveresize,     {.v = "25x 0y 0w 0h" } }),
  &((Keychord){1, {{MODKEY, XK_Left}},   moveresize,     {.v = "-25x 0y 0w 0h" } }),
  &((Keychord){1, {{MODKEY|ShiftMask, XK_Down}},   moveresize,     {.v = "0x 0y 0w 25h" } }),
  &((Keychord){1, {{MODKEY|ShiftMask, XK_Up}},     moveresize,     {.v = "0x 0y 0w -25h" } }),
  &((Keychord){1, {{MODKEY|ShiftMask, XK_Right}},  moveresize,     {.v = "0x 0y 25w 0h" } }),
  &((Keychord){1, {{MODKEY|ShiftMask, XK_Left}},   moveresize,     {.v = "0x 0y -25w 0h" } }),
  &((Keychord){1, {{MODKEY|ControlMask, XK_Up}},     moveresizeedge, {.v = "t"} }),
  &((Keychord){1, {{MODKEY|ControlMask, XK_Down}},   moveresizeedge, {.v = "b"} }),
  &((Keychord){1, {{MODKEY|ControlMask, XK_Left}},   moveresizeedge, {.v = "l"} }),
  &((Keychord){1, {{MODKEY|ControlMask, XK_Right}},  moveresizeedge, {.v = "r"} }),
  &((Keychord){1, {{MODKEY|ControlMask|ShiftMask, XK_Up}},     moveresizeedge, {.v = "T"} }),
  &((Keychord){1, {{MODKEY|ControlMask|ShiftMask, XK_Down}},   moveresizeedge, {.v = "B"} }),
  &((Keychord){1, {{MODKEY|ControlMask|ShiftMask, XK_Left}},   moveresizeedge, {.v = "L"} }),
  &((Keychord){1, {{MODKEY|ControlMask|ShiftMask, XK_Right}},  moveresizeedge, {.v = "R"} }),
  &((Keychord){1, {{MODKEY, XK_h}},      setmfact,       {.f = -0.05} }),
  &((Keychord){1, {{MODKEY, XK_l}},      setmfact,       {.f = +0.05} }),
  &((Keychord){1, {{MODKEY|ShiftMask, XK_h}},      setcfact,       {.f = +0.25} }),
  &((Keychord){1, {{MODKEY|ShiftMask, XK_l}},      setcfact,       {.f = -0.25} }),
  &((Keychord){1, {{MODKEY|ShiftMask, XK_o}},      setcfact,       {.f =  0.00} }),
  &((Keychord){1, {{MODKEY, XK_Return}}, zoom,           {0} }),
  &((Keychord){1, {{MODKEY, XK_Tab}},    spawn,           {.v = clipcmd} }),
  &((Keychord){1, {{MODKEY, XK_q}},      killclient,     {0} }),
  &((Keychord){1, {{MODKEY, XK_u}},      setlayout,      {.v = &layouts[0]} }),
  &((Keychord){1, {{MODKEY, XK_t}},      setlayout,      {.v = &layouts[1]} }),
  &((Keychord){1, {{MODKEY, XK_m}},      setlayout,      {.v = &layouts[2]} }),
  &((Keychord){1, {{MODKEY, XK_space}},  setlayout,      {0} }),
  &((Keychord){1, {{MODKEY|ShiftMask, XK_space}},  togglefloating, {0} }),
  &((Keychord){1, {{MODKEY|ShiftMask, XK_f}},      togglefullscr,  {0} }),
  &((Keychord){1, {{MODKEY, XK_comma}},  focusnthmon,       {.i = 1 } }),
  &((Keychord){1, {{MODKEY, XK_period}}, focusnthmon,       {.i = 0 } }),
  &((Keychord){1, {{MODKEY|ShiftMask, XK_comma}},  tagnthmon,         {.i = 1 } }),
  &((Keychord){1, {{MODKEY|ShiftMask, XK_period}}, tagnthmon,         {.i = 0 } }),
  &((Keychord){1, {{MODKEY, XK_f}}, togglefullscr,         {0} }),
  TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_7,                      4)
    TAGKEYS(                        XK_8,                      5)
    TAGKEYS(                        XK_9,                      6)
    TAGKEYS(                        XK_0,                      7)
    &((Keychord){1, {{MODKEY|ShiftMask,             XK_r}},      quit,           {1} }),
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
  { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkWinTitle,          0,              Button2,        zoom,           {0} },
  { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

