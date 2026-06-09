#include "CommandDef.h"

const QList<CommandDefinition>& CommandRegistry::commands()
{
    static const QList<CommandDefinition> cmds = {
        { "CALIBRATE", "CMD,CALIBRATE",  "#79c0ff", "rgba(121,192,255,0.1)", true,         true },
        { "ARM",       "CMD,ARM",        "#f0883e", "rgba(240,136,62,0.1)",  true,         true },
        { "DEPLOY",    "CMD,DEPLOY",     "#d2a8ff", "rgba(210,168,255,0.1)", true,         true },
        { "START MTR", "CMD,MTR_START",  "#3fb950", "rgba(63,185,80,0.1)",   false,        true },
        { "STOP MTR",  "CMD,MTR_STOP",   "#f85149", "rgba(248,81,73,0.1)",   false,        true },
        { "BUZZ ON",   "CMD,BUZ_ON",     "#e3b341", "rgba(227,179,65,0.1)",  false,        true },
        { "BUZZ OFF",  "CMD,BUZ_OFF",    "#8b949e", "rgba(139,148,158,0.1)", false,        true },
        { "REQ GPS",   "CMD,REQ_GPS",    "#58a6ff", "rgba(88,166,255,0.1)",  false,        true },
        { "RESET",     "CMD,RESET",      "#f85149", "rgba(248,81,73,0.15)",  true,         true },
    };
    return cmds;
}
