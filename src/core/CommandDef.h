#pragma once
#include <QString>
#include <QList>

// ─────────────────────────────────────────────────────────────────────────────
//  CommandDefinition — describes one button in the Command Center
//
//  label        : text shown on the button
//  serialCmd    : exact string sent over UART to the CanSat (+ '\n' auto-appended)
//  color        : button text / border color  (CSS hex)
//  bgColor      : button background color     (CSS rgba)
//  needsConfirm : if true, a confirmation dialog appears before sending
//  enabled      : set false to show button but disable it (greyed out)
// ─────────────────────────────────────────────────────────────────────────────
struct CommandDefinition {
    QString label;
    QString serialCmd;
    QString color;
    QString bgColor;
    bool    needsConfirm = false;
    bool    enabled      = true;
};

// ─────────────────────────────────────────────────────────────────────────────
//  CommandRegistry — loads the command list defined in Commands.cpp
// ─────────────────────────────────────────────────────────────────────────────
class CommandRegistry {
public:
    // Returns the full list of commands defined in Commands.cpp
    static const QList<CommandDefinition>& commands();
};
